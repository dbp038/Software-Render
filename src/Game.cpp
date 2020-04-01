#include "pch.h"
#include "Game.h"

// scenes
#include "ExampleScene.h"

void Game::Start() {
	// Add scenes here
	AddScene<ExampleScene>();
}

void Game::Update() {
	if ( !scenes.empty() )
		scenes[ activeScene ]->Update();
}

void Game::Render( Graphics &gfx ) {
	scenes[ activeScene ]->Draw( gfx );
}

void Game::SetCurrentScene( size_t idx ) {
	assert( idx < scenes.size() );
	activeScene = idx;
}

