#pragma once

class Viewport {

public:

	Viewport( int width, int height );
	Viewport( int startX, int startY, int width, int height );
	Viewport( const Vector2f &size );
	Viewport( const Vector4f &vp );

	// Transforms a coordinate to screen space.
	//	- returns: A reference to coord with transformations applied.
	Vector4f &Transform( Vector4f &coord ) const;
	// Transforms a coordinate to screen space.
	//	- returns: A value for coord with transformations applied.
	Vector4f GetTransformed( const Vector4f &coord ) const;

private:

	Vector4f offset = Vector4f::Zero();
	Vector4f factor;

};