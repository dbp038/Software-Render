#include "pch.h"
#include "TextureSampler.h"

void ITextureSampler::SetUVMode( UVMode mode ) {
	textureMode = mode;
}

void ITextureSampler::ApplyTextureMode( float &x, float &y ) const {
	switch ( textureMode ) {
	case UVMode::CLAMP:
		x = std::clamp( x, 0.0f, 1.0f );
		y = std::clamp( y, 0.0f, 1.0f );
		break;
	case UVMode::WRAP:
		x = fmodf( x, 1.0f );
		y = fmodf( y, 1.0f );
		if ( x < 0.0f )
			x = x + 1.0f;
		if ( y < 0.0f )
			y = y + 1.0f;
		break;
	default:
		throw std::exception( "Wrong texture sampler mode set." );
		break;
	}
}

Color TexturePointSampler::Sample( const TextureBitmap &texture, float x, float y ) const {
	size_t bufferWidth = texture.GetWidth();
	unsigned int width = texture.GetWidth() - 2, height = texture.GetHeight() - 2;
	const Color *textureBuffer = reinterpret_cast<const Color *>( texture.GetBuffer() );

	ApplyTextureMode( x, y );

	x = x * width + 1;
	y = y * height + 1;

	return textureBuffer[ bufferWidth * unsigned( y ) + unsigned( x ) ];
}

Color TextureLinearSampler::Sample( const TextureBitmap &texture, float x, float y ) const {
	size_t bufferWidth = texture.GetWidth(), pixelOffset;
	unsigned int width = texture.GetWidth() - 2, height = texture.GetHeight() - 2, upperX, upperY, lowerX, lowerY;
	Vector4f lowerColor, upperColor, interp1, interp2;
	const Color *textureBuffer = reinterpret_cast<const Color *>( texture.GetBuffer() );

	ApplyTextureMode( x, y );

	// bilinear interpolation
	x = x * width + 1;
	y = y * height + 1;
	lowerX = unsigned int( floorf( x ) );
	lowerY = unsigned int( floorf( y ) );
	upperX = unsigned int( ceilf( x ) );
	upperY = unsigned int( ceilf( y ) );
	pixelOffset = size_t( bufferWidth ) * lowerY;
	lowerColor = textureBuffer[ pixelOffset + lowerX ].ToVector4f();
	upperColor = textureBuffer[ pixelOffset + upperX ].ToVector4f();
	interp1 = lerp( lowerColor, upperColor, x - lowerX );
	pixelOffset = size_t( bufferWidth ) * upperY;
	lowerColor = textureBuffer[ pixelOffset + lowerX ].ToVector4f();
	upperColor = textureBuffer[ pixelOffset + upperX ].ToVector4f();
	interp2 = lerp( lowerColor, upperColor, x - lowerX );

	return Color( lerp( interp1, interp2, y - lowerY ) );
}
