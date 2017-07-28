#pragma once

#include <Windows.h>
#include <string>

namespace GraphicCookie {
	class Window {
	 public:
		 Window();
		 ~Window();

		 static void CreateWindowGC(float width, float height, int number_parameters);

		 void getWindowSize(float *width, float *height);
		 HWND getHandler();
	 private:
		 HWND window_instance_;
		 WNDCLASSEX window_class_;
		 std::string window_name_;
		 HINSTANCE app_instance_;

		 static LRESULT CALLBACK WindowProc(HWND window_handler, UINT message, WPARAM w_param, LPARAM l_param);

		 float width_, height_;
	};
}