#include "platform.h"
#include "DepthBuffer.h"

DepthBuffer::DepthBuffer( const unsigned int width, const unsigned int height ) {
	Create( width, height );
}

void DepthBuffer::Create( const unsigned int width, const unsigned int height ) {
	this->width = width;
	this->height = height;
	totalSize = (size_t)width * height;

	zbuffer.assign( totalSize, 1.0f );
}

void DepthBuffer::Clear() {
	std::fill( zbuffer.begin(), zbuffer.end(), 1.0f );
}

bool DepthBuffer::Test( const unsigned int x, const unsigned int y, float z ) const {
	return z < zbuffer.at( GetOffset( x, y ) );
}

bool DepthBuffer::TestAndSet( const unsigned int x, const unsigned int y, float z ) {
	size_t offset = GetOffset( x, y );
	if ( z < zbuffer.at( offset ) ) {
		zbuffer[ offset ] = z;
		return true;
	}
	else
		return false;
}

bool DepthBuffer::Test( size_t offset, float z ) const {
	return z < zbuffer.at( offset );
}

bool DepthBuffer::TestAndSet( size_t offset, float z ) {
	if ( Test( offset, z ) ) {
		zbuffer[ offset ] = z;
		return true;
	}
	else
		return false;
}
