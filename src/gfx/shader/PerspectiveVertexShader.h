#pragma once

template<typename Input, typename Output>
struct PerspectiveVertexShader {

	Matrix4f transformMatrix, perspectiveMatrix;

	Output operator()( const Input &vtx ) {
		Output vsOut = vtx;
		vsOut.position = transformMatrix * vsOut.position;
		vsOut.position = perspectiveMatrix * vsOut.position;
		return vsOut;
	}

};