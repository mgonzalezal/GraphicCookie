#include "Core\core.hpp"
#include "Core\window.hpp"
#include "Core\object.hpp"
#include "Core\texture.hpp"

std::unique_ptr<GraphicCookie::Object> cube, cube2, floor_object;
std::unique_ptr<GraphicCookie::Texture> texture, floor_texture;

void GraphicCookie::Core::InitUser() {
	cube = std::unique_ptr<GraphicCookie::Object>(new GraphicCookie::Object());
	cube->Load(ObjectType::ObjectType_Obj, "./data/cube.obj");
	cube->Init();
	float pos[3] = {-5.0f,1.0f, 10.0f };
	cube->SetPosition(pos);

	cube2 = std::unique_ptr<GraphicCookie::Object>(new GraphicCookie::Object());
	cube2->Load(ObjectType::ObjectType_Cube);
	cube2->Init();
	float pos2[3] = { 5.0f,1.0f, 10.0f };
	cube2->SetPosition(pos2);

	floor_object = std::unique_ptr<GraphicCookie::Object>(new GraphicCookie::Object());
	floor_object->Load(ObjectType::ObjectType_Cube);
	floor_object->Init();
	float pos3[3] = { 0.0f, -5.0f, 10.0f };
	floor_object->SetPosition(pos3);
	float scale[3] = { 10.0f,1.0f,10.0f };
	floor_object->SetScale(scale);

	texture = std::unique_ptr<GraphicCookie::Texture>(new GraphicCookie::Texture());
	texture->LoadTexture("./data/cookietexture.jpg");

	floor_texture = std::unique_ptr<GraphicCookie::Texture>(new GraphicCookie::Texture());
	floor_texture->LoadTexture("./data/floor.jpg");	
}

void GraphicCookie::Core::UpdateUser() {

}

void GraphicCookie::Core::RenderUser() {
	texture->UseTexture();
	cube->Render();
	texture->DeuseTexture();

	cube2->Render();

	floor_texture->UseTexture();
	floor_object->Render();
}