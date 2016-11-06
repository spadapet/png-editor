#include "Vertex.hlsli"

Texture2D globalTexture : register(t0);
SamplerState globalSampler : register(s0);

float4 main(TexturePixel input) : SV_TARGET
{
	float4 color = globalTexture.Sample(globalSampler, input.tex);

	if (color.a == 0)
	{
		discard;
	}

	return color;
}
