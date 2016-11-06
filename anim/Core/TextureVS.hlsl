#include "Vertex.hlsli"

cbuffer Globals : register(b0)
{
	matrix worldMatrix;
	matrix projectionMatrix;
};

TexturePixel main(TextureVertex input)
{
	TexturePixel output;

	float4 pos = float4(input.pos, 1);
	output.pos = mul(pos, worldMatrix);
	output.pos = mul(output.pos, projectionMatrix);
	output.tex = input.tex;

	return output;
}
