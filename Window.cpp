#include "pch.h"
#include "Window.h"
#include "resource.h"
#include "Mouse.h"
#include "Keyboard.h"

// error exception helper macro
#define WND_EXCEPT( hr ) Window::Exception( __LINE__,__FILE__,hr )
#define WND_LAST_EXCEPT() Window::Exception( __LINE__,__FILE__,GetLastError() )

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInst( GetModuleHandle( nullptr ) ) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof( wc );
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>( LoadImage(
		GetInstance(), MAKEINTRESOURCE( IDI_ICON1 ),
		IMAGE_ICON, 32, 32, 0
	) );
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>( LoadImage(
		GetInstance(), MAKEINTRESOURCE( IDI_ICON1 ),
		IMAGE_ICON, 16, 16, 0
	) );
	RegisterClassEx( &wc );
}

Window::Window( int width, int height, const char *name ) : width( width ), height( height ) {
	// calculate window size based on desired client region size
	RECT wr = {};
	wr.left = 0;
	wr.right = width + wr.left;
	wr.top = 0;
	wr.bottom = height + wr.top;
	DWORD winFlags = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	if ( AdjustWindowRect( &wr, winFlags, FALSE ) == 0 ) {
		throw WND_LAST_EXCEPT();
	}

	// create window & get hWnd
	hWnd = CreateWindow(
		WindowClass::GetName(), name, winFlags,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	if ( hWnd == nullptr )
		throw WND_LAST_EXCEPT();
	// newly created function starts off as hidden
	ShowWindow( hWnd, SW_SHOWDEFAULT );
}

const char *Window::WindowClass::GetName() noexcept {
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
	return wndClass.hInst;
}

Window::WindowClass::~WindowClass() {
	UnregisterClass( wndClassName, GetInstance() );
}

Window::~Window() {
	DestroyWindow( hWnd );
}

void Window::SetTitle( const std::string &title ) {
	if ( SetWindowText( hWnd, title.c_str() ) == 0 ) {
		throw WND_LAST_EXCEPT();
	}
}

void Window::SetMouseToWindow( Mouse &mouse ) {
	mouse.SetWindow( hWnd );
}

std::optional<int> Window::ProcessMessages() {
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) ) {
		// check for quit because peekmessage does not signal this via return val
		if ( msg.message == WM_QUIT ) {
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return (int)msg.wParam;
		}

		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	// return empty optional when not quitting app
	return {};
}

void Window::Update() {}

LRESULT Window::HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept {
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if ( msg == WM_NCCREATE ) {
		// extract ptr to window class from creation data
		const CREATESTRUCTW *const pCreate = reinterpret_cast<CREATESTRUCTW *>( lParam );
		Window *const pWnd = static_cast<Window *>( pCreate->lpCreateParams );
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pWnd ) );
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &Window::HandleMsgThunk ) );
		// forward message to window instance handler
		return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

LRESULT Window::HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept {
	// retrieve ptr to window instance
	Window *const pWnd = reinterpret_cast<Window *>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
	// forward message to window instance handler
	return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
}

LRESULT Window::HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept {
	switch ( msg ) {
	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;
		break;
	}

	Keyboard::ProcessMessage( msg, wParam, lParam );
	Mouse::ProcessMessage( msg, wParam, lParam );

	// do default Windows stuff for window
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

Window::Exception::Exception( int line, const char *file, HRESULT hr ) noexcept :
	ApplicationException( line, file ),
	hr( hr ) {}

const char *Window::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char *Window::Exception::GetType() const noexcept {
	return "Window Exception";
}

std::string Window::Exception::TranslateErrorCode( HRESULT hr ) noexcept {
	char *pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		reinterpret_cast<LPSTR>( &pMsgBuf ), 0, nullptr
	);
	// 0 string length returned indicates a failure
	if ( nMsgLen == 0 ) {
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree( pMsgBuf );
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept {
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept {
	return TranslateErrorCode( hr );
}
