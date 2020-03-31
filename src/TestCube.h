#pragma once

class TestCube {

public:
	struct IndexedLineList {
		std::vector<Vector4f> vertices;
		std::vector<size_t> indices;
	};
	struct IndexedTriangleList {
		std::vector<Vector4f> vertices;
		std::vector<size_t> indices;
	};

	TestCube( float size ) {
		const float side = size / 2.0f;
		vertices.emplace_back( -side, -side, -side, 1.0f );
		vertices.emplace_back( side, -side, -side, 1.0f );
		vertices.emplace_back( -side, side, -side, 1.0f );
		vertices.emplace_back( side, side, -side, 1.0f );
		vertices.emplace_back( -side, -side, side, 1.0f );
		vertices.emplace_back( side, -side, side, 1.0f );
		vertices.emplace_back( -side, side, side, 1.0f );
		vertices.emplace_back( side, side, side, 1.0f );
	}
	IndexedLineList GetLines() const {
		return{
			vertices,{
			0,1,  1,3,  3,2,  2,0,
			0,4,  1,5,	3,7,  2,6,
			4,5,  5,7,	7,6,  6,4 }
		};
	}
	IndexedTriangleList GetTriangles() const {
		return{
			vertices,{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4 }
		};
	}
private:
	std::vector<Vector4f> vertices;

};