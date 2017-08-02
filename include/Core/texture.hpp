#pragma once

#include <d3d11.h>
#include <d3dx11tex.h>

namespace GraphicCookie {
	class Texture
	{
	public:
		Texture();
		~Texture();

		void LoadTexture(char* path_to_texture);
		void UseTexture();

		ID3D11ShaderResourceView* GetTexture();
	private:
		ID3D11ShaderResourceView* texture_;
	};
}