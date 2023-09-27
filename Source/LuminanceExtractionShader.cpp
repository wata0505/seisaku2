#include "Misc.h"
#include "LuminanceExtractionShader.h"
#include  "Graphics.h"

LuminanceExtractionShader::LuminanceExtractionShader(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	create_vs_from_cso(device, "Shader\\LuminanceExtractionVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	create_ps_from_cso(device, "Shader\\LuminanceExtractionPS.cso", pixelShader.GetAddressOf());

	// 定数バッファ
	{
		// シーン用バッファ
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(CBLuminanceExtraction);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, luminanceExtractionConstantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	}

}

// 描画開始
void LuminanceExtractionShader::Begin(const RenderContext& rc)
{
	Graphics& graphics = Graphics::Instance();
	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	rc.deviceContext->IASetInputLayout(inputLayout.Get());

	rc.deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
	rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	ID3D11Buffer* constantBuffers[] =
	{
		luminanceExtractionConstantBuffer.Get(),
	};
	rc.deviceContext->VSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	rc.deviceContext->PSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);


	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	rc.deviceContext->OMSetBlendState(graphics.GetBlendState(BlendState::Alpha), blend_factor, 0xFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(graphics.GetDepthStencilState(DepthStencilState::ONON), 0);
	rc.deviceContext->RSSetState(graphics.GetRasterizerState(RasterizerState::Solid));
	//rc.deviceContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
}

// 描画
void LuminanceExtractionShader::Draw(const RenderContext& rc, const Sprite* sprite)
{
	CBLuminanceExtraction cbLuminanceExtraction;
	cbLuminanceExtraction.threshold = rc.luminanceExtractionData.threshold;
	cbLuminanceExtraction.intensity = rc.luminanceExtractionData.intensity;
	rc.deviceContext->UpdateSubresource(luminanceExtractionConstantBuffer.Get(), 0, 0, &cbLuminanceExtraction, 0, 0);

	rc.deviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	//luminanceExtractionSRV = rc.finalpassData.bloomTexture;
	rc.deviceContext->PSSetShaderResources(0, 1, sprite->GetShaderResourceView().GetAddressOf());
	rc.deviceContext->Draw(4, 0);

}

// 描画終了
void LuminanceExtractionShader::End(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->PSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->IASetInputLayout(nullptr);
}