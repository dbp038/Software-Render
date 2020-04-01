#pragma once
#include "Color.h"

struct PosVertex {

	Vector4f position;

	// operators so we now how to interpolate vertices
	PosVertex &operator+=( const PosVertex &rhs ) {
		position += rhs.position;
		return *this;
	}
	PosVertex &operator*=( const float rhs ) {
		position *= rhs;
		return *this;
	}

};

struct PosColorVertex : public PosVertex {

	Vector4f color;

	PosColorVertex &operator+=( const PosColorVertex &rhs ) {
		PosVertex::operator+=( rhs );
		color += rhs.color;
		return *this;
	}
	PosColorVertex &operator*=( const float rhs ) {
		PosVertex::operator*=( rhs );
		color *= rhs;
		return *this;
	}

};