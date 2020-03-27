#pragma once

class Bitmap {

public:

	Bitmap( unsigned int width, unsigned int height );
	Bitmap( unsigned int width, unsigned int height, unsigned int pixelSizeInBytes );

	char *GetBuffer();
	const char *GetBuffer() const;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	unsigned int GetPixelSizeInBytes() const;
	size_t GetTotalSizeInBytes() const;

protected:

	std::vector<char> imageBuffer;
	unsigned int width, height;
	unsigned int pixelSizeInBytes;
	size_t totalSizeInBytes;

};