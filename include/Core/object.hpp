#pragma once

#include <memory>
#include "D3DX11.h"
#include <vector>

struct VertexInfo {
	float x, y, z;
	float r, g, b, a;
};

namespace GraphicCookie {
	class Object {
	public:
		Object();
		~Object();

		void Init();
		void Update();
		void Render();
		void Load();
	private:
		void Compile();
		ID3D11VertexShader* vertex_shader_;
		ID3D11PixelShader* pixel_shader_;
		ID3D11InputLayout* input_layout_;
		ID3D11Buffer* vertex_buffer_;


		std::vector<VertexInfo> vertex_info_;
	};
}