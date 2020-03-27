#pragma once
#include "RenderTarget.h"

class Graphics {

public:

	Graphics( unsigned int width, unsigned int height );
	Graphics( const Graphics & ) = delete;
	Graphics &operator=( const Graphics & ) = delete;
	~Graphics();

	void SetWindowToDraw( HWND hWnd );

	void ClearBackground();
	void ClearBackground( unsigned char r, unsigned char g, unsigned char b );
	void PutPixel( unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b );
	void DrawLine( int x0, int y0, int x1, int y1,
		unsigned char r, unsigned char g, unsigned char b );

	void EndFrame() const;

private:

	HWND hWnd = 0;
	HDC dc = 0;
	RenderTarget rt;
	char *currentRawRT = nullptr;

};