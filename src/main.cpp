#include "Windows.h"

#include "Core\core.hpp"
#include "Core\window.hpp"
#include "Core\object.hpp"
#include "Core\texture.hpp"

std::unique_ptr<GraphicCookie::Object> cube, cube2;
std::unique_ptr<GraphicCookie::Texture> texture;

void GraphicCookie::Core::InitUser() {
	cube = std::unique_ptr<GraphicCookie::Object>(new GraphicCookie::Object());
	cube->Load(ObjectType::ObjectType_Triangle);
	cube->Init();
	float pos[3] = { 0.0f,0.0f, 10.0f };
	cube->SetPosition(pos);

	cube2 = std::unique_ptr<GraphicCookie::Object>(new GraphicCookie::Object());
	cube2->Load(ObjectType::ObjectType_Triangle);
	cube2->Init();
	float pos2[3] = { 1.0f,0.0f, 5.0f };
	cube2->SetPosition(pos2);

	texture = std::unique_ptr<GraphicCookie::Texture>(new GraphicCookie::Texture());
	texture->LoadTexture("./data/cookietexture.jpg");
}

void GraphicCookie::Core::UpdateUser() {

}

void GraphicCookie::Core::RenderUser() {
	texture->UseTexture();
	cube->Render();
	texture->UseTexture();
	cube2->Render();
}