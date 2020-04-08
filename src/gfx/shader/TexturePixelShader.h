#pragma once
#include "gfx/core/Bitmap.h"
#include "gfx/context/TextureSampler.h"

template<typename Input, typename Sampler>
struct TexturePixelShader {

	TextureBitmap texture;
	Sampler sampler;

	Color operator()( const Input &vtx ) {
		return sampler.Sample( texture, vtx.uvs.x(), vtx.uvs.y() );
	}

};