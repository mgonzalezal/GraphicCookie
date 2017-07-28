cbuffer MatrixBuffer {
	matrix model_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};


struct VertexInput
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelInput VShader(VertexInput input)
{
	PixelInput output;

	output.position = mul(input.position, model_matrix);
	output.position = mul(output.position, view_matrix);
	output.position = mul(output.position, projection_matrix);
	//output.position = input.position;
	output.color = input.color;

	return output;
}


float4 PShader(PixelInput pixel_input) : SV_TARGET
{
	return pixel_input.color;
}