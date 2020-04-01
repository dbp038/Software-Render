#include "pch.h"
#include "GeometryGenerator.h"
#include "Color.h"

std::vector<Vector4f> GeometryGenerator::Cube::GetVertices( float size ) {
	const float side = size / 2.0f;
	return {
		{ -side, -side, -side, 1.0f },
		{ side, -side, -side, 1.0f },
		{ -side, side, -side, 1.0f },
		{ side, side, -side, 1.0f },
		{ -side, -side, side, 1.0f },
		{ side, -side, side, 1.0f },
		{ -side, side, side, 1.0f },
		{ side, side, side, 1.0f }
	};
}

std::vector<size_t> GeometryGenerator::Cube::GetLines() {
	return {
	0,1,  1,3,  3,2,  2,0,
	0,4,  1,5,	3,7,  2,6,
	4,5,  5,7,	7,6,  6,4
	};
}

std::vector<size_t> GeometryGenerator::Cube::GetIndices() {
	return {
	0,2,1, 2,3,1,
	1,3,5, 3,7,5,
	2,6,3, 3,6,7,
	4,5,7, 4,7,6,
	0,4,2, 2,4,6,
	0,1,4, 1,5,4
	};
}

std::vector<Vector4f> GeometryGenerator::Cube::GetColors() {
	return {
		Colors::Blue.ToVector4f(),
		Colors::Red.ToVector4f(),
		Colors::Cyan.ToVector4f(),
		Colors::Gray.ToVector4f(),
		Colors::Green.ToVector4f(),
		Colors::Pink.ToVector4f(),
		Colors::Magenta.ToVector4f(),
		Colors::Orange.ToVector4f()
	};
}
