#include "Windows.h"

#include "Core\core.hpp"
#include "Core\window.hpp"
#include "Core\object.hpp"

std::unique_ptr<GraphicCookie::Object> triangle;

void GraphicCookie::Core::InitUser() {
	triangle = std::unique_ptr<GraphicCookie::Object>(new GraphicCookie::Object());
	triangle->Load();
	triangle->Init();
}

void GraphicCookie::Core::UpdateUser() {

}

void GraphicCookie::Core::RenderUser() {
	triangle->Render();
}