#pragma once
#include "Graphics.h"
#include "Scene.h"

class Game {

public:

	Game();
	Game( const Game & ) = delete;
	Game &operator=( const Game & ) = delete;

	void Update();
	void Render(Graphics &gfx);

	template <typename SceneType, typename ... ConstructorArgs>
	void AddScene( ConstructorArgs && ... cArgs ) {
		scenes.push_back( std::make_unique<SceneType>( std::forward<ConstructorArgs>( cArgs ) ... ) );
	}

	void SetCurrentScene( size_t idx );

private:

	std::vector<std::unique_ptr<Scene>> scenes;
	size_t activeScene;

};