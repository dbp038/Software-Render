#pragma once
#include "Window.h"
#include <memory>

class Application {

public:

	friend class AppInitializer;

	static Application &Get();

private:

	Application() = default;

	static Application *pAppInstance;

	// Only one window in this application
	std::unique_ptr<Window> pWnd;

	int Start();
	void Update();

};

#define App		Application::Get()