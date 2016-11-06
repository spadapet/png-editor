struct TextureVertex
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
};

struct TexturePixel
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};
