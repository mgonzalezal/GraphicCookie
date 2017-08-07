#include "Core\object.hpp"
#include "Core\core.hpp"
#include "Core\window.hpp"
#include <time.h>
#include "tiny_obj_loader.h"

#define TINYOBJLOADER_IMPLEMENTATION

GraphicCookie::Object::Object(){
	scale_ = { 1.0f,1.0f,1.0f };
	position_ = { 0.0f,0.0f,0.0f };
	rotation_ = { 0.0f,0.0f,0.0f };
}

GraphicCookie::Object::~Object() {
	if(vertex_shader_) vertex_shader_->Release();
	if (pixel_shader_) pixel_shader_->Release();
	if (input_layout_) input_layout_->Release();
	if (vertex_buffer_) vertex_buffer_->Release();
	if (index_buffer_) index_buffer_->Release();
	if (matrix_buffer_) matrix_buffer_->Release();
}

void GraphicCookie::Object::Init() {
	Compile();
}

void GraphicCookie::Object::Update() {

}

void GraphicCookie::Object::Render() {
	float width, height;
	GraphicCookie::Core::getInstance()->getWindow()->getWindowSize(&width, &height);

	D3DXVECTOR3 camera_position = { 0.0, 0.0, -10.0 };
	D3DXVECTOR3 camera_look_at = { 0.0, 0.0, 0.0 };
	D3DXVECTOR3 camera_up = {0.0f, 1.0f, 0.0f};
	D3DXMATRIX model, view, projection;

	D3DXMATRIX scale, rotate, translate, result, tmp;

	D3DXMatrixScaling(&scale, scale_.x, scale_.y, scale_.z);
	D3DXMatrixRotationYawPitchRoll(&rotate, rotation_.x, rotation_.y, rotation_.z);
	D3DXMatrixTranslation(&translate, position_.x, position_.y, position_.z);
	D3DXMatrixMultiply(&tmp, &scale, &rotate);
	D3DXMatrixMultiply(&result, &tmp, &translate);

	D3DXMatrixTranspose(&matrix_data_.model, &result);

	D3DXMatrixLookAtLH(&view, &camera_position, &camera_look_at, &camera_up);
	D3DXMatrixTranspose(&matrix_data_.view, &view);

	D3DXMatrixPerspectiveFovLH(&projection, D3DXToRadian(45.0f), width/height, 0.01f, 100.0f);
	D3DXMatrixTranspose(&matrix_data_.projection, &projection);

	D3D11_MAPPED_SUBRESOURCE new_matrices;
	ZeroMemory(&new_matrices, sizeof(D3D11_MAPPED_SUBRESOURCE));
	Core::getInstance()->getDeviceContext().Map(matrix_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &new_matrices);
	memcpy(new_matrices.pData, &matrix_data_, sizeof(matrix_data_));
	Core::getInstance()->getDeviceContext().Unmap(matrix_buffer_, 0);

	unsigned int stride = sizeof(VertexInfo);
	unsigned int offset = 0;

	Core::getInstance()->getDeviceContext().IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);
	Core::getInstance()->getDeviceContext().IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
	Core::getInstance()->getDeviceContext().IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Core::getInstance()->getDeviceContext().VSSetShader(vertex_shader_, 0, 0);
	Core::getInstance()->getDeviceContext().PSSetShader(pixel_shader_, 0, 0);
	Core::getInstance()->getDeviceContext().VSSetConstantBuffers(0, 1, &matrix_buffer_);
	Core::getInstance()->getDeviceContext().IASetInputLayout(input_layout_);
	Core::getInstance()->getDeviceContext().DrawIndexed(index_info_.size(), 0, 0);
}

void GraphicCookie::Object::Load(ObjectType object_type, std::string obj_path) {
	switch (object_type)
	{
		case ObjectType_Triangle: {
			vertex_info_.push_back({ {-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f} });
			vertex_info_.push_back({ {0.0f, -1.0f, 0.0f }, {0.5f, 0.0f} });
			vertex_info_.push_back({ {1.0f, 1.0f, 0.0f }, {1.0f, 1.0f}});

			index_info_.push_back(0);
			index_info_.push_back(1);
			index_info_.push_back(2);
			break;
		}
		case ObjectType_Quad: {
			vertex_info_.push_back({ {-1.0f, -1.0f, 0.0f}, { 0.0f, 1.0f } });
			vertex_info_.push_back({ { 1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f} });
			vertex_info_.push_back({ { -1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } });
			vertex_info_.push_back({ { 1.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } });

			index_info_.push_back(0);
			index_info_.push_back(1);
			index_info_.push_back(2);
			index_info_.push_back(2);
			index_info_.push_back(1);
			index_info_.push_back(3);
			break;
		}
		case ObjectType_Cube: {
			//FRONT FACE - 0
			vertex_info_.push_back({ { -1.0f, 1.0f, -1.0f },{ 0.0f , 0.0f } });
			vertex_info_.push_back({ { 1.0f, 1.0f, -1.0f },{ 1.0f , 0.0f } });
			vertex_info_.push_back({ { -1.0f, -1.0f, -1.0f },{ 0.0f , 1.0f } });
			vertex_info_.push_back({ { 1.0f, -1.0f, -1.0f },{ 1.0f , 1.0f } });

			index_info_.push_back(0);
			index_info_.push_back(2);
			index_info_.push_back(3);
			index_info_.push_back(0);
			index_info_.push_back(3);
			index_info_.push_back(1);

			//BACK FACE - 4
			vertex_info_.push_back({ { -1.0f, 1.0f, 1.0f },{ 1.0f , 0.0f } });
			vertex_info_.push_back({ { 1.0f, 1.0f, 1.0f },{ 0.0f , 0.0f } });
			vertex_info_.push_back({ { -1.0f, -1.0f, 1.0f },{ 1.0f , 1.0f } });
			vertex_info_.push_back({ { 1.0f, -1.0f, 1.0f },{ 0.0f , 1.0f } });

			index_info_.push_back(5);
			index_info_.push_back(7);
			index_info_.push_back(6);
			index_info_.push_back(5);
			index_info_.push_back(6);
			index_info_.push_back(4);

			//LEFT FACE - 8
			vertex_info_.push_back({ { -1.0f, 1.0f, -1.0f },{ 1.0f , 0.0f } });
			vertex_info_.push_back({ { -1.0f, -1.0f, -1.0f },{ 1.0f , 1.0f } });
			vertex_info_.push_back({ { -1.0f, 1.0f, 1.0f },{ 0.0f , 0.0f } });
			vertex_info_.push_back({ { -1.0f, -1.0f, 1.0f },{ 0.0f ,1.0f } });

			index_info_.push_back(10);
			index_info_.push_back(11);
			index_info_.push_back(9);
			index_info_.push_back(9);
			index_info_.push_back(8);
			index_info_.push_back(10);

			//RIGHT FACE - 12
			vertex_info_.push_back({ { 1.0f, 1.0f, -1.0f },{ 0.0f , 0.0f } });
			vertex_info_.push_back({ { 1.0f, -1.0f, -1.0f },{ 0.0f , 1.0f } });
			vertex_info_.push_back({ { 1.0f, 1.0f, 1.0f },{ 1.0f , 0.0f } });
			vertex_info_.push_back({ { 1.0f, -1.0f, 1.0f },{ 1.0f , 1.0f } });

			index_info_.push_back(12);
			index_info_.push_back(13);
			index_info_.push_back(15);
			index_info_.push_back(12);
			index_info_.push_back(15);
			index_info_.push_back(14);

			//BOTTOM FACE - 16
			vertex_info_.push_back({ { -1.0f, -1.0f, -1.0f },{ 0.0f , 0.0f } });
			vertex_info_.push_back({ { -1.0f, -1.0f, 1.0f },{ 0.0f , 1.0f } });
			vertex_info_.push_back({ { 1.0f, -1.0f, -1.0f },{ 1.0f , 0.0f } });
			vertex_info_.push_back({ { 1.0f, -1.0f, 1.0f },{ 1.0f , 1.0f } });

			index_info_.push_back(16);
			index_info_.push_back(17);
			index_info_.push_back(19);
			index_info_.push_back(16);
			index_info_.push_back(19);
			index_info_.push_back(18);

			//TOP FACE - 20
			vertex_info_.push_back({ { -1.0f, 1.0f, 1.0f },{ 0.0f , 0.0f } });
			vertex_info_.push_back({ { -1.0f, 1.0f, -1.0f },{ 0.0f , 1.0f } });
			vertex_info_.push_back({ { 1.0f, 1.0f, 1.0f },{ 1.0f , 0.0f } });
			vertex_info_.push_back({ { 1.0f, 1.0f, -1.0f },{ 1.0f , 1.0f } });

			index_info_.push_back(20);
			index_info_.push_back(21);
			index_info_.push_back(23);
			index_info_.push_back(20);
			index_info_.push_back(23);
			index_info_.push_back(22);
			break;
		}
		case ObjectType_Obj: {
			if (obj_path == "") {
				MessageBox(NULL, "Object path not valid", "ERROR", MB_OK);
				return;
			}

			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			tinyobj::attrib_t attribute;

			std::string error;
			bool success = tinyobj::LoadObj(&attribute, &shapes, &materials, &error, obj_path.c_str());
			if (!success) {
				MessageBox(NULL, error.c_str(), "ERROR", MB_OK);
				return;
			}
			int index_elements = 0;
			for (int i_s = 0; i_s < shapes.size(); i_s++)
			{
				size_t index_offset = 0;
				for (size_t i_f = 0; i_f < shapes[i_s].mesh.num_face_vertices.size(); i_f++)
				{
					int facevertices = shapes[i_s].mesh.num_face_vertices[i_f];
					for (int i_facevertices = 0; i_facevertices < facevertices; i_facevertices++)
					{
						tinyobj::index_t index = shapes[i_s].mesh.indices[index_offset + i_facevertices];
						tinyobj::real_t vx = attribute.vertices[3 * index.vertex_index + 0];
						tinyobj::real_t vy = attribute.vertices[3 * index.vertex_index + 1];
						tinyobj::real_t vz = attribute.vertices[3 * index.vertex_index + 2];


						tinyobj::real_t tx = 0;
						tinyobj::real_t ty = 0;
						if (index.texcoord_index >= 0) {
							tinyobj::real_t tx = attribute.texcoords[2 * index.texcoord_index + 0];
							tinyobj::real_t ty = attribute.texcoords[2 * index.texcoord_index + 1];							
						}
						

						VertexInfo new_vertex;
						new_vertex.position = { vx,vy,vz };
						new_vertex.uv = { tx,ty };
						vertex_info_.push_back(new_vertex);

						index_info_.push_back(index_elements);
						index_elements++;
					}
					index_offset += facevertices;
				}
			}
		}
		default: {
			break;
		}
	}

	// Create the vertex buffer and upload the data.
	D3D11_BUFFER_DESC vertex_description;
	ZeroMemory(&vertex_description,sizeof(D3D11_BUFFER_DESC));

	vertex_description.Usage = D3D11_USAGE_DEFAULT;
	vertex_description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_description.ByteWidth = sizeof(VertexInfo) * vertex_info_.size();

	D3D11_SUBRESOURCE_DATA vertex_data;
	ZeroMemory(&vertex_data, sizeof(D3D11_SUBRESOURCE_DATA));
	vertex_data.pSysMem = &vertex_info_[0];

	HRESULT vertex_buffer_result = GraphicCookie::Core::getInstance()->getDevice().CreateBuffer(&vertex_description,&vertex_data,&vertex_buffer_);

	if (FAILED(vertex_buffer_result)) {
		MessageBox(NULL, "Vertex buffer not created", "ERROR", MB_OK);
		Core::ShutdownEngineStatic();
		return;
	}

	// Create the index buffer and upload the data.
	D3D11_BUFFER_DESC index_description;
	ZeroMemory(&index_description, sizeof(D3D11_BUFFER_DESC));

	index_description.Usage = D3D11_USAGE_DEFAULT;
	index_description.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_description.ByteWidth = sizeof(unsigned int) * index_info_.size();

	D3D11_SUBRESOURCE_DATA index_data;
	ZeroMemory(&index_data, sizeof(D3D11_SUBRESOURCE_DATA));
	index_data.pSysMem = &index_info_[0];

	HRESULT index_buffer_result = GraphicCookie::Core::getInstance()->getDevice().CreateBuffer(&index_description, &index_data, &index_buffer_);

	if (FAILED(index_buffer_result)) {
		MessageBox(NULL, "Index buffer not created", "ERROR", MB_OK);
		Core::ShutdownEngineStatic();
		return;
	}

	// Create the matrix buffer and upload data.
	D3D11_BUFFER_DESC matrix_description;
	ZeroMemory(&matrix_description, sizeof(D3D11_BUFFER_DESC));

	matrix_description.Usage = D3D11_USAGE_DYNAMIC;
	matrix_description.ByteWidth = sizeof(MatrixData);
	matrix_description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrix_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT matrix_buffer_result = GraphicCookie::Core::getInstance()->getDevice().CreateBuffer(&matrix_description, NULL, &matrix_buffer_);

	if (FAILED(index_buffer_result)) {
		MessageBox(NULL, "Index buffer not created", "ERROR", MB_OK);
		Core::ShutdownEngineStatic();
		return;
	}
}

void GraphicCookie::Object::SetPosition(float position[3])
{
	position_ = position;
}

void GraphicCookie::Object::SetScale(float scale[3])
{
	scale_ = scale;
}

void GraphicCookie::Object::SetRotation(float rotation[3])
{
	rotation_ = rotation;
}

void GraphicCookie::Object::Compile() {
	ID3D10Blob *vertex, *pixel;
	ID3D10Blob *vertex_error, *pixel_error;

	// Load shaders from a file and compile them.
	HRESULT vertex_result = D3DX11CompileFromFile("./data/shaders.shader", NULL, NULL, "VShader", "vs_4_0", 0, 0, 0, &vertex, &vertex_error, 0);

	if (vertex_result != S_OK) {
		char* error_message = static_cast<char*>(vertex_error->GetBufferPointer());
		MessageBox(NULL, error_message, "Error compiling vertex shader", MB_OK);
		Core::ShutdownEngineStatic();
		return;
	}

	HRESULT pixel_result = D3DX11CompileFromFile("./data/shaders.shader", NULL, NULL, "PShader", "ps_4_0", 0, 0, 0, &pixel, &pixel_error, 0);

	if (pixel_result != S_OK) {
		char* error_message = static_cast<char*>(pixel_error->GetBufferPointer());
		MessageBox(NULL, error_message, "Error compiling pixel shader", MB_OK);
		Core::ShutdownEngineStatic();
		return;
	}

	Core::getInstance()->getDevice().CreateVertexShader(vertex->GetBufferPointer(), vertex->GetBufferSize(), 0, &vertex_shader_);
	Core::getInstance()->getDevice().CreatePixelShader(pixel->GetBufferPointer(), pixel->GetBufferSize(),0, &pixel_shader_);

	D3D11_INPUT_ELEMENT_DESC layout_information[] {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	Core::getInstance()->getDevice().CreateInputLayout(layout_information, 2, vertex->GetBufferPointer(), vertex->GetBufferSize(), &input_layout_);

	vertex->Release();
	pixel->Release();
}