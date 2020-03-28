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

	int GetWidth() const;
	int GetHeight() const;
	Vector2i GetScreenSize() const;

	void ClearBackground();
	void ClearBackground( Color color );
	void PutPixel( int x, int y, Color color );
	template<typename VectorType>
	void PutPixel( VectorType pos, Color color ) {
		PutPixel( pos.x(), pos.y(), color );
	}
	void DrawLine( int x0, int y0, int x1, int y1, Color color );
	template<typename VectorType>
	void DrawLine( VectorType pos0, VectorType pos1, Color color ) {
		DrawLine( pos0.x(), pos0.y(), pos1.x(), pos1.y(), color );
	}

	void EndFrame() const;

private:

	HWND hWnd = 0;
	HDC dc = 0;
	RenderTarget rt;
	Color *currentRawRT = nullptr;

};