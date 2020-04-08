#pragma once
#include "gfx/context/VertexDeclarations.h"

struct FaceColorGeometryShader {

	std::vector<Vector4f> faceColors;

	std::array<PosColorVertex, 3> operator()( size_t triangleId, const PosVertex &v0, const PosVertex &v1, const PosVertex &v2 ) {
		PosColorVertex vOut0 = { v0.position, faceColors[ ( triangleId / 2 ) % faceColors.size() ] };
		PosColorVertex vOut1 = { v1.position, faceColors[ ( triangleId / 2 ) % faceColors.size() ] };
		PosColorVertex vOut2 = { v2.position, faceColors[ ( triangleId / 2 ) % faceColors.size() ] };
		return { vOut0, vOut1, vOut2 };
	}

};