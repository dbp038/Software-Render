#pragma once

class DepthBuffer {

public:

	DepthBuffer() = default;
	DepthBuffer( const unsigned int width, const unsigned int height );

	void Create( const unsigned int width, const unsigned int height );

	void Clear();
	bool Test( const unsigned int x, const unsigned int y, float z ) const;
	bool TestAndSet( const unsigned int x, const unsigned int y, float z );
	bool Test( size_t offset, float z ) const;
	bool TestAndSet( size_t offset, float z );

private:

	std::vector<float> zbuffer;
	unsigned int width = 0, height = 0;
	size_t totalSize = 0;

	inline size_t GetOffset( const unsigned int x, const unsigned int y ) const {
		return (size_t)y * width + x;
	}

};