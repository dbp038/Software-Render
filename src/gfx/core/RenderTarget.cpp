#include "platform.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget( unsigned int width, unsigned int height )
	: Bitmap( width, height ) 
{
	SetupInfoDescriptor();
}

RenderTarget::RenderTarget( unsigned int width, unsigned int height, unsigned int pixelSizeInBytes )
	: Bitmap( width, height, pixelSizeInBytes ) 
{
	SetupInfoDescriptor();
}

// Copies bitmap raw data to the background of the window with that device context
bool RenderTarget::SendBitsToDevice( HDC dc ) const {
	return SetDIBitsToDevice( dc, 0, 0, width, height, 0, 0, 0, height,
		pImageBuffer.get(), &bmInfo, DIB_RGB_COLORS ) < 0;
}

void RenderTarget::SetupInfoDescriptor() {
	auto &h = bmInfo.bmiHeader;
	h.biSize = sizeof( h );
	h.biWidth = width;
	h.biHeight = -int( height );
	h.biPlanes = 1;
	h.biBitCount = pixelSizeInBytes * 8;
	h.biCompression = BI_RGB;
	h.biSizeImage = width * height;
}
