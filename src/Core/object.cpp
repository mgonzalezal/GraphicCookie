#include "Core\object.hpp"
#include "Core\core.hpp"

GraphicCookie::Object::Object(){

}

GraphicCookie::Object::~Object() {

}

void GraphicCookie::Object::Init() {
	Compile();
}

void GraphicCookie::Object::Update() {

}

void GraphicCookie::Object::Render() {
	Core::getInstance()->getDeviceContext().IASetInputLayout(input_layout_);

	unsigned int stride = sizeof(VertexInfo);
	unsigned int offset = 0;

	Core::getInstance()->getDeviceContext().IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);
	Core::getInstance()->getDeviceContext().IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);
	Core::getInstance()->getDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Core::getInstance()->getDeviceContext().VSSetShader(vertex_shader_, 0, 0);
	Core::getInstance()->getDeviceContext().PSSetShader(pixel_shader_, 0, 0);
	Core::getInstance()->getDeviceContext().DrawIndexed(index_info_.size(), 0, 0);
}

void GraphicCookie::Object::Load(ObjectType object_type) {
	switch (object_type)
	{
		case ObjectType_Triangle: {
			vertex_info_.push_back({ -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f });
			vertex_info_.push_back({ 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f });
			vertex_info_.push_back({ 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f });

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
			index_info_.push_back(3);
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