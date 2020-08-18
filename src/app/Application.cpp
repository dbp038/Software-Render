#include "platform.h"
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

Window &Application::GetWindow() {
	return *pWnd;
}

void Application::RequestClose() {
	PostQuitMessage( 0 );
}

int Application::Start() {
	// initialize keyboard and mouse
	pKeyboard = std::make_unique<Keyboard>();
	pMouse = std::make_unique<Mouse>();

	pWnd = std::make_unique<Window>( 400, 300, "Software Render" );
	pWnd->SetMouseToWindow();


#ifndef NDEBUG
	std::string debugWarningText = std::string( "Compiled using debug settings, this application " ) +
		"was not designed with performance in mind, expect high load times as well as slow program " +
		"behavior.";
	MessageBox( pWnd->hWnd, debugWarningText.c_str(), "DEBUG warning", MB_OK | MB_ICONEXCLAMATION );
#endif

	return MainLoop();
}

int Application::MainLoop() {
	pWnd->StartUp();
	while ( true ) {
		// process all messages pending, but to not block for new messages
		if ( const auto ecode = Window::ProcessMessages() ) {
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		appTime.delta = float( deltaTimer.ElapsedAndReset() ) * appTime.timeScale;
		appTime.totalTime += appTime.delta;
		pKeyboard->Update();
		pWnd->Update();
	}
}
