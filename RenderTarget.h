#pragma once
#include "Bitmap.h"

class RenderTarget : public Bitmap {

public:

	RenderTarget( unsigned int width, unsigned int height );
	RenderTarget( unsigned int width, unsigned int height, unsigned int pixelSizeInBytes );

	bool SendBitsToDevice( HDC dc ) const;

private:

	BITMAPINFO bmInfo = {};

	void SetupInfoDescriptor();

};