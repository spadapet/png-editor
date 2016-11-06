#include "pch.h"
#include "Core/Shader.h"

std::vector<D3D11_INPUT_ELEMENT_DESC> anim::TextureVertex::GetElements()
{
	return std::vector<D3D11_INPUT_ELEMENT_DESC>
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
}
