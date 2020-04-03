#pragma once

namespace GeometryGenerator {

	namespace Cube {

		std::vector<Vector4f> GetVertices( float size );
		std::vector<Vector4f> GetWrapedVertices( const float size );
		std::vector<Vector4f> GetClampedVertices( const float size );

		std::vector<size_t> GetLines();
		std::vector<size_t> GetIndices();
		std::vector<size_t> GetWrapedIndices();
		std::vector<size_t> GetClampedIndices();

		std::vector<Vector4f> GetColors();
		std::vector<Vector2f> GenerateWrapedUVs();
		std::vector<Vector2f> GenerateClampedUVs();

	}

}