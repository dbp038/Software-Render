#pragma once

template<typename Input, typename Output>
struct PerspectiveVertexShader {

	Matrix4f transformMatrix, perspectiveMatrix;

	Output operator()( const Input &vtx ) {
		Output vsOut = vtx;
		vsOut.position = transformMatrix * vsOut.position;
		Vector4f normal( vsOut.normal.x(), vsOut.normal.y(), vsOut.normal.z(), 0.0f );
		normal = transformMatrix * normal;
		vsOut.normal = Vector3f( normal.x(), normal.y(), normal.z() );
		vsOut.position = perspectiveMatrix * vsOut.position;
		return vsOut;
	}

};