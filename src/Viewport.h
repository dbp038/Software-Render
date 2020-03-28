#pragma once

class Viewport {

public:

	Viewport( int width, int height );
	Viewport( const Vector2i &size );

	// Transforms a coordinate to screen space.
	//	- returns: A reference to coord with transformations applied.
	Vector3f &Transform( Vector3f &coord ) const;
	// Transforms a coordinate to screen space.
	//	- returns: A value for coord with transformations applied.
	Vector3f GetTransformed( const Vector3f &coord ) const;

private:

	Vector3f factor;

};