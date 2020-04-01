#pragma once
#include "ApplicationException.h"
#include "Graphics.h"
#include "Game.h"

class Mouse;

class Window {

public:

	friend class Application;

	class Exception : public ApplicationException {
	public:
		Exception( int line, const char *file, HRESULT hr ) noexcept;
		const char *what() const noexcept override;
		virtual const char *GetType() const noexcept;
		static std::string TranslateErrorCode( HRESULT hr ) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};

	Window( int width, int height, const char *name );
	~Window();
	Window( const Window & ) = delete;
	Window &operator=( const Window & ) = delete;

	const std::string &GetTitle() const;

	void SetTitle( const std::string &title );
	void SetMouseToWindow();

private:

	// singleton manages registration/cleanup of window class
	class WindowClass {
	public:
		static const char *GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass( const WindowClass & ) = delete;
		WindowClass &operator=( const WindowClass & ) = delete;
		static constexpr const char *wndClassName = "SoftwareRender Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

	std::string title;
	int width;
	int height;
	HWND hWnd;
	Graphics gfx;
	Game game;

	static LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;

	static std::optional<int> ProcessMessages();
	void StartUp();
	void Update();

};