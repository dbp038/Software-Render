#include "pch.h"
#include "Window.h"
#include "Application.h"

Application *Application::pAppInstance = nullptr;

Application &Application::Get() {
	return *pAppInstance;
}

int Application::Start() {

	pWnd = std::make_unique<Window>( 800, 600, "DBP's Software Render" );

	// main loop
	while ( true ) {
		// process all messages pending, but to not block for new messages
		if ( const auto ecode = Window::ProcessMessages() ) {
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		Update();
	}
}

void Application::Update() {
	pWnd->Update();
}
