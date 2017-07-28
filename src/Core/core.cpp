#include "./Core/core.hpp"
#include "./Core/window.hpp"

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
	swap_chain_desc.SampleDesc.Count = 1; // Number of multisample we want.
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
		&swap_chain, // Swap chain pointer to a pointer, meaning that is going to create the object for us.
		&device, // Same as above but for the device
		NULL, // Feature levels if we put a variable here, we could be able to know what features we have available.
		&device_context // Same as the swap chain and device.
	);

	ID3D11Texture2D* back_buffer; // Declare a 2D texture where we are going to render.
	swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
	device->CreateRenderTargetView(back_buffer, NULL, &main_back_buffer);
	back_buffer->Release();

	ID3D11Texture2D* stencil_depth_buffer;
	D3D11_TEXTURE2D_DESC texture_stencil_depth_description;
	ZeroMemory(&texture_stencil_depth_description,sizeof(D3D11_TEXTURE2D_DESC));

	texture_stencil_depth_description.Width = width;
	texture_stencil_depth_description.Height = height;
	texture_stencil_depth_description.MipLevels = 1;
	texture_stencil_depth_description.ArraySize = 1;
	texture_stencil_depth_description.Format = DXGI_FORMAT_D32_FLOAT;
	texture_stencil_depth_description.SampleDesc.Count = 1;
	texture_stencil_depth_description.SampleDesc.Quality = 0;
	texture_stencil_depth_description.Usage = D3D11_USAGE_DEFAULT;
	texture_stencil_depth_description.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture_stencil_depth_description.CPUAccessFlags = 0;
	texture_stencil_depth_description.MiscFlags = 0;
	result = device->CreateTexture2D(&texture_stencil_depth_description, NULL, &stencil_depth_buffer);
	if (FAILED(result))
	{
		MessageBox(NULL, "Error creating texture", "Error", MB_OK);
	}

	D3D11_DEPTH_STENCIL_DESC depth_stencil_description;
	ZeroMemory(&depth_stencil_description, sizeof(D3D11_DEPTH_STENCIL_DESC));


	//Depth configuration
	depth_stencil_description.DepthEnable = true;
	depth_stencil_description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_description.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil configuration
	depth_stencil_description.StencilEnable = true;
	depth_stencil_description.StencilReadMask = 0xFF;
	depth_stencil_description.StencilWriteMask = 0xFF;

	// Stencil operations in case pixel is front
	depth_stencil_description.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_description.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations in case pixel is back
	depth_stencil_description.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_description.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_description.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_description.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	ID3D11DepthStencilState* depth_stencil_state;
	device->CreateDepthStencilState(&depth_stencil_description, &depth_stencil_state);
	device_context->OMSetDepthStencilState(depth_stencil_state, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
	ZeroMemory(&depth_stencil_view_desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depth_stencil_view_desc.Format = DXGI_FORMAT_D32_FLOAT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;

	device->CreateDepthStencilView(stencil_depth_buffer, &depth_stencil_view_desc, &depth_stencil_view);
	device_context->OMSetRenderTargets(1, &main_back_buffer, depth_stencil_view);
		
	ID3D11RasterizerState* rasterizer;
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizer_desc.FillMode = D3D11_FILL_SOLID; // Determines how we want to render a primitive, for example solid or wireframe.
	rasterizer_desc.CullMode = D3D11_CULL_BACK; // The faces that we want to cull or not.
	rasterizer_desc.FrontCounterClockwise = true; // If we want to render the triangles counter clock wise or not
	
	// TODO: Calculate this value when depth buffer created.
	rasterizer_desc.DepthBias = 0; 
	rasterizer_desc.DepthBiasClamp = 0.0f;
	rasterizer_desc.SlopeScaledDepthBias = 0.0f;
	rasterizer_desc.DepthClipEnable = true;
	//TODOEND

	rasterizer_desc.ScissorEnable = false; // If we have pixels outside a rectagle we will cull them (false until scissor activated)
	rasterizer_desc.MultisampleEnable = false;
	rasterizer_desc.AntialiasedLineEnable = false;

	result = device->CreateRasterizerState(&rasterizer_desc,&rasterizer);
	if (FAILED(result)) {
		MessageBox(NULL, "Rasterizer not created", "Warning", MB_OK);
	}

	device_context->RSSetState(rasterizer);

	//Create the viewport and set it.
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	device_context->RSSetViewports(1, &viewport);

	is_running_ = true;
}

GraphicCookie::Window* GraphicCookie::Core::getWindow()
{
	return window_.get();
}

void GraphicCookie::Core::ShutdownEngine()
{
	if (swap_chain) {
		swap_chain->SetFullscreenState(false, NULL); // Go to windowed mode when closing.
		swap_chain->Release();
	}
	
	if (main_back_buffer) {
		main_back_buffer->Release();
	}

	if (depth_stencil_view) {
		depth_stencil_view->Release();
	}

	if (device) {
		device->Release();
	}
	if (device_context) {
		device_context->Release();
	}

	is_running_ = false;
}

GraphicCookie::Core::Core()
{
	window_ = std::unique_ptr<GraphicCookie::Window>(new GraphicCookie::Window());
}

GraphicCookie::Core::~Core()
{

}

void GraphicCookie::Core::RenderCore() {
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	device_context->ClearRenderTargetView(main_back_buffer, clearColor);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
	RenderUser();
	swap_chain->Present(0, 0);
}

bool GraphicCookie::Core::IsRunning()
{
	return is_running_;
}

ID3D11Device& GraphicCookie::Core::getDevice()
{
	return *device;
}

ID3D11DeviceContext& GraphicCookie::Core::getDeviceContext() {
	return *device_context;
}

int WINAPI WinMain(HINSTANCE app_instance, HINSTANCE previous_instance, LPSTR cmd_data, int number_parameters) {	
	GraphicCookie::Core *instance = GraphicCookie::Core::getInstance();


	GraphicCookie::Window::CreateWindowGC(800, 600, number_parameters);

	instance->InitUser();

	MSG message;

	while (instance->IsRunning()) {
		// We use PeekMessage because getmessage is blocking until receives a message, and PeekMessage is not blocking.
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message); //In case we got a keyboard message, translate it in a right way.
			DispatchMessage(&message); // Dispatch the message to the WinProc function.
		}

		if (message.message == WM_QUIT) {
			GraphicCookie::Core::getInstance()->ShutdownEngine();
		}else{
			instance->UpdateUser();		
			instance->RenderCore();		
		}
	}

	return 0;
}
