#pragma once

template<typename Input>
struct SingleColorPixelShader {

	Color color;

	Color operator()( const Input &vtx ) {
		return color;
	}

};