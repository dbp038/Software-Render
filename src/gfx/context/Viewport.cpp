#include "platform.h"
#include "Viewport.h"

Viewport::Viewport( int width, int height )
	: factor( ( width - 1 ) / 2.0f, ( height - 1 ) / 2.0f, 1.0f, 1.0f ) 
{}

Viewport::Viewport( int startX, int startY, int width, int height )
	: factor( ( width - 1 ) / 2.0f, ( height - 1 ) / 2.0f, 1.0f, 1.0f ),
	offset( startX, startY, 0.0f, 0.0f )
{}

Viewport::Viewport( const Vector2f &size )
	: Viewport( size.x(), size.y() )
{}

Viewport::Viewport( const Vector4f &vp )
	: Viewport( vp.x(), vp.y(), vp.z(), vp.w() ) 
{}

Vector4f &Viewport::Transform( Vector4f &coord ) const {
	// change y axis from going upwards to going downwards the screen
	coord.y() *= -1.0f;
	// transform xy space from [-1, 1] to [0, 2]
	coord += Vector4f( 1.0, 1.0f, 0.0f, 0.0f );
	// transform xy coordinates to pixels in screen, note that factor variable already
	// includes the division by 2 built in to transform space to [0.0f, 1.0f]
	coord = coord.cwiseProduct( factor ) + offset;
	return coord;
}

Vector4f Viewport::GetTransformed( const Vector4f &coord ) const {
	Vector4f transfCoord = coord;
	return Transform( transfCoord );
}