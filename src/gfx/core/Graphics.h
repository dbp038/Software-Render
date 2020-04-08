#pragma once
#include "RenderTarget.h"
#include "gfx/context/RenderContext.h"

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

	void BindContext( IRenderContext &ctx );

	void ClearBackground();
	void ClearBackground( const Color &color );
	void Draw();

	void EndFrame() const;

private:

	HWND hWnd = 0;
	HDC dc = 0;
	RenderTarget rt;

	RenderPipeline pipeline;
	IRenderContext *pCurrentCtx = nullptr;

};