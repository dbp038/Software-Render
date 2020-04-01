#include "pch.h"
#include "ExampleScene.h"
#include "Viewport.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Application.h"


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


}

void ExampleScene::ShowFPS() {
	if ( storedWindowName.empty() )
		storedWindowName = App.GetWindow().GetTitle();
	if ( winTitleUpdater.IsFinished() ) {
		App.GetWindow().SetTitle( storedWindowName + " (FPS = " + std::to_string( int( 1.0f / Time.delta ) ) + ")" );
		winTitleUpdater.Reset();
	}
}

