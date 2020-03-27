#include "pch.h"
#include "Bitmap.h"

Bitmap::Bitmap( unsigned int width, unsigned int height )
	: Bitmap( width, height, 4u ) {}

Bitmap::Bitmap( unsigned int width, unsigned int height, unsigned int pixelSizeInBytes )
	: width( width ), height( height ), pixelSizeInBytes( pixelSizeInBytes ) {
	totalSizeInBytes = size_t( width ) * height * pixelSizeInBytes;
	imageBuffer.resize( totalSizeInBytes );
	memset( imageBuffer.data(), 0, totalSizeInBytes );
}

char *Bitmap::GetBuffer() {
	return imageBuffer.data();
}

const char *Bitmap::GetBuffer() const {
	return imageBuffer.data();
}

unsigned int Bitmap::GetWidth() const {
	return width;
}

unsigned int Bitmap::GetHeight() const {
	return height;
}

unsigned int Bitmap::GetPixelSizeInBytes() const {
	return pixelSizeInBytes;
}

size_t Bitmap::GetTotalSizeInBytes() const {
	return totalSizeInBytes;
}
