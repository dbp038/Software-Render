#include "platform.h"
#include "TextureBitmap.h"
#include "app/resources/FileLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#pragma warning(push, 0)
#include "app/resources/stb_image.h"
#pragma warning(pop)

TextureBitmap::TextureBitmap( const std::string &fileName ) {
	SetTexture( fileName );
}

TextureBitmap::~TextureBitmap() {
	stbi_image_free( pImageBuffer );
}

void TextureBitmap::SetTexture( const std::string &fileName ) {
	int tWidth, tHeight, tComponents;
	pImageBuffer = LoadPNG( fileName, &tWidth, &tHeight, &tComponents );
	width = tWidth;
	height = tHeight;
	pixelSizeInBytes = tComponents;
	totalSizeInBytes = size_t( width ) * height * pixelSizeInBytes;
}

unsigned char *TextureBitmap::GetBuffer() {
	return pImageBuffer;
}

const unsigned char *TextureBitmap::GetBuffer() const {
	return pImageBuffer;
}

unsigned int TextureBitmap::GetWidth() const {
	return width;
}

unsigned int TextureBitmap::GetHeight() const {
	return height;
}

unsigned int TextureBitmap::GetPixelSizeInBytes() const {
	return pixelSizeInBytes;
}

size_t TextureBitmap::GetTotalSizeInBytes() const {
	return totalSizeInBytes;
}

unsigned char *TextureBitmap::LoadPNG( const std::string &filename, int *outWidth, int *outHeight, int *outComponents ) {
	if ( !FileLoader::FileExists( filename ) ) {
		std::string msg = filename + " file could not be found.";
		throw std::exception( msg.c_str() );
	}
	stbi_set_flip_vertically_on_load( true );
	unsigned char *pOut = stbi_load( filename.c_str(), outWidth, outHeight, outComponents, 4 );

	// process image from RGBA to BGRA
	unsigned int *pImage = reinterpret_cast<unsigned int *>( pOut );
	unsigned int *pImageEnd = pImage + size_t( *outWidth ) * ( *outHeight );
	unsigned int r = 0u, b = 0u;
	for ( ; pImage < pImageEnd; pImage++ ) {
		// swap red and blue channels
		r = ( *pImage & 0xFFu ) << 16u;
		b = ( *pImage & 0xFF0000u ) >> 16u;
		*pImage = ( *pImage & 0xFF00FF00 ) | r | b;
	}

	return pOut;
}

