#pragma once
#include "RenderTarget.h"
#include "gfx/context/Viewport.h"
#include "Color.h"

class RenderPipeline {

	template<typename CtxType>
	using InputVertex = typename CtxType::InputVertex;
	template<typename CtxType>
	using VS = typename CtxType::VS;
	template<typename CtxType>
	using PS = typename CtxType::PS;
	template<typename CtxType>
	using VSOut = typename CtxType::VSOut;

public:

	RenderPipeline() = delete;
	RenderPipeline( RenderTarget &renderTarget );

	template<typename CtxType>
	void DrawCall( CtxType &ctx ) {

		VertexProcessingStage( ctx );
		// - Primitive Assembly stage ( I decide where the primitive assembly stage goes )
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
		vertices.reserve( indicesIn.size() );
		// execute vertex shader for each vertex
		for ( auto &vtx : verticesIn ) {
			vertices.push_back( vertexShader( vtx ) );
		}

		PrimitiveAssemblyStage( ctx, vertices, indicesIn );
	}

	// triangles only for now
	template<typename CtxType>
	void PrimitiveAssemblyStage( CtxType &ctx, std::vector<VSOut<CtxType>> &vertices, const std::vector<size_t> &indices ) {
		const Vector3f cameraView( 0.0f, 0.0f, 1.0f );
		for ( size_t i = 0; i < indices.size(); i += 3 ) {
			// assemble triangle
			VSOut<CtxType> &v0 = vertices[ indices[ i ] ];
			VSOut<CtxType> &v1 = vertices[ indices[ i + 1 ] ];
			VSOut<CtxType> &v2 = vertices[ indices[ i + 2 ] ];

			VertexPostProcessStage( ctx, v0, v1, v2 );
		}
	}


	template<typename CtxType>
	void VertexPostProcessStage( CtxType &ctx, const VSOut<CtxType> &v0, const VSOut<CtxType> &v1, const VSOut<CtxType> &v2 ) {
		// Geometry based clipping and culling
		int i;
		auto &clipCullUnit = ctx.GetClippingCullingUnit();
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

		VSOut<CtxType> newV0, newV1, newV2;
		const Viewport &vp = ctx.GetViewport();
		for ( i = 0; i < clipCullUnit.GetTriangleCount(); i++ ) {
			// perform backface culling
			if ( !clipCullUnit.IsCurrentTriangleBackfaced() ) {
				clipCullUnit.GetCurrentTriangle( newV0, newV1, newV2 );

				// perform screen transformation (normalized coordinates -> pixels in screen)
				vp.Transform( newV0.position );
				vp.Transform( newV1.position );
				vp.Transform( newV2.position );

				RasterizationStage( ctx, newV0, newV1, newV2 );
			}
			clipCullUnit.NextTriangle();
		}
	}

	template<typename CtxType>
	void PrimitiveCullingStage( CtxType &ctx, const VSOut<CtxType> &v0, const VSOut<CtxType> &v1, const VSOut<CtxType> &v2 ) {
		DrawTriangle( ctx, v0, v1, v2 );
		// pixel shader is executed inside DrawTriangle
	}

	template<typename CtxType>
	void RasterizationStage( CtxType &ctx, const VSOut<CtxType> &v0, const VSOut<CtxType> &v1, const VSOut<CtxType> &v2 ) {
		DrawTriangle( ctx, v0, v1, v2 );
		// pixel shader is executed inside DrawTriangle
	}

	template<typename CtxType>
	void PixelShaderStage( CtxType &ctx, Color *pBufferPosition, VSOut<CtxType> vtx ) {
		PS<CtxType> &pixelShader = ctx.GetPixelShaderData();
		// perspective correction:
		// revert perspective divide to avoid texture warping
		float zInverse = 1.0f / vtx.position.w();
		vtx *= zInverse;
		OutputMergerStage( ctx, pixelShader( vtx ), pBufferPosition );
	}

	template<typename CtxType>
	void OutputMergerStage( CtxType &ctx, const Color &newColor, Color *pOldColorInBuffer ) {
		// TODO: implment z-buffer test
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
	void DrawTriangle( CtxType &ctx, const VSOut<CtxType> &v0, const VSOut<CtxType> &v1, const VSOut<CtxType> &v2 ) {
		std::array<const VSOut<CtxType> *, 3> orderedVertices;
		const VSOut<CtxType> *pvEdge = &v0;
		const VSOut<CtxType> *pvLeft = &v1;
		const VSOut<CtxType> *pvRight = &v2;

		// order from vertices lowermost to uppermost (screen position)
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
			VSOut<CtxType> newVertex = *orderedVertices[ 0 ];
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
	void DrawScanlineTriangle( CtxType &ctx, const VSOut<CtxType> &vEdge, const VSOut<CtxType> &vLeft, const VSOut<CtxType> &vRight ) {
		VSOut<CtxType> scanlineStartPos, scanlineEndPos, currentPos, horizontalIncr, leftIncr, rightIncr;
		float deltaDrawPercentage, yStart, yLimit;
		int rightLimit, width = rt.GetWidth(), horizontalOffset;
		Color *pBuffer = reinterpret_cast<Color *>( rt.GetBuffer() );
		Color *pCurrentPixel = pBuffer, *pEndPixel = pBuffer;

		// check the direction of the raster
		if ( vEdge.position.y() > vLeft.position.y() )
			yStart = roundf( vLeft.position.y() );	// top part of the top-left rule
		else
			yStart = ceilf( vLeft.position.y() - 0.5f );
		// initialize positions and thresholds
		yLimit = ceilf( vEdge.position.y() - 0.5f );
		scanlineStartPos = vLeft;
		scanlineEndPos = vRight;
		currentPos = scanlineStartPos;

		// avoid completely flat horizontal triangle case
		if ( vEdge.position.y() != vLeft.position.y() ) {
			// calculate how much draw percentage represents a 1 pixel increment in the y axis
			deltaDrawPercentage = 1.0f / abs( ( yLimit - yStart ) );
			// calculate linear increments for left and right sides of the triangle
			{
				// formula:
				//	leftIncr = ( vEdge - vLeft ) * deltaDrawPercentage;
				leftIncr = vLeft;
				leftIncr *= -1;
				leftIncr += vEdge;
				leftIncr *= deltaDrawPercentage;
				// formula:
				//	rightIncr = ( vEdge - vRight ) * deltaDrawPercentage;
				rightIncr = vRight;
				rightIncr *= -1;
				rightIncr += vEdge;
				rightIncr *= deltaDrawPercentage;
				// we do it like this so we don't have to define a lot of operators for 
				// every vertex declaration, sorry if it's not easy to understand
			}
			// compute scanline start and end pixel positions
			horizontalOffset = static_cast<int>( roundf( scanlineStartPos.position.x() ) ); // left part of the top-left rule
			rightLimit = static_cast<int>( ceilf( scanlineEndPos.position.x() - 0.5f ) );
			// calculate how much represents a 1 pixel increment in viewport space 
			{
				// formula:
				//	horizontalIncr = ( scanlineEndPos - scanlineStartPos ) * ( 1.0f / ( rightLimit - horizontalOffset ) );
				horizontalIncr = scanlineStartPos;
				horizontalIncr *= -1;
				horizontalIncr += scanlineEndPos;
				horizontalIncr *= 1.0f / ( rightLimit - horizontalOffset );
				// we do it like this so we don't have to define a lot of operators for 
				// every vertex declaration, sorry if it's not easy to understand
			}

			// raster loop
			for ( float drawPercentage = 0.0f; drawPercentage <= 1.0f; drawPercentage += deltaDrawPercentage ) {
				// position buffer to the start of the next line in the buffer
				if ( vEdge.position.y() > vLeft.position.y() )
					pCurrentPixel = &pBuffer[ static_cast<int>( roundf( scanlineStartPos.position.y() ) ) *width ];
				else
					pCurrentPixel = &pBuffer[ static_cast<int>( ceilf( scanlineStartPos.position.y() - 0.5f ) ) *width ];
				// buffer end position in scanline
				pEndPixel = pCurrentPixel + rightLimit;
				// buffer start position in scanline
				pCurrentPixel += horizontalOffset;

				// avoid 0 lenght scanline rare case
				if ( pCurrentPixel != pEndPixel ) {
					// draw single scanline loop
					for ( ; pCurrentPixel <= pEndPixel; pCurrentPixel++ ) {
						PixelShaderStage( ctx, pCurrentPixel, currentPos );
						// keep track of the current position in viewport
						currentPos += horizontalIncr;
					}
				}

				// calculate next scanline
				scanlineStartPos += leftIncr;
				scanlineEndPos += rightIncr;
				currentPos = scanlineStartPos;
				// compute next scanline start and end pixel positions
				horizontalOffset = static_cast<int>( roundf( scanlineStartPos.position.x() ) ); // left part of the top-left rule
				rightLimit = static_cast<int>( ceilf( scanlineEndPos.position.x() - 0.5f ) );
			}
		}
	}

};