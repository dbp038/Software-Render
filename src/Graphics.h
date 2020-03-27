#pragma once
#include "RenderTarget.h"
#include "Color.h"
#include <Eigen/Core>

class Graphics {

public:

	Graphics( unsigned int width, unsigned int height );
	Graphics( const Graphics & ) = delete;
	Graphics &operator=( const Graphics & ) = delete;
	~Graphics();

	void SetWindowToDraw( HWND hWnd );

	void ClearBackground();
	void ClearBackground( Color color );
	void PutPixel( int x, int y, Color color );
	void PutPixel( Eigen::Vector2i pos , Color color );
	void DrawLine( int x0, int y0, int x1, int y1, Color color );
	void DrawLine( Eigen::Vector2i pos0, Eigen::Vector2i pos1, Color color );

	void EndFrame() const;

private:

	HWND hWnd = 0;
	HDC dc = 0;
	RenderTarget rt;
	Color *currentRawRT = nullptr;

};