#include "Core\window.hpp"
#include "Core\core.hpp"

GraphicCookie::Window::Window()
{
}

GraphicCookie::Window::~Window()
{
}

void GraphicCookie::Window::CreateWindowGC(float width, float height, HINSTANCE app_instance, int number_parameters)
{
	Window* window = Core::getInstance()->getWindow();

	window->width_ = width;
	window->height_ = height;

	//Windows class struct to fill (necessary to create the window) and clear the memory
	WNDCLASSEX window_class;
	ZeroMemory(&window_class, sizeof(WNDCLASSEX));

	window_class.cbSize = sizeof(WNDCLASSEX); //Tell the size of the struct WNDCLASSEX | WNDCLASS ETC...
	window_class.style = CS_HREDRAW | CS_VREDRAW; // Redraws the window if we move it horizontally or vertically.
	window_class.lpfnWndProc = WindowProc; // Function to handle the events
	window_class.hInstance = app_instance; // App instance given in WinMain
	window_class.hCursor = LoadCursor(app_instance, IDC_ARROW); // Load a cursor in this case the hand one
																//window_class.hbrBackground = (HBRUSH)COLOR_WINDOW; // Specify the window background color.
	window_class.lpszClassName = "WindowClass"; // Name the window class

												//Register the window class.
	RegisterClassEx(&window_class);

	RECT window_rect = { 0, 0, width, height };
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, false);
	

	window->window_instance_ = CreateWindowEx(NULL,
		"WindowClass", //Name of the windows class
		"Directx11 Test", //Title show in the window's bar
		WS_OVERLAPPEDWINDOW, // Style of the window
		300, // x position
		300, // y position
		window_rect.right - window_rect.left, // window width
		window_rect.bottom - window_rect.top, // window height
		NULL, // Parent window
		NULL, // Menu bar handler
		app_instance, // program handler passed in WinMain
		NULL // Use in case of multiple windows
	);

	//If the window is not created show an error message
	if (!window->window_instance_) {
		MessageBox(NULL, "Window not created", "ERROR", MB_OK);
		return;
	}

	ShowWindow(window->window_instance_, number_parameters);

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
