#pragma once

template<typename Input>
struct VertexColorPixelShader {

	Color operator()( const Input &vtx ) {
		return Color( vtx.color.x(), vtx.color.y(), vtx.color.z(), vtx.color.w() );
	}

};