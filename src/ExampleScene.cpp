#include "pch.h"
#include "ExampleScene.h"
#include "Viewport.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Application.h"

ExampleScene::ExampleScene() : cube( 1.0f ) {}

void ExampleScene::Update() {
	auto &kb = Keyboard::Get();
	float speed = rotationSpeed;
	if ( kb.IsKeyDown( Keyboard::LeftShift ) )
		speed *= 2;
	if ( kb.IsKeyDown( Keyboard::W ) ) {
		pitch -= Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::S ) ) {
		pitch += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::D ) ) {
		yaw += Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::A ) ) {
		yaw -= Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::E ) ) {
		roll -= Time.delta * speed;
	}
	if ( kb.IsKeyDown( Keyboard::Q ) ) {
		roll += Time.delta * speed;
	}
}

void ExampleScene::Draw( Graphics &gfx ) {
	unsigned char r, g, b;
	float dt = Time.delta;
	double time = Time.totalTime;
	r = unsigned char( ( sin( time + 2.0f * PI / 3.0f ) / 2.0f + 0.5f ) * 255.0f );
	g = unsigned char( ( sin( time + 4.0f * PI / 3.0f ) / 2.0f + 0.5f ) * 255.0f );
	b = unsigned char( ( sin( time ) / 2.0f + 0.5f ) * 255.0f );
	Viewport screenTransformer( gfx.GetScreenSize() );
	auto lines = cube.GetLines();
	for ( auto &vert : lines.vertices ) {
		vert = Matrices::RotationYawPitchRoll4f( yaw, pitch, roll ) * vert;
		// add some value to z, for now we ignore z axis but later we'll not.
		vert = Matrices::Translation4f(0.0f, 0.0f, 2.0f) * vert;
		
		vert = Matrices::PerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 1000.0f ) * vert;
		//vert = Matrices::PerspectiveFovLH( 5.0f * PI / 12.0f, 4.0f / 3.0f, 0.5f, 1000.0f ) * vert;
		screenTransformer.Transform( vert );
	}
	auto &indices = lines.indices;
	for ( auto i = lines.indices.cbegin(),
		end = lines.indices.cend();
		i != end; std::advance( i, 2 ) ) {
		gfx.DrawLine( lines.vertices[ *i ], lines.vertices[ *std::next( i ) ], Color( r, g, b ) );
	}
}

