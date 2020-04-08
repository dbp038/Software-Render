#pragma once
#include "gfx/core/Color.h"
#include "TextureBitmap.h"

class ITextureSampler {

public:

	enum class UVMode {
		WRAP, CLAMP
	};

	void SetUVMode( UVMode mode );

protected:

	UVMode textureMode = UVMode::CLAMP;

	void ApplyTextureMode( float &x, float &y ) const;

};

class TexturePointSampler : public ITextureSampler {

public:

	Color Sample( const TextureBitmap &texture, float x, float y ) const;

};

class TextureLinearSampler : public ITextureSampler {

public:

	Color Sample( const TextureBitmap &texture, float x, float y ) const;

};