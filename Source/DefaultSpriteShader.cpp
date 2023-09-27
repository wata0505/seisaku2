#include "Misc.h"
#include "DefaultSpriteShader.h"
#include  "Graphics.h"

DefaultSpriteShader::DefaultSpriteShader(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "DISSOLVE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	create_vs_from_cso(device, "Shader\\SpriteVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	create_ps_from_cso(device, "Shader\\SpritePS.cso", pixelShader.GetAddressOf());
	
}

// •`‰æŠJŽn
void DefaultSpriteShader::Begin(const RenderContext& rc)
{
	Graphics& graphics = Graphics::Instance();
	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	rc.deviceContext->IASetInputLayout(inputLayout.Get());

	rc.deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	rc.deviceContext->OMSetBlendState(graphics.GetBlendState(BlendState::Alpha), blend_factor, 0xFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(graphics.GetDepthStencilState(DepthStencilState::ONON), 0);
	rc.deviceContext->RSSetState(graphics.GetRasterizerState(RasterizerState::Solid));
}

// •`‰æ
void DefaultSpriteShader::Draw(const RenderContext& rc, const Sprite* sprite)
{
	Graphics& graphics = Graphics::Instance();
	UINT stride = sizeof(Sprite::Vertex);
	UINT offset = 0;
	rc.deviceContext->IASetVertexBuffers(0, 1, sprite->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	rc.deviceContext->PSSetShaderResources(0, 1, sprite->GetShaderResourceView().GetAddressOf());
	//ƒfƒBƒ]ƒ‹ƒu
	rc.deviceContext->PSSetShaderResources(5, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::Dissolve)->GetShaderResourceView().GetAddressOf());
	//ƒfƒBƒ]ƒ‹ƒu ‰~
	rc.deviceContext->PSSetShaderResources(6, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::DissolveCircle)->GetShaderResourceView().GetAddressOf());
	//ƒfƒBƒ]ƒ‹ƒu ‰~
	rc.deviceContext->PSSetShaderResources(7, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::DissolveDiagonal)->GetShaderResourceView().GetAddressOf());
	rc.deviceContext->Draw(4, 0);

}

// •`‰æI—¹
void DefaultSpriteShader::End(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->PSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->IASetInputLayout(nullptr);
}
