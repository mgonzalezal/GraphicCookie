#include "Windows.h"

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

IDXGISwapChain *g_swap_chain;
ID3D11Device *g_device;
ID3D11DeviceContext *g_device_context;
ID3D11RenderTargetView *g_main_back_buffer;
ID3D11PixelShader *g_pixel_shader;
ID3D11VertexShader *g_vertex_shader;
ID3D11Buffer *g_vertex_buffer;
ID3D11InputLayout *g_input_layout;

struct Vertex {
	float x, y, z;
	D3DXCOLOR color;
};

void InitPipeline() {
	ID3D10Blob *vertex_shader, *pixel_shader;

	// Load shaders from a file and compile them.
	D3DX11CompileFromFile("../data/shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &vertex_shader, 0, 0);
	D3DX11CompileFromFile("../data/shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &pixel_shader, 0, 0);

	//Create the shaders.
	g_device->CreateVertexShader(vertex_shader->GetBufferPointer(), vertex_shader->GetBufferSize(), NULL, &g_vertex_shader);
	g_device->CreatePixelShader(pixel_shader->GetBufferPointer(), pixel_shader->GetBufferSize(), NULL, &g_pixel_shader);

	//And activate them.
	g_device_context->VSSetShader(g_vertex_shader, 0, 0);
	g_device_context->PSSetShader(g_pixel_shader, 0, 0);

	// Specify the content of the vertex data.
	D3D11_INPUT_ELEMENT_DESC input_element_desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//Create the input layout with our data filled above.
	g_device->CreateInputLayout(input_element_desc, 2, vertex_shader->GetBufferPointer(), vertex_shader->GetBufferSize(), &g_input_layout);
	//And set it.
	g_device_context->IASetInputLayout(g_input_layout);
}

void InitGraphics() {
	Vertex triangle[] =
	{
		{ 0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
		{ 0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
		{ -0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
	};


	//Create the buffer description.
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));

	//Fill it with desired data
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC; // CPU can modify / GPU only read it.
	buffer_desc.ByteWidth = sizeof(Vertex) * 3; // Size of the buffer, as we want to draw a triangle multiply by 3.
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Specify the data of the buffer.
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow the cpu to modify this buffer.

														 //Now create the buffer.
	g_device->CreateBuffer(&buffer_desc, NULL, &g_vertex_buffer);

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	g_device_context->Map(g_vertex_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped_subresource); // Map the resource.
	memcpy(mapped_subresource.pData, triangle, sizeof(triangle)); //Upload the data.
	g_device_context->Unmap(g_vertex_buffer, NULL); // Unmap it.
}

//Function to initialize directx11 passing a window as a parameter.
void InitDirectx(HWND window_handler) {

	//Create a struct with the definition of our swap chain (back buffers and stuff).
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swap_chain_desc.BufferCount = 1; // Number of back buffers desired.
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Color format.
	swap_chain_desc.BufferDesc.Width = SCREEN_WIDTH; // back buffer width
	swap_chain_desc.BufferDesc.Height = SCREEN_HEIGHT; // back buffer height
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Specify the use of the back buffer in this case we want it to draw in the screen.
	swap_chain_desc.OutputWindow = window_handler; // Window where we want to draw stuff.
	swap_chain_desc.SampleDesc.Count = 4; // Number of multisample we want.
	swap_chain_desc.Windowed = true; // Just tell if the window is windowed or not.
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // This allows to switch from windowed to fullscreen and viceversa.

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

	ID3D11Texture2D* back_buffer; // Declare a 2D texture where we are going to render.
	g_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
	g_device->CreateRenderTargetView(back_buffer, NULL, &g_main_back_buffer);
	back_buffer->Release();
	g_device_context->OMSetRenderTargets(1, &g_main_back_buffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	g_device_context->RSSetViewports(1, &viewport);

	InitPipeline();
	InitGraphics();
}

//Function to clear directx.
void CleanDirectx() {
	g_swap_chain->SetFullscreenState(false, NULL); // Go to windowed mode when closing.
	g_pixel_shader->Release();
	g_vertex_shader->Release();
	g_vertex_buffer->Release();
	g_swap_chain->Release();
	g_main_back_buffer->Release();
	g_device->Release();
	g_device_context->Release();
}

void Render() {
	g_device_context->ClearRenderTargetView(g_main_back_buffer, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_device_context->IASetVertexBuffers(0, 1, &g_vertex_buffer, &stride, &offset); // Set the buffers.
	g_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // Set the drawing mode.
	g_device_context->Draw(3, 0); // Draw it.

	g_swap_chain->Present(0, 0);
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
	window_class.hCursor = LoadCursor(app_instance, IDC_ARROW); // Load a cursor in this case the hand one
																//window_class.hbrBackground = (HBRUSH)COLOR_WINDOW; // Specify the window background color.
	window_class.lpszClassName = "WindowClass"; // Name the window class

												//Register the window class.
	RegisterClassEx(&window_class);

	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, false);

	window_handler = CreateWindowEx(NULL,
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

		Render();
	}

	CleanDirectx();

	return 0;
}