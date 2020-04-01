#include "pch.h"
#include "ExampleScene.h"
#include "Viewport.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Application.h"
#include "GeometryGenerator.h"


ExampleScene::ExampleScene() : winTitleUpdater( 1.0f / 60.0f ) {
	auto vertices = GeometryGenerator::Cube::GetVertices( 1.0f );
	auto colors = GeometryGenerator::Cube::GetColors();
	cubeIndices = GeometryGenerator::Cube::GetIndices();
	for ( size_t i = 0; i < vertices.size(); i++ ) {
		cubeVertices.push_back( PosColorVertex{ vertices[ i ], colors[ i ] } );
	}

	ctx.BindVertexData( cubeVertices );
	ctx.BindIndexData( cubeIndices );
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

	gfx.ClearBackground();

	ctx.BindViewport( Viewport( gfx.GetWidth(), gfx.GetHeight() ) );

	Matrix4f transform = Matrices::RotationYawPitchRoll4f( yaw, pitch, roll );
	transform = Matrices::Translation4f( 0.0f, 0.0f, 2.0f ) * transform;
	Matrix4f perspecive = Matrices::PerspectiveLH( 1.0f, 3.0f / 4.0f, 0.5f, 1000.0f );
	ctx.BindVertexShaderBuffer( { transform, perspecive } );

	gfx.BindContext( ctx );
	gfx.Draw();
}

void ExampleScene::ShowFPS() {
	if ( storedWindowName.empty() )
		storedWindowName = App.GetWindow().GetTitle();
	if ( winTitleUpdater.IsFinished() ) {
		App.GetWindow().SetTitle( storedWindowName + " (FPS = " + std::to_string( int( 1.0f / Time.delta ) ) + ")" );
		winTitleUpdater.Reset();
	}
}

