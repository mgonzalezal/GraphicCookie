cbuffer MatrixBuffer {
	matrix model_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};


struct VertexInput
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PixelInput VShader(VertexInput input)
{
	PixelInput output;

	output.position = mul(input.position, model_matrix);
	output.position = mul(output.position, view_matrix);
	output.position = mul(output.position, projection_matrix);
	output.tex = input.tex;

	return output;
}

Texture2D texture_shader;
SamplerState sampler_type;

float4 PShader(PixelInput pixel_input) : SV_TARGET
{
	float texture_color;

	texture_color = texture_shader.Sample(sampler_type, pixel_input.tex);

	return texture_color;
}