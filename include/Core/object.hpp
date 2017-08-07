#pragma once

#include <memory>
#include "D3DX11.h"
#include "D3dx9math.h"
#include <vector>

enum ObjectType {
	ObjectType_Triangle,
	ObjectType_Quad,
	ObjectType_Cube,
	ObjectType_Obj
};

struct VertexInfo {
	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
};

struct MatrixData {
	D3DXMATRIX model;
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

namespace GraphicCookie {
	class Object {
	public:
		Object();
		~Object();

		void Init();
		void Update();
		void Render();
		void Load(ObjectType object_type, std::string obj_path = "");

		void SetPosition(float position[3]);
		void SetScale(float scale[3]);
		void SetRotation(float rotation[3]);
	private:
		void Compile();
		ID3D11VertexShader* vertex_shader_;
		ID3D11PixelShader* pixel_shader_;
		ID3D11InputLayout* input_layout_;
		ID3D11Buffer* vertex_buffer_;
		ID3D11Buffer* index_buffer_;
		ID3D11Buffer* matrix_buffer_;

		ObjectType object_type_;

		std::vector<VertexInfo> vertex_info_;
		std::vector<unsigned int> index_info_;
		MatrixData matrix_data_;

		D3DXVECTOR3 position_;
		D3DXVECTOR3 scale_;
		D3DXVECTOR3 rotation_;
	};
}