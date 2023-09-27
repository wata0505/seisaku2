#include "ToonShader.h"
#include "misc.h"
#include "Graphics.h"

using namespace DirectX;

ToonShader::ToonShader(ID3D11Device* device)
{
	// 頂点シェーダー
	{
		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			 {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	   { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BONES", 0,DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_UINT, 0,
			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		create_vs_from_cso(device, "Shader\\ToonVS.cso", vertexShader.ReleaseAndGetAddressOf(),
			inputLayout.ReleaseAndGetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
	}

	// ピクセルシェーダー
	{

		create_ps_from_cso(device, "Shader\\ToonPS.cso", pixelShader.GetAddressOf());
	}

	// 定数バッファ
	{
		// シーン用バッファ
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(constants);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		desc.ByteWidth = sizeof(UvConstants);
		 hr = device->CreateBuffer(&desc, 0, uvBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ラスタライザーステート
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = false;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_FRONT;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	
	// ブレンドステート
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, blendState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	
}

// 描画開始
void ToonShader::Begin(ID3D11DeviceContext* dc, const RenderContext& rc)
{

	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);
	Graphics& graphics = Graphics::Instance();
	
	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->OMSetBlendState(graphics.GetBlendState(BlendState::Alpha), blend_factor, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(graphics.GetDepthStencilState(DepthStencilState::ONON), 0);
	dc->RSSetState(rasterizerState.Get());
	dc->RSSetState(graphics.GetRasterizerState(RasterizerState::Solid));
}

// 描画
void ToonShader::Draw(ID3D11DeviceContext* dc, const Model* model, std::vector<SkinnedMeshResouurce::constants> buffer, const DirectX::XMFLOAT4 uvstatus, const DirectX::XMFLOAT4 color)
{
	Graphics& graphics = Graphics::Instance();
	const SkinnedMeshResouurce* resource = model->GetResource();
	buffers = buffer;
	int size = 0;
	for (const SkinnedMeshResouurce::Mesh& mesh : resource->GetMeshes())
	{

		SkinnedMeshResouurce::constants buffer = buffers.at(size);
		UvConstants uv;
		uint32_t stride{ sizeof(SkinnedMeshResouurce::Vertex) };
		uint32_t offset{ 0 };
		dc->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
		dc->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dc->IASetInputLayout(inputLayout.Get());

		for (const SkinnedMeshResouurce::Mesh::subset& subset : mesh.subsets)
		{
			// マテリアルの識別ID からマテリアルを取得し参照として設定
			const SkinnedMeshResouurce::Material& material{ resource->materials.find({subset.materialUniqueId}).operator*() };
			
			XMStoreFloat4(&buffer.materialColor, XMLoadFloat4(&color));
			//constantbuffer = modle-Getconstant_buffer();
			dc->UpdateSubresource(constantBuffer.Get(), 0, 0, &buffer, 0, 0);

			dc->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
			dc->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
			uv.uvStatus = uvstatus;
			dc->UpdateSubresource(uvBuffer.Get(), 0, 0, &uv, 0, 0);
			dc->PSSetConstantBuffers(2, 1, uvBuffer.GetAddressOf());
			//テクスチャ
			dc->PSSetShaderResources(0, 1, EffectTexAll::Instance().GetSprite(int(uvstatus.w))->GetShaderResourceView().GetAddressOf());
			//ディゾルブ
			dc->PSSetShaderResources(5, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::Dissolve)->GetShaderResourceView().GetAddressOf());
			dc->DrawIndexed(subset.indexCount, subset.startIndexLocation, 0);
		}
		size++;
	}
}

// 描画終了
void ToonShader::End(ID3D11DeviceContext* dc)
{
	dc->VSSetShader(nullptr, nullptr, 0);
	dc->PSSetShader(nullptr, nullptr, 0);
	dc->IASetInputLayout(nullptr);
	dc->RSSetState(Graphics::Instance().GetRasterizerState(RasterizerState::Solid));
}