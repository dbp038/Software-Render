#pragma once

// disable warnings, Eigen devs must know what they're doing so there's no point
#pragma warning(push)
#pragma warning ( disable : 26812 )
#pragma warning ( disable : 26450 )
#pragma warning ( disable : 26495 )
#include <Eigen/Dense>
#pragma warning(pop)

using Eigen::Matrix;
using Eigen::MatrixXf;
using Eigen::Matrix2f;
using Eigen::Matrix3f;
using Eigen::Matrix4f;
using Eigen::Vector4f;
using Eigen::Vector3f;
using Eigen::Vector2f;
using Eigen::Vector2i;
using Quaternion = Eigen::Quaternionf;

const double PI = 3.1415926535897932;

template <typename T>
inline auto lerp( const T &v1, const T &v2, const float p ) {
	return ( v2 - v1 ) * p + v1;
}

namespace Matrices {

	inline Matrix3f Translation2D3f( float x, float y ) {
		return ( Matrix3f() <<
			1, 0,	x,
			0, 1,	y,
			0, 0,	1
			).finished();
	}
	inline Matrix3f Translation2D3f( const Vector3f &offset ) {
		return Translation2D3f( offset.x(), offset.y() );
	}

	inline Matrix4f Translation4f( float x, float y, float z ) {
		return ( Matrix4f() <<
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1
			).finished();
	}
	inline Matrix4f Translation4f( const Vector3f &offset ) {
		return Translation4f( offset.x(), offset.y(), offset.z() );
	}

	inline Matrix2f Rotation2D2f( float angle ) {
		return ( Matrix2f() <<
			cos( angle ),		sin( angle ),
			-sin( angle ),	cos( angle )
			).finished();
	}
	inline Matrix3f Rotation2D3f( float angle ) {
		return ( Matrix3f() <<
			cos( angle ),		sin( angle ),		0,
			-sin( angle ),	cos( angle ),		0,
			0,							0,							1
			).finished();
	}

	inline Matrix3f RotationX3f( float angle ) {
		return ( Matrix3f() <<
			1,							0,							0,
			0,							cos( angle ),		sin( angle ),
			0,							-sin( angle ),	cos( angle )
			).finished();
	}
	inline Matrix4f RotationX4f( float angle ) {
		return ( Matrix4f() <<
			1,							0,							0,							0,
			0,							cos( angle ),		sin( angle ),		0,
			0,							-sin( angle ),	cos( angle ),		0,
			0,							0,							0,							1
			).finished();
	}

	inline Matrix3f RotationY3f( float angle ) {
		return ( Matrix3f() <<
			cos( angle ),		0,							-sin( angle ),
			0,							1,							0,
			sin( angle ),		0,							cos( angle )
			).finished();
	}
	inline Matrix4f RotationY4f( float angle ) {
		return ( Matrix4f() <<
			cos( angle ),		0,							-sin( angle ),	0,
			0,							1,							0,							0,
			sin( angle ),		0,							cos( angle ),		0,
			0,							0,							0,							1
			).finished();
	}

	inline Matrix3f RotationZ3f( float angle ) {
		return ( Matrix3f() <<
			cos( angle ),		sin( angle ),		0,
			-sin( angle ),	cos( angle ),		0,
			0,							0,							1
			).finished();
	}
	inline Matrix4f RotationZ4f( float angle ) {
		return ( Matrix4f() <<
			cos( angle ),		sin( angle ),		0,							0,
			-sin( angle ),	cos( angle ),		0,							0,
			0,							0,							1,							0,
			0,							0,							0,							1
			).finished();
	}

	inline Matrix3f RotationYawPitchRoll3f( float yaw, float pitch, float roll ) {
		return RotationY3f( yaw ) * RotationX3f( pitch ) * RotationZ3f( roll );
	}
	inline Matrix4f RotationYawPitchRoll4f( float yaw, float pitch, float roll ) {
		return RotationY4f( yaw ) * RotationX4f( pitch ) * RotationZ4f( roll );
	}

	inline Matrix2f Scale2D2f( float x, float y ) {
		return ( Matrix2f() <<
			x, 0,
			0, y
			).finished();
	}
	inline Matrix2f Scale2D2f( const Vector2f &s ) {
		return Scale2D2f( s.x(), s.y() );
	}
	inline Matrix2f Scale2D2f( float s ) {
		return Scale2D2f( s, s );
	}

	inline Matrix3f Scale2D3f( float x, float y ) {
		return ( Matrix3f() <<
			x, 0,	0,
			0, y,	0,
			0, 0,	1
			).finished();
	}
	inline Matrix3f Scale2D3f( const Vector2f &s ) {
		return Scale2D3f( s.x(), s.y() );
	}
	inline Matrix3f Scale2D3f( float s ) {
		return Scale2D3f( s, s );
	}

	inline Matrix3f Scale3f( float x, float y, float z ) {
		return ( Matrix3f() <<
			x, 0,	0,
			0, y,	0,
			0, 0,	z
			).finished();
	}
	inline Matrix3f Scale3f( const Vector3f &s ) {
		return Scale3f( s.x(), s.y(), s.z() );
	}
	inline Matrix3f Scale3f( float s ) {
		return Scale3f( s, s, s );
	}

	inline Matrix4f Scale4f( float x, float y, float z ) {
		return ( Matrix4f() <<
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
			).finished();
	}
	inline Matrix4f Scale4f( const Vector3f &s ) {
		return Scale4f( s.x(), s.y(), s.z() );
	}
	inline Matrix4f Scale4f( float s ) {
		return Scale4f( s, s, s );
	}

	inline Matrix4f PerspectiveLH( float w, float h, float zn, float zf ) {
		return ( Matrix4f() <<
			2 * zn / w,	0,					0,								0,
			0,					2 * zn / h,	0, 								0,
			0,					0,					zf / ( zf - zn ),	zn * zf / ( zn - zf ),
			0,					0,					1,								0
			).finished();
	}
	inline Matrix4f PerspectiveRH( float w, float h, float zn, float zf ) {
		return ( Matrix4f() <<
			2 * zn / w,	0,					0,								0,
			0,					2 * zn / h,	0,								0,
			0,					0,					zf / ( zf - zn ),	zn * zf / ( zn - zf ),
			0,					0,					-1,								0
			).finished();
	}
	inline Matrix4f PerspectiveFovLH( float fovy, float aspect, float zn, float zf ) {
		float yScale = 1.0f / tan( fovy / 2.0f );
		float xScale = yScale / aspect;
		return ( Matrix4f() <<
			xScale, 0,			0,								0,
			0,			yScale, 0,								0,
			0,			0,			zf / ( zf - zn ), zn * zf / ( zn - zf ),
			0,			0,			1,								0
			).finished();
	}
	inline Matrix4f PerspectiveFovRH( float fovy, float aspect, float zn, float zf ) {
		float yScale = 1.0f / tan( fovy / 2.0f );
		float xScale = yScale / aspect;
		return ( Matrix4f() <<
			xScale, 0,			0,								0,
			0,			yScale, 0,								0,
			0,			0,			zf / ( zf - zn ), zn * zf / ( zn - zf ),
			0,			0,			-1,								0
			).finished();
	}

}