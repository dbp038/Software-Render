#include "pch.h"
#include "Bitmap.h"

Bitmap::Bitmap( unsigned int width, unsigned int height )
	: Bitmap( width, height, 4u ) {}

Bitmap::Bitmap( unsigned int width, unsigned int height, unsigned int pixelSizeInBytes )
	: width( width ), height( height ), pixelSizeInBytes( pixelSizeInBytes ) {
	totalSizeInBytes = size_t( width ) * height * pixelSizeInBytes;
	pImageBuffer = std::make_unique<unsigned char[]>( totalSizeInBytes );
	memset( pImageBuffer.get(), 0, totalSizeInBytes );
}

unsigned char *Bitmap::GetBuffer() {
	return pImageBuffer.get();
}

const unsigned char *Bitmap::GetBuffer() const {
	return pImageBuffer.get();
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