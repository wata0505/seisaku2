#include "Misc.h"
#include "InstancingSpriteShader.h"
#include  "Graphics.h"
#include "InstancingSpriteManager.h"
#include "Camera.h"

InstancingSpriteShader::InstancingSpriteShader(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "DISSOLVE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "DTEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	create_vs_from_cso(device, "Shader\\InstancingVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	create_gs_from_cso(device, "Shader\\InstancingGS.cso", geometryShader.GetAddressOf());
	create_ps_from_cso(device, "Shader\\InstancingPS.cso", pixelShader.GetAddressOf());

	{
		//// シーン用バッファ

		D3D11_BUFFER_DESC cBufferDesc;
		cBufferDesc.Usage = D3D11_USAGE_DEFAULT;    // 動的(ダイナミック)使用法
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // 定数バッファ
		cBufferDesc.CPUAccessFlags = 0;     // CPUから書き込む
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;
		cBufferDesc.ByteWidth = sizeof(SceneConstants);

		HRESULT hr = device->CreateBuffer(&cBufferDesc, 0, sceneBuffers.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		cBufferDesc.ByteWidth = sizeof(InstancingSpriteConstants);

		 hr = device->CreateBuffer(&cBufferDesc, 0, constantBuffers.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	}

}

// 描画開始
void InstancingSpriteShader::Begin(const RenderContext& rc)
{
	Graphics& graphics = Graphics::Instance();
	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->GSSetShader(geometryShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	rc.deviceContext->IASetInputLayout(inputLayout.Get());
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);

	SceneConstants scene{};
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&Camera::Instance().GetProjection());
	P = XMMatrixTranspose(P);//転地行列へ
	scene.View = Camera::Instance().GetView();
	DirectX::XMStoreFloat4x4(&scene.Projection, P);

	// ビュー・プロジェクション行列と光の当たる方向をシーン定数バッファとして設定
	rc.deviceContext->UpdateSubresource(sceneBuffers.Get(), 0, 0, &scene, 0, 0);
	rc.deviceContext->VSSetConstantBuffers(0, 1, sceneBuffers.GetAddressOf());
	rc.deviceContext->PSSetConstantBuffers(0, 1, sceneBuffers.GetAddressOf());
	rc.deviceContext->GSSetConstantBuffers(0, 1, sceneBuffers.GetAddressOf());

	rc.deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	rc.deviceContext->OMSetBlendState(graphics.GetBlendState(BlendState::Alpha), blend_factor, 0xFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(graphics.GetDepthStencilState(DepthStencilState::ONOFF), 0);
	rc.deviceContext->RSSetState(graphics.GetRasterizerState(RasterizerState::Solid));
	//rc.deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
}

// 描画
void InstancingSpriteShader::Draw(const RenderContext& rc, const Sprite* sprite)
{
	Graphics& graphics = Graphics::Instance();
	InstancingSpriteManager& manager = InstancingSpriteManager::Instance();
	for (int i = 0; i < InstancingSprite::Max; i++) {
		if (manager.GetInstancingSpriteNo(0) == 0)continue;
		InstancingSpriteConstants buffer = manager.GetSpriteConstants(0);
		//UINT stride = sizeof(Sprite::Vertex);
		//UINT offset = 0;
		//rc.deviceContext->IASetVertexBuffers(0, 1, sprite->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		rc.deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		rc.deviceContext->UpdateSubresource(constantBuffers.Get(), 0, 0, &buffer, 0, 0);
		rc.deviceContext->VSSetConstantBuffers(1, 1, constantBuffers.GetAddressOf());
		rc.deviceContext->PSSetConstantBuffers(1, 1, constantBuffers.GetAddressOf());
		//テクスチャ
		rc.deviceContext->PSSetShaderResources(0, 1, manager.GetMoveSprite(i)->GetShaderResourceView().GetAddressOf());
		//カラーテクスチャ
		rc.deviceContext->PSSetShaderResources(1, 1, manager.GetTexSprite(i)->GetShaderResourceView().GetAddressOf());
		//ディゾルブ
		rc.deviceContext->PSSetShaderResources(5, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::Dissolve)->GetShaderResourceView().GetAddressOf());
		//ディゾルブ 円
		rc.deviceContext->PSSetShaderResources(6, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::DissolveCircle)->GetShaderResourceView().GetAddressOf());
		rc.deviceContext->DrawIndexedInstanced(1, manager.GetInstancingSpriteNo(0), 0, 0, 0);
	}
}

// 描画終了
void InstancingSpriteShader::End(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->GSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->PSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->IASetInputLayout(nullptr);
}