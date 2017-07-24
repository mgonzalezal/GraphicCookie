#include "Windows.h"

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

IDXGISwapChain *g_swap_chain;
ID3D11Device *g_device;
ID3D11DeviceContext * g_device_context;


//Function to initialize directx11 passing a window as a parameter.
void InitDirectx(HWND window_handler) {

	//Create a struct with the definition of our swap chain (back buffers and stuff).
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swap_chain_desc.BufferCount = 1; // Number of back buffers desired.
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Color format.
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Specify the use of the back buffer in this case we want it to draw in the screen.
	swap_chain_desc.OutputWindow = window_handler; // Window where we want to draw stuff.
	swap_chain_desc.SampleDesc.Count = 4; // Number of multisample we want.
	swap_chain_desc.Windowed = true; // Just tell if the window is windowed or not.

	D3D11CreateDeviceAndSwapChain(NULL, // Indicate the graphics adapter we want to use (NULL for the default one).
		D3D_DRIVER_TYPE_HARDWARE, // This tells how we want Directx to work, in our case by hardware.
		NULL, // If the last parameter were SOFTWARE, fill this one.
		NULL, // Flags to allow debugging for example.
		NULL, // Features required for our demo.
		NULL, // Number of features listed before.
		D3D11_SDK_VERSION, // Version of the directx sdk used in our game.
		&swap_chain_desc, // Pointer to the swap chain defined before.
		&g_swap_chain, // Swap chain pointer to a pointer, meaning that is going to create the object for us.
		&g_device, // Same as above but for the device
		NULL, // Feature levels if we put a variable here, we could be able to know what features we have available.
		&g_device_context // Same as the swap chain and device.
	);
}

//Function to clear directx.
void CleanDirectx() {
	g_swap_chain->Release();
	g_device->Release();
	g_device_context->Release();
}

LRESULT CALLBACK WindowProc(HWND window_handler, UINT message, WPARAM w_param, LPARAM l_param) {
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

int WINAPI WinMain(HINSTANCE app_instance, HINSTANCE previous_instance, LPSTR cmd_data, int number_parameters) {
	//Window handler.
	HWND window_handler;

	//Windows class struct to fill (necessary to create the window) and clear the memory
	WNDCLASSEX window_class;
	ZeroMemory(&window_class, sizeof(WNDCLASSEX));

	window_class.cbSize = sizeof(WNDCLASSEX); //Tell the size of the struct WNDCLASSEX | WNDCLASS ETC...
	window_class.style = CS_HREDRAW | CS_VREDRAW; // Redraws the window if we move it horizontally or vertically.
	window_class.lpfnWndProc = WindowProc; // Function to handle the events
	window_class.hInstance = app_instance; // App instance given in WinMain
	window_class.hCursor = LoadCursor(app_instance, IDC_HAND); // Load a cursor in this case the hand one
	window_class.hbrBackground = (HBRUSH)COLOR_WINDOW; // Specify the window background color.
	window_class.lpszClassName = "WindowClass"; // Name the window class

												//Register the window class.
	RegisterClassEx(&window_class);

	RECT window_rect = { 0, 0, 800, 600 };
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, false);

	window_handler = CreateWindowEx(NULL,
		"WindowClass", //Name of the windows class
		"Directx11 Test", //Title show in the window's bar
		WS_OVERLAPPEDWINDOW, // Style of the window
		CW_USEDEFAULT, // x position
		CW_USEDEFAULT, // y position
		window_rect.right - window_rect.left, // window width
		window_rect.bottom - window_rect.top, // window height
		NULL, // Parent window
		NULL, // Menu bar handler
		app_instance, // program handler passed in WinMain
		NULL // Use in case of multiple windows
	);

	//If the window is not created show an error message
	if (!window_handler) {
		MessageBox(NULL, "Window not created", "ERROR", MB_OK);
		return 0;
	}

	//In case is created show the window.
	ShowWindow(window_handler, number_parameters);

	InitDirectx(window_handler);

	MSG message;

	while (true) {
		// We use PeekMessage because getmessage is blocking until receives a message, and PeekMessage is not blocking.
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message); //In case we got a keyboard message, translate it in a right way.
			DispatchMessage(&message); // Dispatch the message to the WinProc function.
			if (message.message == WM_QUIT) break; // If the message is WM_QUIT we break the main loop.
		}
		else {

		}
	}

	CleanDirectx();

	return 0;
}