#include "./Core/core.hpp"
#include "./Core/window.hpp"


#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

//Definition of the core data needed for the engine to work
struct GraphicCookie::Core::CoreData {
	IDXGISwapChain *swap_chain;
	ID3D11Device *device;
	ID3D11DeviceContext *device_context;
	ID3D11RenderTargetView *main_back_buffer;
	ID3D11PixelShader *pixel_shader;
	ID3D11VertexShader *vertex_shader;
	ID3D11Buffer *vertex_buffer;
	ID3D11InputLayout *input_layout;
};



GraphicCookie::Core* GraphicCookie::Core::instance_ = nullptr;

GraphicCookie::Core* GraphicCookie::Core::getInstance()
{
	if (instance_) return instance_;

	return instance_ = new Core();
}

void GraphicCookie::Core::InitEngine()
{
	float width, height;
	window_->getWindowSize(&width, &height);
	//Create a struct with the definition of our swap chain (back buffers and stuff).
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swap_chain_desc.BufferCount = 1; // Number of back buffers desired.
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Color format.
	swap_chain_desc.BufferDesc.Width = width; // back buffer width
	swap_chain_desc.BufferDesc.Height = height; // back buffer height
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Specify the use of the back buffer in this case we want it to draw in the screen.
	swap_chain_desc.OutputWindow = window_->getHandler(); // Window where we want to draw stuff.
	swap_chain_desc.SampleDesc.Count = 4; // Number of multisample we want.
	swap_chain_desc.Windowed = true; // Just tell if the window is windowed or not.
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // This allows to switch from windowed to fullscreen and viceversa.

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, // Indicate the graphics adapter we want to use (NULL for the default one).
		D3D_DRIVER_TYPE_HARDWARE, // This tells how we want Directx to work, in our case by hardware.
		NULL, // If the last parameter were SOFTWARE, fill this one.
		NULL, // Flags to allow debugging for example.
		NULL, // Features required for our demo.
		NULL, // Number of features listed before.
		D3D11_SDK_VERSION, // Version of the directx sdk used in our game.
		&swap_chain_desc, // Pointer to the swap chain defined before.
		&core_data_->swap_chain, // Swap chain pointer to a pointer, meaning that is going to create the object for us.
		&core_data_->device, // Same as above but for the device
		NULL, // Feature levels if we put a variable here, we could be able to know what features we have available.
		&core_data_->device_context // Same as the swap chain and device.
	);

	ID3D11Texture2D* back_buffer; // Declare a 2D texture where we are going to render.
	core_data_->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
	core_data_->device->CreateRenderTargetView(back_buffer, NULL, &core_data_->main_back_buffer);
	back_buffer->Release();
	core_data_->device_context->OMSetRenderTargets(1, &core_data_->main_back_buffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	core_data_->device_context->RSSetViewports(1, &viewport);
}

GraphicCookie::Window* GraphicCookie::Core::getWindow()
{
	return window_.get();
}

void GraphicCookie::Core::ShutdownEngine()
{
	delete instance_;
}

GraphicCookie::Core::Core()
{
	core_data_ = std::unique_ptr<CoreData>(new CoreData());
	window_ = std::unique_ptr<GraphicCookie::Window>(new GraphicCookie::Window());
}

GraphicCookie::Core::~Core()
{
	core_data_->swap_chain->SetFullscreenState(false, NULL); // Go to windowed mode when closing.
	core_data_->swap_chain->Release();
	core_data_->main_back_buffer->Release();
	core_data_->device->Release();
	core_data_->device_context->Release();
}

int WINAPI WinMain(HINSTANCE app_instance, HINSTANCE previous_instance, LPSTR cmd_data, int number_parameters) {
	GraphicCookie::Core::getInstance()->Init();
	
	GraphicCookie::Window::CreateWindowGC(800, 600, app_instance, number_parameters);

	MSG message;

	while (true) {
		// We use PeekMessage because getmessage is blocking until receives a message, and PeekMessage is not blocking.
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message); //In case we got a keyboard message, translate it in a right way.
			DispatchMessage(&message); // Dispatch the message to the WinProc function.
			if (message.message == WM_QUIT) break; // If the message is WM_QUIT we break the main loop.
		}
		GraphicCookie::Core::getInstance()->Update();
		GraphicCookie::Core::getInstance()->Render();
		//Render();
	}

	//CleanDirectx();

	GraphicCookie::Core::getInstance()->ShutdownEngine();

	return 0;
}
