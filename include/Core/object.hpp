#pragma once

#include <memory>
#include "D3DX11.h"
#include <vector>

enum ObjectType {
	ObjectType_Triangle,
	ObjectType_Quad
};

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
		void Load(ObjectType object_type);
	private:
		void Compile();
		ID3D11VertexShader* vertex_shader_;
		ID3D11PixelShader* pixel_shader_;
		ID3D11InputLayout* input_layout_;
		ID3D11Buffer* vertex_buffer_;
		ID3D11Buffer* index_buffer_;

		ObjectType object_type_;

		std::vector<VertexInfo> vertex_info_;
		std::vector<unsigned int> index_info_;
	};
}