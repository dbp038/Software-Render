#include "platform.h"
#include "Application.h"

// This class allows WinMain (and no one else) to have access to Application's Start method
class AppInitializer {

public:

	AppInitializer() {
		Application::pAppInstance = &app;
	}

	int StartApplication() {
		return app.Start();
	}

private:

	Application app;

};

int CALLBACK WinMain(
	_In_			HINSTANCE hInstance,
	_In_opt_	HINSTANCE hPrevInstance,
	_In_			LPSTR			lpCmdLine,
	_In_			int				nCmdShow
) {
	AppInitializer appInit;
	try {
		return appInit.StartApplication();
	}
	catch ( const ApplicationException & e ) {
		MessageBox( nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION );
	}
	catch ( const std::exception & e ) {
		MessageBox( nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION );
	}
	catch ( ... ) {
		MessageBox( nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION );
	}
	return -1;
}