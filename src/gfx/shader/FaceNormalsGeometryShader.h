#pragma once

struct FaceNormalsGeometryShader {

	std::array<PosNUvVertex, 3> operator()( size_t triangleId, const PosUvVertex &v0, const PosUvVertex &v1, const PosUvVertex &v2 ) {
		const Vector3f *pV0 = reinterpret_cast<const Vector3f *>( &v0.position );
		const Vector3f *pV1 = reinterpret_cast<const Vector3f *>( &v1.position );
		const Vector3f *pV2 = reinterpret_cast<const Vector3f *>( &v2.position );
		Vector3f faceNormal = ( *pV1 - *pV0 ).cross( *pV2 - *pV0 ).normalized();
		PosNUvVertex vOut0, vOut1, vOut2;
		vOut0.position = v0.position;
		vOut1.position = v1.position;
		vOut2.position = v2.position;
		vOut0.uvs = v0.uvs;
		vOut1.uvs = v1.uvs;
		vOut2.uvs = v2.uvs;
		vOut0.normal = faceNormal;
		vOut1.normal = faceNormal;
		vOut2.normal = faceNormal;
		return { vOut0, vOut1, vOut2 };
	}

};