#pragma once 

#include <memory>
#include <Windows.h>


namespace GraphicCookie {
	class Window;

	class Core {
		friend class Window;
	 public:
		static Core* getInstance();
		Window* getWindow();
		void ShutdownEngine();
		void Init();
		void Update();
		void Render();

	 private:
		Core();
		~Core();
		static Core* instance_;

		std::unique_ptr<Window> window_;

		struct CoreData;
		std::unique_ptr<CoreData> core_data_;

		void InitEngine();
	};
}