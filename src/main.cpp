#include "Windows.h"

#include "Core\core.hpp"
#include "Core\window.hpp"
#include "Core\object.hpp"


/*
struct Vertex {
	float x, y, z;
	D3DXCOLOR color;
};

void InitPipeline() {
	ID3D10Blob *vertex_shader, *pixel_shader;

	// Load shaders from a file and compile them.
	D3DX11CompileFromFile("../data/shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &vertex_shader, 0, 0);
	D3DX11CompileFromFile("../data/shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &pixel_shader, 0, 0);

	//Create the shaders.
	g_device->CreateVertexShader(vertex_shader->GetBufferPointer(), vertex_shader->GetBufferSize(), NULL, &g_vertex_shader);
	g_device->CreatePixelShader(pixel_shader->GetBufferPointer(), pixel_shader->GetBufferSize(), NULL, &g_pixel_shader);

	//And activate them.
	g_device_context->VSSetShader(g_vertex_shader, 0, 0);
	g_device_context->PSSetShader(g_pixel_shader, 0, 0);

	// Specify the content of the vertex data.
	D3D11_INPUT_ELEMENT_DESC input_element_desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//Create the input layout with our data filled above.
	g_device->CreateInputLayout(input_element_desc, 2, vertex_shader->GetBufferPointer(), vertex_shader->GetBufferSize(), &g_input_layout);
	//And set it.
	g_device_context->IASetInputLayout(g_input_layout);
}

void InitGraphics() {
	Vertex triangle[] =
	{
		{ 0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
		{ 0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
		{ -0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
	};


	//Create the buffer description.
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));

	//Fill it with desired data
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC; // CPU can modify / GPU only read it.
	buffer_desc.ByteWidth = sizeof(Vertex) * 3; // Size of the buffer, as we want to draw a triangle multiply by 3.
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Specify the data of the buffer.
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // Allow the cpu to modify this buffer.

														 //Now create the buffer.
	g_device->CreateBuffer(&buffer_desc, NULL, &g_vertex_buffer);

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	g_device_context->Map(g_vertex_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped_subresource); // Map the resource.
	memcpy(mapped_subresource.pData, triangle, sizeof(triangle)); //Upload the data.
	g_device_context->Unmap(g_vertex_buffer, NULL); // Unmap it.
}

void Render() {
	g_device_context->ClearRenderTargetView(g_main_back_buffer, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_device_context->IASetVertexBuffers(0, 1, &g_vertex_buffer, &stride, &offset); // Set the buffers.
	g_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // Set the drawing mode.
	g_device_context->Draw(3, 0); // Draw it.

	g_swap_chain->Present(0, 0);
}*/

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