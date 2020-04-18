#pragma once

struct PerspectiveVertexShader {

	Matrix4f transformMatrix, perspectiveMatrix;

	PosNUvVertex operator()( const PosNUvVertex &vtx ) {
		PosNUvVertex vsOut = vtx;
		vsOut.position = transformMatrix * vsOut.position;
		Vector4f normal( vsOut.normal.x(), vsOut.normal.y(), vsOut.normal.z(), 0.0f );
		normal = transformMatrix * normal;
		vsOut.normal = Vector3f( normal.x(), normal.y(), normal.z() );
		vsOut.position = perspectiveMatrix * vsOut.position;
		return vsOut;
	}

};