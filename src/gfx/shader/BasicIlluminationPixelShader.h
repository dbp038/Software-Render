#pragma once

template<typename Sampler>
struct BasicIlluminationPixelShader {

	Vector3f lightDirection = Vector3f( 0.0f, 0.0f, 1.0f );
	float lightIntensity = 1.0f;
	float ambientFactor = 0.05f;

	TextureBitmap texture;
	Sampler sampler;

	Color operator()( const PosNUvVertex &vtx ) {
		Color out = sampler.Sample( texture, vtx.uvs.x(), vtx.uvs.y() );
		float illuminationFactor = std::max( -vtx.normal.dot( lightDirection ) * lightIntensity, ambientFactor );
		return out * illuminationFactor;
	}

};