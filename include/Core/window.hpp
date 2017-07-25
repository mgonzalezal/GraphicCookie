#pragma once

#include <Windows.h>

namespace GraphicCookie {
	class Window {
	 public:
		 Window();
		 ~Window();

		 static void CreateWindowGC(float width, float height, HINSTANCE app_instance, int number_parameters);

		 void getWindowSize(float *width, float *height);
		 HWND getHandler();
	 private:
		 HWND window_instance_;
		 static LRESULT CALLBACK WindowProc(HWND window_handler, UINT message, WPARAM w_param, LPARAM l_param);

		 float width_, height_;
	};
}