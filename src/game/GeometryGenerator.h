#pragma once

namespace GeometryGenerator {

	namespace Cube {

		std::vector<Vector4f> GetVertices( float size );

		std::vector<size_t> GetLines();

		std::vector<size_t> GetIndices();

		std::vector<Vector4f> GetColors();

	}

}