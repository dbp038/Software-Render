#include "pch.h"
#include "Graphics.h"

Graphics::Graphics( unsigned int width, unsigned int height )
	: rt( width, height, sizeof( Color ) ) {
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

void Graphics::PutPixel( int x, int y, Color color ) {
	assert( x >= 0 && x < rt.GetWidth() );
	assert( y >= 0 && y < rt.GetHeight() );
	size_t offset = ( size_t( y ) * rt.GetWidth() + x );
	currentRawRT[ offset ] = color;
}

void Graphics::PutPixel( Eigen::Vector2i pos, Color color ) {
	PutPixel( pos.x(), pos.y(), color );
}

void Graphics::DrawLine( int x0, int y0, int x1, int y1, Color color ) {
	// basic Bresenham implementation
	// get delta distance from source point to destination point
	int dY = y1 - y0;
	int dX = x1 - x0;

	// compute slanted increments
	int slantedYIncr, slantedXIncr, straightYIncr, straightXIncr;
	if ( dY >= 0 )
		slantedYIncr = 1;
	else {
		dY = -dY;
		slantedYIncr = -1;
	}
	if ( dX >= 0 )
		slantedXIncr = 1;
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
	while ( true ) {
		PutPixel( x, y, color );
		// we want to paint at least one pixel but we don't want to increment x in that case
		if ( x == x1 ) break;
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
	}
}

void Graphics::DrawLine( Eigen::Vector2i pos0, Eigen::Vector2i pos1, Color color ) {
	DrawLine( pos0.x(), pos0.y(), pos1.x(), pos1.y(), color );
}

void Graphics::EndFrame() const {
	rt.SendBitsToDevice( dc );
}
