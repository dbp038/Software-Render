#include "pch.h"
#include "Viewport.h"

Viewport::Viewport( int width, int height )
	: factor( width / 2.0f, height / 2.0f, 1.0f ) 
{}

Viewport::Viewport( const Vector2i &size )
	: Viewport( size.x(), size.y() ) 
{}

Vector3f &Viewport::Transform( Vector3f &coord ) const {
	// change y axis from going upwards to going downwards the screen
	coord[ 1 ] *= -1.0f;
	// transform space from [-1.0f, 1.0f) to [0.0f, 2.0f)
	coord += Vector3f::Ones();
	// transform coordinates to pixels in screen, note that factor already includes the
	// division by 2 built in to transform space to [0.0f, 1.0f)
	coord = coord.cwiseProduct( factor );
	return coord;
}

Vector3f Viewport::GetTransformed( const Vector3f &coord ) const {
	Vector3f transfCoord = coord;
	return Transform( transfCoord );
}
