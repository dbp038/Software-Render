#include "pch.h"
#include "Color.h"
#include "Graphics.h"

Graphics::Graphics( unsigned int width, unsigned int height )
	: rt( width, height, sizeof( Color ) ), pipeline( rt ) {}

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
	memset( rt.GetBuffer(), 0, rt.GetTotalSizeInBytes() );
}

void Graphics::ClearBackground( const Color &color ) {
	Color *buffer = reinterpret_cast<Color *>( rt.GetBuffer() );
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

void Graphics::EndFrame() const {
	rt.SendBitsToDevice( dc );
}
