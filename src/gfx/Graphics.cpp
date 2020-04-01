#include "pch.h"
#include "Graphics.h"

Graphics::Graphics( unsigned int width, unsigned int height )
	: rt( width, height, sizeof( Color ) ), pipeline( rt ) {
	currentRawRT = reinterpret_cast<Color *>( rt.GetBuffer() );
}

Graphics::~Graphics() {
	ReleaseDC( hWnd, dc );
}

void Graphics::SetWindowToDraw( HWND hWnd ) {
	this->hWnd = hWnd;
	// we can keep the device context because of the CS_OWNDC window style
	// thanks to this we don't have to get it and release it every frame
	dc = GetDC( hWnd );
}

int Graphics::GetWidth() const {
	return rt.GetWidth();
}

int Graphics::GetHeight() const {
	return rt.GetHeight();
}

Vector2i Graphics::GetScreenSize() const {
	return Vector2i( GetWidth(), GetHeight() );
}

void Graphics::BindContext( IRenderContext &ctx ) {
	pCurrentCtx = &ctx;
}

void Graphics::ClearBackground() {
	memset( currentRawRT, 0, rt.GetTotalSizeInBytes() );
}

void Graphics::ClearBackground( Color color ) {
	Color *buffer = currentRawRT;
	size_t totalElements = size_t( rt.GetWidth() ) * rt.GetHeight();
	void *bufferEnd = buffer + totalElements;
	for ( ; buffer < bufferEnd; buffer++ ) {
		buffer[ 0 ] = color;
	}
}

void Graphics::Draw() {
	if ( pCurrentCtx )
		pCurrentCtx->Draw( pipeline );
}

void Graphics::PutPixel( int x, int y, Color color ) {
	assert( x >= 0 && x < rt.GetWidth() );
	assert( y >= 0 && y < rt.GetHeight() );
	size_t offset = ( size_t( y ) * rt.GetWidth() + x );
	currentRawRT[ offset ] = color;
}

void Graphics::DrawLine( int x0, int y0, int x1, int y1, Color color ) {
	// basic Bresenham implementation
	// get delta distance from source point to destination point
	int dY = y1 - y0;
	int dX = x1 - x0;

	// compute slanted increments
	int slantedYIncr, slantedXIncr, straightYIncr, straightXIncr;
	if ( dY > 0 )
		slantedYIncr = 1;
	else if ( dY == 0 )
		slantedYIncr = 0;
	else {
		dY = -dY;
		slantedYIncr = -1;
	}
	if ( dX > 0 )
		slantedXIncr = 1;
	else if ( dX == 0 )
		slantedXIncr = 0;
	else {
		dX = -dX;
		slantedXIncr = -1;
	}
	// compute straight increments
	if ( dX >= dY ) {
		straightYIncr = 0;
		straightXIncr = slantedXIncr;
	}
	else {
		straightXIncr = 0;
		straightYIncr = slantedYIncr;
		std::swap( dX, dY );
	}

	// raster line loop
	int x = x0, y = y0;
	int straightAdv = 2 * dY;
	int adv = straightAdv - dX;
	int slantedAdv = adv - dX;
	do {
		PutPixel( x, y, color );
		// we want to paint at least one pixel but we don't want to increment x in that case
		if ( adv >= 0 ) {
			// slanted advancement
			x += slantedXIncr;
			y += slantedYIncr;
			adv += slantedAdv;
		}
		else {
			// straight advancement
			x += straightXIncr;
			y += straightYIncr;
			adv += straightAdv;
		}
	} while ( x != x1 || y != y1 );
}

void Graphics::DrawTriangle( const Vector4f &v0, const Vector4f &v1, const Vector4f &v2, Color color ) {
	std::vector<const Vector4f *> orderedVertices;
	const Vector4f *pvEdge = &v0;
	const Vector4f *pvLeft = &v1;
	const Vector4f *pvRight = &v2;

	orderedVertices.resize( 3 );
	// order from vertices lowermost to uppermost (screen position)
	// assign v0 as the first one for now
	orderedVertices[ 0 ] = &v0;
	orderedVertices[ 1 ] = &v1;
	orderedVertices[ 2 ] = &v2;
	// check if v1 is the first
	if ( orderedVertices[ 0 ]->y() < v1.y() ) {
		orderedVertices[ 0 ] = &v1;
		orderedVertices[ 1 ] = &v0;
		orderedVertices[ 2 ] = &v2;
	}
	// check if v2 is the first
	if ( orderedVertices[ 0 ]->y() < v2.y() ) {
		orderedVertices[ 0 ] = &v2;
		orderedVertices[ 1 ] = &v0;
		orderedVertices[ 2 ] = &v1;
	}
	// check if last two are in order
	if ( orderedVertices[ 1 ]->y() < orderedVertices[ 2 ]->y() )
		std::swap( orderedVertices[ 1 ], orderedVertices[ 2 ] );

	// check if top only triangle case
	if ( orderedVertices[ 0 ]->y() == orderedVertices[ 1 ]->y() ) {
		pvEdge = orderedVertices[ 2 ];
		// assure pvLeft is on the left side and pvRight is on the right side
		if ( orderedVertices[ 0 ]->x() <= orderedVertices[ 1 ]->x() ) {
			pvLeft = orderedVertices[ 0 ];
			pvRight = orderedVertices[ 1 ];
		}
		else {
			pvLeft = orderedVertices[ 1 ];
			pvRight = orderedVertices[ 0 ];
		}

		DrawScanlineTriangle( *pvEdge, *pvLeft, *pvRight, color );
	}
	// check if bottom only triangle case
	else if ( orderedVertices[ 1 ]->y() == orderedVertices[ 2 ]->y() ) {
		pvEdge = orderedVertices[ 0 ];
		// assure pvLeft is on the left side and pvRight is on the right side
		if ( orderedVertices[ 1 ]->x() <= orderedVertices[ 2 ]->x() ) {
			pvLeft = orderedVertices[ 1 ];
			pvRight = orderedVertices[ 2 ];
		}
		else {
			pvLeft = orderedVertices[ 2 ];
			pvRight = orderedVertices[ 1 ];
		}

		DrawScanlineTriangle( *pvEdge, *pvLeft, *pvRight, color );
	}
	else {
		// create another vertex to split the triangle and interpolate its position
		// along the opposite side
		Vector4f newVertex = *orderedVertices[ 0 ];
		float p = ( orderedVertices[ 0 ]->y() - orderedVertices[ 1 ]->y() ) / ( orderedVertices[ 0 ]->y() - orderedVertices[ 2 ]->y() );
		newVertex = lerp( newVertex, *orderedVertices[ 2 ], p );

		// assure pvLeft is on the left side and pvRight is on the right side
		if ( orderedVertices[ 1 ]->x() <= newVertex.x() ) {
			pvLeft = orderedVertices[ 1 ];
			pvRight = &newVertex;
		}
		else {
			pvLeft = &newVertex;
			pvRight = orderedVertices[ 1 ];
		}

		pvEdge = orderedVertices[ 0 ];
		// bottom triangle
		DrawScanlineTriangle( *pvEdge, *pvLeft, *pvRight, color );
		pvEdge = orderedVertices[ 2 ];
		// top triangle
		DrawScanlineTriangle( *pvEdge, *pvLeft, *pvRight, color );
	}
}

void Graphics::EndFrame() const {
	rt.SendBitsToDevice( dc );
}

void Graphics::DrawScanlineTriangle( const Vector4f &vEdge, const Vector4f &vLeft, const Vector4f &vRight, Color color ) {
	float deltaDrawPercentage, yStart, yLimit;
	Vector4f scanlineStartPos, scanlineEndPos, currentPos, horizontalIncr, leftIncr, rightIncr;
	int rightLimit, width = GetWidth(), horizontalOffset;
	Color *pBuffer = currentRawRT, *pBufferEnd = currentRawRT;

	// check the direction of the raster
	if ( vEdge.y() > vLeft.y() )
		yStart = ceilf( vLeft.y() - 0.5f );	// top part of the top-left rule
	else
		yStart = roundf( vLeft.y() );
	// initialize positions and thresholds
	yLimit = roundf( vEdge.y() );
	scanlineStartPos = vLeft;
	scanlineEndPos = vRight;
	currentPos = scanlineStartPos;

	// avoid completely flat horizontal triangle case
	if ( vEdge.y() != vLeft.y() ) {
		// calculate how much draw percentage represents a 1 pixel increment in the y axis
		deltaDrawPercentage = 1.0f / abs( ( yLimit - yStart ) );
		// these values represent the linear increments for left and right sides of the triangle
		leftIncr = ( vEdge - vLeft ) * deltaDrawPercentage;
		rightIncr = ( vEdge - vRight ) * deltaDrawPercentage;

		// raster loop
		for ( float drawPercentage = 0.0f; drawPercentage <= 1.0f; drawPercentage += deltaDrawPercentage ) {
			// position buffer to the start of the next line in the buffer
			pBuffer = &currentRawRT[ static_cast<int>( roundf( scanlineStartPos.y() ) ) *width ];

			// compute scanline start and end pixel positions
			horizontalOffset = static_cast<int>( roundf( scanlineStartPos.x() - 1.0f ) ); // left part of the top-left rule
			rightLimit = static_cast<int>( ceilf( scanlineEndPos.x() ) );
			// buffer end position in scanline
			pBufferEnd = pBuffer + rightLimit;
			// buffer start position in scanline
			pBuffer += horizontalOffset;

			// avoid 0 lenght scanline rare case
			if ( pBuffer != pBufferEnd ) {
				// calculate how much represents a 1 pixel increment in viewport space (for now obviously 1 pixel)
				horizontalIncr = ( scanlineEndPos - scanlineStartPos ) * ( 1.0f / ( rightLimit - horizontalOffset ) );
				// draw single scanline loop
				for ( ; pBuffer <= pBufferEnd; pBuffer++ ) {
					*pBuffer = color;	// put pixel
					// keep track of the current position in viewport
					currentPos += horizontalIncr;
				}
			}

			// calculate next scanline
			scanlineStartPos += leftIncr;
			scanlineEndPos += rightIncr;
			currentPos = scanlineStartPos;
		}
	}
}
