#pragma once
#include "gfx/Bitmap.h"
#include "gfx/TextureSampler.h"

template<typename Input, typename Sampler>
struct TexturePixelShader {

	TextureBitmap texture;
	Sampler sampler;

	Color operator()( const Input &vtx ) {
		return sampler.Sample( texture, vtx.uvs.x(), vtx.uvs.y() );
	}

};