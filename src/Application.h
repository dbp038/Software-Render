#pragma once
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Time.h"

class Application {

public:

	friend class AppInitializer;

	static Application &Get();
	static bool Exists();
	static const TimeData &GetTime();

private:

	Application() = default;

	static Application *pAppInstance;
	static TimeData appTime;

	// Only one window in this application
	std::unique_ptr<Window> pWnd;

	std::unique_ptr<Keyboard> pKeyboard;
	std::unique_ptr<Mouse> pMouse;

	std::unique_ptr<Game> pGame;

	UnscaledTimer deltaTimer;

	int Start();
	int MainLoop();

};

#define App							Application::Get()
#define Time						Application::GetTime()