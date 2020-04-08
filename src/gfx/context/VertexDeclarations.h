#pragma once
#include "gfx/core/Color.h"

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

struct PosUvVertex : public PosVertex {

	Vector2f uvs;

	PosUvVertex &operator+=( const PosUvVertex &rhs ) {
		PosVertex::operator+=( rhs );
		uvs += rhs.uvs;
		return *this;
	}
	PosUvVertex &operator*=( const float rhs ) {
		PosVertex::operator*=( rhs );
		uvs *= rhs;
		return *this;
	}

};