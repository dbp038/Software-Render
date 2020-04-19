#pragma once
#include "RenderTarget.h"
#include "gfx/context/Viewport.h"
#include "Color.h"
#include "ClippingCullingUnit.h"
#include <omp.h>

class RenderPipeline {

	template<typename CtxType>
	using InputVertex = typename CtxType::InputVertex;
	template<typename CtxType>
	using VS = typename CtxType::VS;
	template<typename CtxType>
	using PS = typename CtxType::PS;
	template<typename CtxType>
	using VSOut = typename CtxType::VSOut;
	template<typename CtxType>
	using GSOut = typename CtxType::GSOut;

public:

	RenderPipeline() = delete;
	RenderPipeline( RenderTarget &renderTarget );

	template<typename CtxType>
	void DrawCall( CtxType &ctx ) {

		VertexProcessingStage( ctx );
		// - Primitive Assembly stage
		// - Vertex Post-Processing stage
		// - RaSterization stage
		// - Pixel Shader stage
		// - Output Merger stage
		// These stages are called from within VertexProcessingStage function in this order
		// in a nested manner
	}

private:

	RenderTarget &rt;

	template<typename CtxType>
	void VertexProcessingStage( CtxType &ctx ) {
		auto &verticesIn = ctx.GetVertices();
		auto &indicesIn = ctx.GetIndices();
		VS<CtxType> &vertexShader = ctx.GetVertexShaderData();
		std::vector<VSOut<CtxType>> vertices;
		vertices.resize( verticesIn.size() );
		// execute vertex shader for each vertex
#pragma omp parallel for
		for ( int i = 0; i < verticesIn.size(); i++ ) {
			vertices[ i ] = vertexShader( verticesIn[ i ] );
		}

		PrimitiveAssemblyStage( ctx, vertices, indicesIn );
	}

	template<typename CtxType>
	void PrimitiveAssemblyStage( CtxType &ctx, std::vector<VSOut<CtxType>> &vertices, const std::vector<size_t> &indices ) {
		size_t size = indices.size();
		int threadsDone = 0, nextTriangle = 0;

		struct ThreadSchedule {
			size_t assignedTriangle;
			bool hasWork;
		};
		int numThreads = omp_get_max_threads();
		std::vector<ThreadSchedule> threads;
		threads.resize( size_t( numThreads ) - 1 );

		for ( int i = 0; i < numThreads - 1; i++ ) {
			threads[ i ].assignedTriangle = nextTriangle;
			threads[ i ].hasWork = true;
			nextTriangle++;
		}
#pragma omp parallel num_threads(numThreads)
		{
			int threadId = omp_get_thread_num();
			if ( threadId == numThreads - 1 ) {
				// this thread is going to manage the work of the other threads
				// wait until all threads finished their work
				while ( threadsDone < numThreads - 1 ) {
					for ( int i = 0; i < numThreads - 1; i++ ) {
						if ( !threads[ i ].hasWork ) {
							// assign the next triangle to a thread
							threads[ i ].assignedTriangle = nextTriangle;
							nextTriangle++;
							threads[ i ].hasWork = true;
						}
					}
				}
			}
			else {
				// all other threads are going to process triangles
				ClippingCullingUnit<GSOut<CtxType>> clipCullUnit;
				int privateTriangle = 0;
				size_t vertexIdx = 0;
				while ( true ) {
					if ( threads[ threadId ].hasWork ) {
						privateTriangle = threads[ threadId ].assignedTriangle;
						vertexIdx = privateTriangle * 3;
						threads[ threadId ].hasWork = false;

						if ( vertexIdx >= size )
							break;

						VSOut<CtxType> &v0 = vertices[ indices[ vertexIdx ] ];
						VSOut<CtxType> &v1 = vertices[ indices[ ++vertexIdx ] ];
						VSOut<CtxType> &v2 = vertices[ indices[ ++vertexIdx ] ];

						VertexPostProcessStage( ctx, ctx.gs( privateTriangle, v0, v1, v2 ), clipCullUnit );
					}
				}
			}
#pragma omp atomic
			threadsDone++;
		}
	}

	template<typename CtxType>
	void VertexPostProcessStage( CtxType &ctx, const std::array<GSOut<CtxType>, 3> &triangle, ClippingCullingUnit<GSOut<CtxType>> &clipCullUnit ) {
		const GSOut<CtxType> &v0 = triangle[ 0 ];
		const GSOut<CtxType> &v1 = triangle[ 1 ];
		const GSOut<CtxType> &v2 = triangle[ 2 ];
		// Geometry based clipping and culling
		int i;
		clipCullUnit.SetInitialTriangle( v0, v1, v2 );

		// we do clipping for znear before the perspective divide to avoid problems when
		// trying to interpolate too close to the origin point
		clipCullUnit.ClipCullNegativeZAxis( 0.0f );
		for ( i = 0; i < clipCullUnit.GetPolygonSize(); i++ ) {
			ApplyPerspectiveDivide( clipCullUnit.GetPolygonVertex( i ) );
		}
		clipCullUnit.ClipCullPositiveZAxis( 1.0f );
		clipCullUnit.ClipCullNegativeXAxis( -1.0f );
		clipCullUnit.ClipCullPositiveXAxis( 1.0f );
		clipCullUnit.ClipCullNegativeYAxis( -1.0f );
		clipCullUnit.ClipCullPositiveYAxis( 1.0f );

		GSOut<CtxType> newV0, newV1, newV2;
		for ( i = 0; i < clipCullUnit.GetTriangleCount(); i++ ) {
			// perform backface culling
			if ( !clipCullUnit.IsCurrentTriangleBackfaced() ) {
				clipCullUnit.GetCurrentTriangle( newV0, newV1, newV2 );

				RasterizationStage( ctx, newV0, newV1, newV2 );
			}
			clipCullUnit.NextTriangle();
		}
	}

	template<typename CtxType>
	void RasterizationStage( CtxType &ctx, GSOut<CtxType> &v0, GSOut<CtxType> &v1, GSOut<CtxType> &v2 ) {
		// perform screen transformation (normalized coordinates -> pixels in screen)
		const Viewport &vp = ctx.GetViewport();
		vp.Transform( v0.position );
		vp.Transform( v1.position );
		vp.Transform( v2.position );
		
		DrawTriangle( ctx, v0, v1, v2 );
		// pixel shader is executed inside DrawTriangle
		
		// Draw a white wireframe if you want
		//DrawLine( v0.position.x(), v0.position.y(), v1.position.x(), v1.position.y(), Colors::White );
		//DrawLine( v1.position.x(), v1.position.y(), v2.position.x(), v2.position.y(), Colors::White );
		//DrawLine( v2.position.x(), v2.position.y(), v0.position.x(), v0.position.y(), Colors::White );
	}

	template<typename CtxType>
	void PixelShaderStage( CtxType &ctx, Color *pBufferPosition, GSOut<CtxType> vtx ) {
		PS<CtxType> &pixelShader = ctx.GetPixelShaderData();
		// perspective correction:
		// revert perspective divide to avoid texture warping
		float zInverse = 1.0f / vtx.position.w();
		vtx *= zInverse;
		OutputMergerStage( ctx, pixelShader( vtx ), pBufferPosition );
	}

	template<typename CtxType>
	void OutputMergerStage( CtxType &ctx, const Color &newColor, Color *pOldColorInBuffer ) {
		// zbuffer test should be here instead of being in the rasterization stage
		*pOldColorInBuffer = newColor;
	}

	template<typename VType>
	void ApplyPerspectiveDivide( VType &vtx ) {
		// apply the perspective divide and store z value inverted in w component so we
		// can get homo-space coordinates again dividing by w
		float zInverse = 1.0f / vtx.position.w();
		vtx *= zInverse;
		vtx.position.w() = zInverse;
		// now z is mapped to [0, 1] according to zNear and zFar respectively
	}

	template<typename CtxType>
	void DrawTriangle( CtxType &ctx, const GSOut<CtxType> &v0, const GSOut<CtxType> &v1, const GSOut<CtxType> &v2 ) {
		std::array<const GSOut<CtxType> *, 3> orderedVertices;
		const GSOut<CtxType> *pvEdge = &v0;
		const GSOut<CtxType> *pvLeft = &v1;
		const GSOut<CtxType> *pvRight = &v2;

		// order vertices from lowermost to uppermost (screen position)
		// assign v0 as the first one for now
		orderedVertices[ 0 ] = &v0;
		orderedVertices[ 1 ] = &v1;
		orderedVertices[ 2 ] = &v2;
		// check if v1 is the first
		if ( orderedVertices[ 0 ]->position.y() < v1.position.y() ) {
			orderedVertices[ 0 ] = &v1;
			orderedVertices[ 1 ] = &v0;
			orderedVertices[ 2 ] = &v2;
		}
		// check if v2 is the first
		if ( orderedVertices[ 0 ]->position.y() < v2.position.y() ) {
			orderedVertices[ 0 ] = &v2;
			orderedVertices[ 1 ] = &v0;
			orderedVertices[ 2 ] = &v1;
		}
		// check if last two are in order
		if ( orderedVertices[ 1 ]->position.y() < orderedVertices[ 2 ]->position.y() )
			std::swap( orderedVertices[ 1 ], orderedVertices[ 2 ] );

		// check if top only triangle case
		if ( orderedVertices[ 0 ]->position.y() == orderedVertices[ 1 ]->position.y() ) {
			pvEdge = orderedVertices[ 2 ];
			// assure pvLeft is on the left side and pvRight is on the right side
			if ( orderedVertices[ 0 ]->position.x() <= orderedVertices[ 1 ]->position.x() ) {
				pvLeft = orderedVertices[ 0 ];
				pvRight = orderedVertices[ 1 ];
			}
			else {
				pvLeft = orderedVertices[ 1 ];
				pvRight = orderedVertices[ 0 ];
			}

			DrawScanlineTriangle( ctx, *pvEdge, *pvLeft, *pvRight );
		}
		// check if bottom only triangle case
		else if ( orderedVertices[ 1 ]->position.y() == orderedVertices[ 2 ]->position.y() ) {
			pvEdge = orderedVertices[ 0 ];
			// assure pvLeft is on the left side and pvRight is on the right side
			if ( orderedVertices[ 1 ]->position.x() <= orderedVertices[ 2 ]->position.x() ) {
				pvLeft = orderedVertices[ 1 ];
				pvRight = orderedVertices[ 2 ];
			}
			else {
				pvLeft = orderedVertices[ 2 ];
				pvRight = orderedVertices[ 1 ];
			}

			DrawScanlineTriangle( ctx, *pvEdge, *pvLeft, *pvRight );
		}
		else {
			// create another vertex to split the triangle and interpolate its position
			// along the opposite side
			GSOut<CtxType> newVertex = *orderedVertices[ 0 ];
			float p =
				( orderedVertices[ 0 ]->position.y() - orderedVertices[ 1 ]->position.y() )
				/
				( orderedVertices[ 0 ]->position.y() - orderedVertices[ 2 ]->position.y() )
				;
			newVertex = VertexLerp( newVertex, *orderedVertices[ 2 ], p );

			// assure pvLeft is on the left side and pvRight is on the right side
			if ( orderedVertices[ 1 ]->position.x() <= newVertex.position.x() ) {
				pvLeft = orderedVertices[ 1 ];
				pvRight = &newVertex;
			}
			else {
				pvLeft = &newVertex;
				pvRight = orderedVertices[ 1 ];
			}

			pvEdge = orderedVertices[ 0 ];
			// bottom triangle
			DrawScanlineTriangle( ctx, *pvEdge, *pvLeft, *pvRight );
			pvEdge = orderedVertices[ 2 ];
			// top triangle
			DrawScanlineTriangle( ctx, *pvEdge, *pvLeft, *pvRight );
		}
	}

	template<typename CtxType>
	void DrawScanlineTriangle( CtxType &ctx, const GSOut<CtxType> &vEdge, const GSOut<CtxType> &vLeft, const GSOut<CtxType> &vRight ) {
		GSOut<CtxType> scanlineStartPos, scanlineEndPos, currentPos;
		GSOut<CtxType> horizontalIncrement, leftIncrement, rightIncrement;
		int currentScanline, totalScanlines, nextRowOffset = rt.GetWidth();
		Color *pBuffer = reinterpret_cast<Color *>( rt.GetBuffer() );
		Color *pCurrentPixel = pBuffer, *pScanlineEnd = pBuffer, *pStartingPixel;
		float roundToPixelFactor;

		if ( vEdge.position.y() > vLeft.position.y() ) {
			// top to bottom triangle case
			pStartingPixel = &pBuffer[ size_t( nextRowOffset ) * size_t( ceilf( vLeft.position.y() - 0.5f ) ) ];	// top part of the top-left rule
			totalScanlines = size_t( ceilf( vEdge.position.y() - 1.5f ) - ceilf( vLeft.position.y() - 0.5f ) ) + 1;
			roundToPixelFactor = ceilf( vLeft.position.y() - 0.5f ) + 0.5f - vLeft.position.y();
		}
		else {
			// bottom to top triangle case
			pStartingPixel = &pBuffer[ size_t( nextRowOffset ) * size_t( ceilf( vLeft.position.y() - 1.5f ) ) ];
			totalScanlines = size_t( ceilf( vLeft.position.y() - 1.5f ) - roundf( vEdge.position.y() ) ) + 1;
			roundToPixelFactor = vLeft.position.y() - ceilf( vLeft.position.y() - 1.5f ) - 0.5f;
			// change triangle draw direction
			nextRowOffset = -nextRowOffset;
		}

		// calculate linear increments for left and right sides of the triangle
		{
			// calculate how much draw percentage represents a 1 pixel increment in the y axis
			float verticalPixelIncrement = 1.0f / abs( vEdge.position.y() - vLeft.position.y() );
			
			leftIncrement = GetVertexIncrement( vLeft, vEdge, verticalPixelIncrement );
			rightIncrement = GetVertexIncrement( vRight, vEdge, verticalPixelIncrement );
		}

		// calculate horizontal increments for the triangle
		{
			float horizontalPixelIncrement = 1.0f / ( vRight.position.x() - vLeft.position.x() );
			horizontalIncrement = GetVertexIncrement( vLeft, vRight, horizontalPixelIncrement );
		}

		// align scanline starting and ending positions with pixel positions defined
		// by the top-left rule
		scanlineStartPos = leftIncrement;
		scanlineStartPos *= roundToPixelFactor;
		scanlineStartPos += vLeft;
		scanlineEndPos = rightIncrement;
		scanlineEndPos *= roundToPixelFactor;
		scanlineEndPos += vRight;

		// raster loop
		for ( currentScanline = 0; currentScanline < totalScanlines; currentScanline++ ) {
			currentPos = scanlineStartPos;
			// position buffer to the start of the current row in the buffer
			pCurrentPixel = &pStartingPixel[ currentScanline * nextRowOffset ];
			// compute buffer end position in scanline
			// -1.49f and not -1.5f to make slightly larger scanline to counter some float
			// imprecisions due to too many interpolations
			pScanlineEnd = pCurrentPixel + size_t( ceilf( scanlineEndPos.position.x() - 1.49f ) );
			// compute buffer start position in scanline
			pCurrentPixel += size_t( ceilf( scanlineStartPos.position.x() - 0.5f ) );	// left part of the top-left rule

			// draw single scanline loop
			for ( ; pCurrentPixel <= pScanlineEnd; pCurrentPixel++ ) {
				// zbuffer test
				// this should be tested at output merger stage, but texture interpolation is
				// expensive for the cpu so there's no need to do that if we know the pixel
				// is going to be occluded
				if ( ctx.zbuffer.TestAndSet( pCurrentPixel - pBuffer, currentPos.position.z() ) )
					PixelShaderStage( ctx, pCurrentPixel, currentPos );
				// keep track of the current position in viewport
				currentPos += horizontalIncrement;
			}

			// calculate next scanline
			scanlineStartPos += leftIncrement;
			scanlineEndPos += rightIncrement;
		}
	}

	// applies the formula ( v1 - v0 ) * incrementFactor
	template<typename VType>
	VType GetVertexIncrement( const VType &v0, const VType &v1, float incrementFactor ) {
		// we do it like this so we don't have to define a lot of operators for 
		// every vertex declaration
		VType vOut;
		vOut = v0;
		vOut *= -1;
		vOut += v1;
		vOut *= incrementFactor;
		return vOut;
	}
};
