#include "Core/core.hpp"
#include "Core/Texture.hpp"

GraphicCookie::Texture::Texture()
{
}

GraphicCookie::Texture::~Texture()
{
	if (texture_) {
		texture_->Release();
		texture_ = 0;
	}
}

void GraphicCookie::Texture::LoadTexture(char * path_to_texture)
{
	HRESULT result;

	ID3D11Device& device = GraphicCookie::Core::getInstance()->getDevice();

	result = D3DX11CreateShaderResourceViewFromFile(&device, path_to_texture, NULL, NULL, &texture_, NULL);
	if(FAILED(result)) {
		MessageBox(NULL, "Error loading texture", "ERROR", MB_OK);
		Core::ShutdownEngineStatic();
		return;
	}

}

void GraphicCookie::Texture::UseTexture()
{
	Core::getInstance()->getDeviceContext().PSSetShaderResources(0, 1, &texture_);
}

void GraphicCookie::Texture::DeuseTexture()
{
	ID3D11ShaderResourceView* nullSRV = nullptr;
	Core::getInstance()->getDeviceContext().PSSetShaderResources(0, 1, &nullSRV);
}

ID3D11ShaderResourceView * GraphicCookie::Texture::GetTexture()
{
	return texture_;
}
