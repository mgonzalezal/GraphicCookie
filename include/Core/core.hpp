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
	 public:
		static Core* getInstance();
		static void ShutdownEngineStatic();

		Window* getWindow();
		void ShutdownEngine();
		void InitUser();
		void UpdateUser();
		void RenderUser();
		void RenderCore();



		bool IsRunning();

		ID3D11Device& getDevice();
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
		ID3D11DepthStencilView *depth_stencil_view;
		bool is_running_;
	};
}