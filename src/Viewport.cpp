#include "pch.h"
#include "Viewport.h"

Viewport::Viewport( int width, int height )
	: factor( width / 2.0f, height / 2.0f, 1.0f, 1.0f )
{}

Viewport::Viewport( const Vector2i &size )
	: Viewport( size.x(), size.y() ) {}

Vector4f &Viewport::Transform( Vector4f &coord ) const {
	// apply the perspective divide and store z value (inverse of z inverse) in
	// w component so we can get homo space coordinates again
 	float z = coord.w();
	coord *= 1.0f / z;
	coord.w() = z;
	// now z is mapped to [0, 1] according to zNear and zFar respectively

	// change y axis from going upwards to going downwards the screen
	coord.y() *= -1.0f;
	// transform xy space from [-1, 1] to [0, 2]
	coord += Vector4f( 1.0, 1.0f, 0.0f, 0.0f );
	// transform xy coordinates to pixels in screen, note that factor variable already
	// includes the division by 2 built in to transform space to [0.0f, 1.0f]
	coord = coord.cwiseProduct( factor );
	return coord;
}

Vector4f Viewport::GetTransformed( const Vector4f &coord ) const {
	Vector4f transfCoord = coord;
	return Transform( transfCoord );
}
