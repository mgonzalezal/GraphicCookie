#include "Core\object.hpp"
#include "Core\core.hpp"
#include "Core\window.hpp"
#include <time.h>

GraphicCookie::Object::Object(){

}

GraphicCookie::Object::~Object() {
	vertex_shader_->Release();
	pixel_shader_->Release();
	input_layout_->Release();
	vertex_buffer_->Release();
	index_buffer_->Release();
	matrix_buffer_->Release();
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

	D3DXMatrixScaling(&scale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&rotate, 10.0f, clock() / 1000.0f, 10.0f);
	D3DXMatrixTranslation(&translate, 0.0f, 0.0f, 10.0f);
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

void GraphicCookie::Object::Load(ObjectType object_type) {
	switch (object_type)
	{
		case ObjectType_Triangle: {
			vertex_info_.push_back({ -2.0f, 2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f });
			vertex_info_.push_back({ 0.0f, -2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f });
			vertex_info_.push_back({ 2.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f });

			index_info_.push_back(0);
			index_info_.push_back(1);
			index_info_.push_back(2);
			break;
		}
		case ObjectType_Quad: {
			vertex_info_.push_back({ -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f });
			vertex_info_.push_back({ 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f });
			vertex_info_.push_back({ -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f });
			vertex_info_.push_back({ 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f });

			index_info_.push_back(0);
			index_info_.push_back(1);
			index_info_.push_back(2);
			index_info_.push_back(1);
			index_info_.push_back(2);
			index_info_.push_back(3);
			break;
		}
		case ObjectType_Cube: {
			vertex_info_.push_back({ -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f });
			vertex_info_.push_back({ 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f });
			vertex_info_.push_back({ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f });
			vertex_info_.push_back({ 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f });

			vertex_info_.push_back({ -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f });
			vertex_info_.push_back({ 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f });
			vertex_info_.push_back({ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f });
			vertex_info_.push_back({ 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f });

			//FRONT FACE
			index_info_.push_back(0);
			index_info_.push_back(2);
			index_info_.push_back(3);
			index_info_.push_back(0);
			index_info_.push_back(3);
			index_info_.push_back(1);

			//BACK FACE
			index_info_.push_back(5);
			index_info_.push_back(7);
			index_info_.push_back(6);
			index_info_.push_back(5);
			index_info_.push_back(6);
			index_info_.push_back(4);

			//LEFT FACE
			index_info_.push_back(4);
			index_info_.push_back(6);
			index_info_.push_back(2);
			index_info_.push_back(4);
			index_info_.push_back(2);
			index_info_.push_back(0);

			//RIGHT FACE
			index_info_.push_back(1);
			index_info_.push_back(3);
			index_info_.push_back(7);
			index_info_.push_back(1);
			index_info_.push_back(7);
			index_info_.push_back(5);

			//BOTTOM FACE
			index_info_.push_back(2);
			index_info_.push_back(6);
			index_info_.push_back(7);
			index_info_.push_back(2);
			index_info_.push_back(7);
			index_info_.push_back(3);

			//TOP FACE
			index_info_.push_back(4);
			index_info_.push_back(0);
			index_info_.push_back(1);
			index_info_.push_back(4);
			index_info_.push_back(1);
			index_info_.push_back(5);
			break;
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
		return;
	}
}

void GraphicCookie::Object::Compile() {
	ID3D10Blob *vertex, *pixel;
	ID3D10Blob *vertex_error, *pixel_error;

	// Load shaders from a file and compile them.
	HRESULT vertex_result = D3DX11CompileFromFile("./data/shaders.shader", NULL, NULL, "VShader", "vs_4_0", 0, 0, 0, &vertex, &vertex_error, 0);

	if (vertex_result != S_OK) {
		char* error_message = static_cast<char*>(vertex_error->GetBufferPointer());
		MessageBox(NULL, error_message, "Error compiling vertex shader", MB_OK);
	}

	HRESULT pixel_result = D3DX11CompileFromFile("./data/shaders.shader", NULL, NULL, "PShader", "ps_4_0", 0, 0, 0, &pixel, &pixel_error, 0);

	if (pixel_result != S_OK) {
		char* error_message = static_cast<char*>(pixel_error->GetBufferPointer());
		MessageBox(NULL, error_message, "Error compiling pixel shader", MB_OK);
	}

	Core::getInstance()->getDevice().CreateVertexShader(vertex->GetBufferPointer(), vertex->GetBufferSize(), 0, &vertex_shader_);
	Core::getInstance()->getDevice().CreatePixelShader(pixel->GetBufferPointer(), pixel->GetBufferSize(),0, &pixel_shader_);

	D3D11_INPUT_ELEMENT_DESC layout_information[] {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	Core::getInstance()->getDevice().CreateInputLayout(layout_information, 2, vertex->GetBufferPointer(), vertex->GetBufferSize(), &input_layout_);

	vertex->Release();
	pixel->Release();
}