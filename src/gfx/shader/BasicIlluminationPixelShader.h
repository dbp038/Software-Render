#pragma once
#include "gfx/context/TextureSampler.h"

struct BasicIlluminationGlobalColorPixelShader {

	Vector4f globalColor = Colors::White.ToVector4f();
	Vector3f lightDirection = Vector3f( 0.0f, 0.0f, 1.0f );
	float lightIntensity = 1.0f;
	float ambientFactor = 0.15f;

	Color operator()( const PosNVertex &vtx ) {
		Color out = globalColor;
		float illuminationFactor = std::max( -vtx.normal.dot( lightDirection ) * lightIntensity, ambientFactor );
		return out * illuminationFactor;
	}

};

template<typename Sampler>
struct BasicIlluminationTexturePixelShader {

	Vector3f lightDirection = Vector3f( 0.0f, 0.0f, 1.0f );
	float lightIntensity = 1.0f;
	float ambientFactor = 0.15f;

	TextureBitmap texture;
	Sampler sampler;

	Color operator()( const PosNUvVertex &vtx ) {
		Color out = sampler.Sample( texture, vtx.uvs.x(), vtx.uvs.y() );
		float illuminationFactor = std::max( -vtx.normal.dot( lightDirection ) * lightIntensity, ambientFactor );
		return out * illuminationFactor;
	}

};