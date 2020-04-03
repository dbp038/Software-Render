#pragma once

class IBitmap {

public:

	virtual unsigned char *GetBuffer() = 0;
	virtual const unsigned char *GetBuffer() const = 0;
	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;
	virtual unsigned int GetPixelSizeInBytes() const = 0;
	virtual size_t GetTotalSizeInBytes() const = 0;

};

class Bitmap : public IBitmap {

public:

	Bitmap( unsigned int width, unsigned int height );
	Bitmap( unsigned int width, unsigned int height, unsigned int pixelSizeInBytes );

	unsigned char *GetBuffer() override;
	const unsigned char *GetBuffer() const override;
	unsigned int GetWidth() const override;
	unsigned int GetHeight() const override;
	unsigned int GetPixelSizeInBytes() const override;
	size_t GetTotalSizeInBytes() const override;

protected:

	std::unique_ptr<unsigned char[]> pImageBuffer;
	unsigned int width, height;
	unsigned int pixelSizeInBytes;
	size_t totalSizeInBytes;

};