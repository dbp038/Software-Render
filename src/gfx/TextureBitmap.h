#pragma once
#include "Bitmap.h"

class TextureBitmap : public IBitmap {

public:

	TextureBitmap() = default;
	TextureBitmap( const std::string &fileName );
	~TextureBitmap();

	void SetTexture( const std::string &fileName );

	unsigned char *GetBuffer() override;
	const unsigned char *GetBuffer() const override;
	unsigned int GetWidth() const override;
	unsigned int GetHeight() const override;
	unsigned int GetPixelSizeInBytes() const override;
	size_t GetTotalSizeInBytes() const override;

protected:

	unsigned char *pImageBuffer = nullptr;
	unsigned int width = 0, height = 0;
	unsigned int pixelSizeInBytes = 0;
	size_t totalSizeInBytes = 0;

	static unsigned char *LoadPNG( const std::string &filename, int *outWidth, int *outHeight, int *outComponents );

};