#pragma once

template<typename Input, typename Output>
struct DefaultGeometryShader {

	std::array<Output, 3> operator()( size_t triangleId, const Input &v0, const Input &v1, const Input &v2 ) {
		return { v0, v1, v2 };
	}

};