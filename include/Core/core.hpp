#pragma once 

#include <memory>
#include <Windows.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

namespace GraphicCookie {
	class Window;

	class Core {
		friend class Window;
		friend class Object;
	 public:
		static Core* getInstance();
		Window* getWindow();
		void ShutdownEngine();
		void InitUser();
		void UpdateUser();
		void RenderUser();

		void RenderCore();

		ID3D11DeviceContext& getDeviceContext();

	 private:
		Core();
		~Core();
		static Core* instance_;

		std::unique_ptr<Window> window_;

		void InitEngine();

		IDXGISwapChain *swap_chain;
		ID3D11Device *device;
		ID3D11DeviceContext *device_context;
		ID3D11RenderTargetView *main_back_buffer;
	};
}