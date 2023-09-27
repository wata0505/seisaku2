#include "Misc.h"
#include "SkyboxShader.h"
#include "Graphics.h"

SkyboxShader::SkyboxShader(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	create_vs_from_cso(device, "Shader\\SkyboxVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	create_ps_from_cso(device, "Shader\\SkyboxPS.cso", pixelShader.GetAddressOf());
	// 定数バッファ
	{
		// シーン用バッファ
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(CbScene);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, sceneConstantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	
}

// 描画開始
void SkyboxShader::Begin(const RenderContext& rc)
{
	Graphics& graphics = Graphics::Instance();
	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	rc.deviceContext->IASetInputLayout(inputLayout.Get());

	rc.deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	rc.deviceContext->OMSetBlendState(graphics.GetBlendState(BlendState::Alpha), blend_factor, 0xFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(graphics.GetDepthStencilState(DepthStencilState::OFFOFF), 0);
	rc.deviceContext->RSSetState(graphics.GetRasterizerState(RasterizerState::Solid));
	//rc.deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());

	ID3D11Buffer* constantBuffers[] =
	{
		sceneConstantBuffer.Get(),
	};
	rc.deviceContext->VSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	rc.deviceContext->PSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);

	CbScene cbScene;
	cbScene.viewPosition = rc.viewPosition;
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);
	// ビュープロジェクションの逆行列を利用する
	DirectX::XMStoreFloat4x4(&cbScene.inverseView, DirectX::XMMatrixInverse(nullptr, V));
	DirectX::XMStoreFloat4x4(&cbScene.inverseProjection, DirectX::XMMatrixInverse(nullptr, P));
	rc.deviceContext->UpdateSubresource(sceneConstantBuffer.Get(), 0, 0, &cbScene, 0, 0);

}

// 描画
void SkyboxShader::Draw(const RenderContext& rc, const Sprite* sprite)
{
	UINT stride = sizeof(Sprite::Vertex);
	UINT offset = 0;
	rc.deviceContext->IASetVertexBuffers(0, 1, sprite->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	rc.deviceContext->PSSetShaderResources(0, 1, sprite->GetShaderResourceView().GetAddressOf());
	rc.deviceContext->Draw(4, 0);
}

// 描画終了
void SkyboxShader::End(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->PSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->IASetInputLayout(nullptr);
}
