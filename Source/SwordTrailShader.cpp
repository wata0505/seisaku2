#include "Misc.h"
#include "SwordTrailShader.h"
#include "Graphics.h"

SwordTrailShader::SwordTrailShader(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	create_vs_from_cso(device, "Shader\\SwordTrailVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	create_ps_from_cso(device, "Shader\\SwordTrailPS.cso", pixelShader.GetAddressOf());
	// 定数バッファ
	{
		// シーン用バッファ
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(scene_constants);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		desc.ByteWidth = sizeof(constants);
	}


}

// 描画開始
void SwordTrailShader::Begin(const RenderContext& rc)
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

// 描画
void SwordTrailShader::Draw(const RenderContext& rc, const SwordTrail* sprite, DirectX::XMFLOAT4 color)
{
	Graphics& graphics = Graphics::Instance();
	// 頂点バッファの内容の編集を開始する。
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	HRESULT hr = rc.deviceContext->Map(sprite->GetVertexBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	SwordTrailShader::constants buffer;
	XMStoreFloat4(&buffer.materialColor, XMLoadFloat4(&color));
	//constantbuffer = modle-Getconstant_buffer();
	rc.deviceContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &buffer, 0, 0);
	rc.deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	rc.deviceContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
	// pDataを編集することで頂点データの内容を書き換えることができる。
	SwordTrailVertex* v = static_cast<SwordTrailVertex*>(mappedBuffer.pData);
	for (size_t i = 0; i < sprite->vertex.size(); i++)
	{
		v[i].position = sprite->vertex.at(i).position;
		v[i].texcoord = sprite->vertex.at(i).texcoord;
		//位置情報が入力されなくなったら描画しない
		if (sprite->vertex.at(i).position.x == 0 && sprite->vertex.at(i).position.y == 0 && sprite->vertex.at(i).position.z == 0) {
			return;
		}

	}
	rc.deviceContext->Unmap(sprite->GetVertexBuffer().Get(), 0);
	UINT stride = sizeof(SwordTrailVertex);
	UINT offset = 0;
	rc.deviceContext->IASetVertexBuffers(0, 1, sprite->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	rc.deviceContext->PSSetShaderResources(0, 1,EffectTexAll::Instance().GetSprite(int(EffectTexAll::EfTexAll::Sumi))->GetShaderResourceView().GetAddressOf());

	//ディゾルブ
	rc.deviceContext->PSSetShaderResources(5, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::Dissolve)->GetShaderResourceView().GetAddressOf());
	rc.deviceContext->Draw(42, 0);
}

// 描画終了
void SwordTrailShader::End(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->PSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->IASetInputLayout(nullptr);
}