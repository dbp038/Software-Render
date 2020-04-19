#include "platform.h"
#include "GeometryGenerator.h"
#include "gfx/core/Color.h"

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

std::vector<Vector4f> GeometryGenerator::Cube::GetWrapedVertices( const float size ) {
	const float side = size / 2.0f;
	return {
		{ -side, -side, -side, 1.0f }, // 0
		{ side, -side, -side, 1.0f }, // 1
		{ -side, side, -side, 1.0f }, // 2
		{ side, side, -side, 1.0f }, // 3
		{ -side, -side, side, 1.0f }, // 4
		{ side, -side, side, 1.0f }, // 5
		{ -side, side, side, 1.0f }, // 6
		{ side, side, side, 1.0f }, // 7
		{ -side, -side, -side, 1.0f }, // 8
		{ side, -side, -side, 1.0f }, // 9
		{ -side, -side, -side, 1.0f }, // 10
		{ -side, -side, side, 1.0f }, // 11
		{ side, -side, -side, 1.0f }, // 12
		{ side, -side, side, 1.0f } // 13
	};
}

std::vector<Vector4f> GeometryGenerator::Cube::GetClampedVertices( const float size ) {
	return GetWrapedVertices( size );
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

std::vector<size_t> GeometryGenerator::Cube::GetWrapedIndices() {
	return {
			0,2,1,		2,3,1,
			4,8,5,		5,8,9,
			2,6,3,		3,6,7,
			4,5,7,		4,7,6,
			2,10,11,	2,11,6,
			12,3,7,		12,7,13
	};
}

std::vector<size_t> GeometryGenerator::Cube::GetClampedIndices() {
	return GetWrapedIndices();
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

std::vector<Vector2f> GeometryGenerator::Cube::GenerateWrapedUVs() {
	return {
		{ 1.0f, 0.0f	},
		{ 0.0f, 0.0f	},
		{ 1.0f, 1.0f	},
		{ 0.0f, 1.0f	},
		{ 1.0f, 3.0f	},
		{ 0.0f, 3.0f	},
		{ 1.0f, 2.0f	},
		{ 0.0f, 2.0f	},
		{ 1.0f, 4.0f	},
		{ 0.0f, 4.0f	},
		{ 2.0f, 1.0f	},
		{ 2.0f, 2.0f	},
		{ -1.0f, 1.0f	},
		{ -1.0f, 2.0f	}
	};
}

std::vector<Vector2f> GeometryGenerator::Cube::GenerateClampedUVs() {
	return {
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f },
		{ 0.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 1.0f, 0.0f }
	};
}
