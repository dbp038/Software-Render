#include "pch.h"
#include "ExampleScene.h"
#include "Viewport.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Application.h"

ExampleScene::ExampleScene() : cube( 1.0f ), winTitleUpdater( 1.0f / 60.0f ) {

}

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

	if ( kb.IsKeyJustPressed( Keyboard::Escape ) )
		App.RequestClose();
}

void ExampleScene::Draw( Graphics &gfx ) {
	ShowFPS();

	unsigned char r, g, b;
	float dt = Time.delta;
	double time = Time.totalTime;
	r = unsigned char( ( sin( time + 2.0f * PI / 3.0f ) / 2.0f + 0.5f ) * 255.0f );
	g = unsigned char( ( sin( time + 4.0f * PI / 3.0f ) / 2.0f + 0.5f ) * 255.0f );
	b = unsigned char( ( sin( time ) / 2.0f + 0.5f ) * 255.0f );
	gfx.ClearBackground( Color( r, g, b ) );
	Viewport screenTransformer( gfx.GetScreenSize() );
	auto tris = cube.GetTriangles();
	for ( auto &vert : tris.vertices ) {
		vert = Matrices::RotationYawPitchRoll4f( yaw, pitch, roll ) * vert;
		vert = Matrices::Translation4f( 0.0f, 0.0f, 2.0f ) * vert;

		vert = Matrices::PerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 1000.0f ) * vert;
		screenTransformer.Transform( vert );
	}
	auto &indices = tris.indices;
	for ( auto i = tris.indices.cbegin(),
		end = tris.indices.cend();
		i != end; std::advance( i, 3 ) ) {
		gfx.DrawTriangle(
			tris.vertices[ *i ], tris.vertices[ *std::next( i ) ], tris.vertices[ *std::next( std::next( i ) ) ], Color( 255 - r, 255 - g, 255 - b ) );
	}
}

void ExampleScene::ShowFPS() {
	if ( storedWindowName.empty() )
		storedWindowName = App.GetWindow().GetTitle();
	if ( winTitleUpdater.IsFinished() ) {
		App.GetWindow().SetTitle( storedWindowName + " (FPS = " + std::to_string( int( 1.0f / Time.delta ) ) + ")" );
		winTitleUpdater.Reset();
	}
}

