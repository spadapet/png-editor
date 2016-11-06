#pragma once

namespace anim
{
	struct TextureVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tex;

		static std::vector<D3D11_INPUT_ELEMENT_DESC> GetElements();
	};
}
