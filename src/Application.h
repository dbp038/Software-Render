#pragma once
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"

class Application {

public:

	friend class AppInitializer;

	static Application &Get();

private:

	Application() = default;

	static Application *pAppInstance;

	// Only one window in this application
	std::unique_ptr<Window> pWnd;

	std::unique_ptr<Keyboard> pKeyboard;
	std::unique_ptr<Mouse> pMouse;

	std::unique_ptr<Game> pGame;

	int Start();
	int MainLoop();

};

#define App							Application::Get()