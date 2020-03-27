#include "pch.h"
#include "Graphics.h"

Graphics::Graphics( unsigned int width, unsigned int height )
	: rt( width, height ) {
	currentRawRT = rt.GetBuffer();
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

void Graphics::ClearBackground( unsigned char r, unsigned char g, unsigned char b ) {
	char *buffer = currentRawRT;
	void *bufferEnd = buffer + rt.GetTotalSizeInBytes();
	const unsigned int pixelSize = rt.GetPixelSizeInBytes();
	for ( ; buffer < bufferEnd; buffer += pixelSize ) {
		buffer[ 0 ] = r;
		buffer[ 1 ] = g;
		buffer[ 2 ] = b;
	}
}

void Graphics::PutPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b ) {
	size_t offset = ( size_t( y ) * rt.GetWidth() + x ) * rt.GetPixelSizeInBytes();
	currentRawRT[ offset ] = r;
	currentRawRT[ offset + 1 ] = g;
	currentRawRT[ offset + 2 ] = b;
}

void Graphics::DrawLine( int x0, int y0, int x1, int y1, unsigned char r, unsigned char g, unsigned char b ) {
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
		PutPixel( x, y, r, g, b );
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

void Graphics::EndFrame() const {
	rt.SendBitsToDevice( dc );
}
