#include "pch.h"
#include "Application.h"

Application *Application::pAppInstance = nullptr;

Application &Application::Get() {
	return *pAppInstance;
}

int Application::Start() {

	pWnd = std::make_unique<Window>( 640, 640, "DBP's Software Render" );

	// initialize keyboard and mouse
	pKeyboard = std::make_unique<Keyboard>();
	pMouse = std::make_unique<Mouse>();
	pWnd->SetMouseToWindow();

	return MainLoop();
}

int Application::MainLoop() {
	while ( true ) {
		// process all messages pending, but to not block for new messages
		if ( const auto ecode = Window::ProcessMessages() ) {
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		pKeyboard->Update();
		pWnd->Update();
	}
}
