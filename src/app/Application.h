#pragma once
#include "Window.h"
#include "device/Keyboard.h"
#include "device/Mouse.h"
#include "Time.h"

class Application {

public:

	friend class AppInitializer;

	static Application &Get();
	static bool Exists();
	static const TimeData &GetTime();

	Window &GetWindow();

	void RequestClose();

private:

	Application() = default;

	static Application *pAppInstance;
	static TimeData appTime;

	// Only one window in this application
	std::unique_ptr<Window> pWnd;

	std::unique_ptr<Keyboard> pKeyboard;
	std::unique_ptr<Mouse> pMouse;

	UnscaledTimer deltaTimer;

	int Start();
	int MainLoop();

};

#define App							Application::Get()
#define Time						Application::GetTime()