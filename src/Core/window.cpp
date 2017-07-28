#include "Core\window.hpp"
#include "Core\core.hpp"

GraphicCookie::Window::Window()
{
}

GraphicCookie::Window::~Window()
{
	ChangeDisplaySettings(NULL, 0);
	DestroyWindow(window_instance_);
	window_instance_ = NULL;
	UnregisterClass(window_name_.c_str(), app_instance_);
	app_instance_ = NULL;
}

void GraphicCookie::Window::CreateWindowGC(float width, float height, int number_parameters)
{
	HINSTANCE app_instance_ = GetModuleHandle(NULL);

	Window* window = Core::getInstance()->getWindow();

	window->width_ = width;
	window->height_ = height;

	window->window_name_ = "Engine";

	//Windows class struct to fill (necessary to create the window) and clear the memory
	ZeroMemory(&window->window_class_, sizeof(WNDCLASSEX));

	window->window_class_.cbSize = sizeof(WNDCLASSEX); //Tell the size of the struct WNDCLASSEX | WNDCLASS ETC...
	window->window_class_.style = CS_HREDRAW | CS_VREDRAW; // Redraws the window if we move it horizontally or vertically.
	window->window_class_.lpfnWndProc = WindowProc; // Function to handle the events
	window->window_class_.hInstance = app_instance_; // App instance given in WinMain
	window->window_class_.hCursor = LoadCursor(NULL, IDC_HAND); // Load a cursor in this case the hand one
	window->window_class_.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	window->window_class_.hIconSm = window->window_class_.hIcon;
	window->window_class_.lpszMenuName = NULL;
	window->window_class_.lpszClassName = window->window_name_.c_str(); // Name the window class

	RegisterClassEx(&window->window_class_);//Register the window class.

	RECT window_rect = { 0, 0, width, height };
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, false);
	

	window->window_instance_ = CreateWindowEx(NULL,
		window->window_name_.c_str(), //Name of the windows class
		"Directx11 Test", //Title show in the window's bar
		WS_OVERLAPPEDWINDOW, // Style of the window
		300, // x position
		300, // y position
		window_rect.right - window_rect.left, // window width
		window_rect.bottom - window_rect.top, // window height
		NULL, // Parent window
		NULL, // Menu bar handler
		app_instance_, // program handler passed in WinMain
		NULL // Use in case of multiple windows
	);

	//If the window is not created show an error message
	if (!window->window_instance_) {
		MessageBox(NULL, "Window not created", "ERROR", MB_OK);
		return;
	}

	ShowWindow(window->window_instance_, number_parameters);
	SetForegroundWindow(window->window_instance_);
	SetFocus(window->window_instance_);

	Core::getInstance()->InitEngine();
}

void GraphicCookie::Window::getWindowSize(float* width, float* height)
{
	*width = width_;
	*height = height_;
}

HWND GraphicCookie::Window::getHandler()
{
	return window_instance_;
}

LRESULT GraphicCookie::Window::WindowProc(HWND window_handler, UINT message, WPARAM w_param, LPARAM l_param)
{
	switch (message)
	{
		//In case we receive a message to detroy de window do it and return 0 because we handled the message.
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	default:
		break;
	}

	//If we didn't handled the message return it to the system
	return DefWindowProc(window_handler, message, w_param, l_param);
}
