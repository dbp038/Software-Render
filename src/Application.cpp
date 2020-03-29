#include "pch.h"
#include "Application.h"

Application *Application::pAppInstance = nullptr;
TimeData Application::appTime;

Application &Application::Get() {
	return *pAppInstance;
}

bool Application::Exists() {
	return pAppInstance != nullptr;
}

const TimeData &Application::GetTime() {
	return appTime;
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

		appTime.delta = deltaTimer.ElapsedAndReset() * appTime.timeScale;
		appTime.totalTime += appTime.delta;
		pKeyboard->Update();
		pWnd->Update();
	}
}
