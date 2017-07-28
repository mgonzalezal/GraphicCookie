#include "Windows.h"

#include "Core\core.hpp"
#include "Core\window.hpp"
#include "Core\object.hpp"

std::unique_ptr<GraphicCookie::Object> cube;

void GraphicCookie::Core::InitUser() {
	cube = std::unique_ptr<GraphicCookie::Object>(new GraphicCookie::Object());
	cube->Load(ObjectType::ObjectType_Cube);
	cube->Init();
}

void GraphicCookie::Core::UpdateUser() {

}

void GraphicCookie::Core::RenderUser() {
	cube->Render();
}