#include "misc.h"
#include "ShadowMap.h"
#include "Graphics.h"

using namespace DirectX;

inline XMFLOAT4 to_xmfloat4(const FbxDouble4& fbxdouble4)
{
	XMFLOAT4 xmfloat4;
	xmfloat4.x = static_cast<float>(fbxdouble4[0]);
	xmfloat4.y = static_cast<float>(fbxdouble4[1]);
	xmfloat4.z = static_cast<float>(fbxdouble4[2]);
	xmfloat4.w = static_cast<float>(fbxdouble4[3]);
	return xmfloat4;
}

ShadowMapShader::ShadowMapShader(ID3D11Device* device)
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

		create_vs_from_cso(device, "Shader\\ShadowMapVS.cso", vertexShader[Default].ReleaseAndGetAddressOf(),
			inputLayout.ReleaseAndGetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
		create_vs_from_cso(device, "Shader\\InstancingShadowMapVS.cso", vertexShader[Instancing].ReleaseAndGetAddressOf(),
			inputLayout.ReleaseAndGetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
	}

	// ピクセルシェーダー
	{

		create_ps_from_cso(device, "Shader\\ShadowMapPS.cso", pixelShader[Default].GetAddressOf());
		create_ps_from_cso(device, "Shader\\ShadowMapPS.cso", pixelShader[ModelShader::Instancing].GetAddressOf());
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
		desc.ByteWidth = sizeof(Constants);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		desc.ByteWidth = sizeof(ShadowConstants);
		hr = device->CreateBuffer(&desc, 0, shadow_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		desc.ByteWidth = sizeof(InstancingConstants);
		hr = device->CreateBuffer(&desc, 0, instan_buffer.GetAddressOf());
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
	//
	//// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	
		HRESULT hr = device->CreateDepthStencilState(&desc, depthStencilState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	//
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
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_FRONT;
		desc.AntialiasedLineEnable = false;
	
		HRESULT hr = device->CreateRasterizerState(&desc, rasterizerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	//
	
}

// 描画開始
void ShadowMapShader::Begin(ID3D11DeviceContext* dc, const RenderContext& rc)
{

	dc->VSSetShader(vertexShader[Default].Get(), nullptr, 0);
	dc->PSSetShader(pixelShader[Default].Get(), nullptr, 0);
	//render2D& render2d = render2D::Instance();
	Graphics& graphics = Graphics::Instance();
	
	//Camera& camera = Camera::Instance();
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);

	ShadowConstants data{};
	// ビュー行列とプロジェクション行列を掛け合わせる(乗算)
	XMStoreFloat4x4(&data.shadow_projection, V * P);
	dc->UpdateSubresource(shadow_buffer.Get(), 0, 0, &data, 0, 0);
	// 設定したシーン定数バッファを GPU 側に登録
	dc->VSSetConstantBuffers(5, 1, shadow_buffer.GetAddressOf());
	//設定したシーン定数バッファをピクセルシェーダー側に登録
	dc->PSSetConstantBuffers(5, 1, shadow_buffer.GetAddressOf());

	//深度テスト：オン、深度ライト：オンの深度ステンシルステートを設定しておく
	dc->OMSetDepthStencilState(depthStencilState.Get(), 1);
	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->OMSetBlendState(blendState.Get(), blend_factor, 0xFFFFFFFF);
	dc->RSSetState(rasterizerState.Get());


}

// 描画
void ShadowMapShader::Draw(ID3D11DeviceContext* dc, const Model* model, std::vector<SkinnedMeshResouurce::constants> buffer, const DirectX::XMFLOAT4 uvstatus, const DirectX::XMFLOAT4 color)
{
	const SkinnedMeshResouurce* resource = model->GetResource();
	std::vector<SkinnedMeshResouurce::constants> buffers = buffer;
	int size = 0;
	for (const SkinnedMeshResouurce::Mesh& mesh : resource->GetMeshes())
	{

		SkinnedMeshResouurce::constants buffer = buffers.at(size);
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

			XMStoreFloat4(&buffer.materialColor, XMLoadFloat4(&materialColor));
			//constantbuffer = modle-Getconstant_buffer();
			dc->UpdateSubresource(constant_buffer.Get(), 0, 0, &buffer, 0, 0);
			dc->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
			dc->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
			dc->DrawIndexed(subset.indexCount, subset.startIndexLocation, 0);
		}
		size++;
	}
}

void ShadowMapShader::Draw(ID3D11DeviceContext* dc, const Model* model, InstancingConstants buffer, int max, const DirectX::XMFLOAT4 color)
{
	dc->VSSetShader(vertexShader[ModelShader::Instancing].Get(), nullptr, 0);
	dc->PSSetShader(pixelShader[ModelShader::Instancing].Get(), nullptr, 0);
	Graphics& graphics = Graphics::Instance();
	const SkinnedMeshResouurce* resource = model->GetResource();

	for (const SkinnedMeshResouurce::Mesh& mesh : resource->GetMeshes())
	{

		InstancingConstants buffers = buffer;
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

			//XMStoreFloat4(&buffer.materialColor, XMLoadFloat4(&color));
			//constantbuffer = modle-Getconstant_buffer();
			dc->UpdateSubresource(instan_buffer.Get(), 0, 0, &buffer, 0, 0);
			dc->VSSetConstantBuffers(0, 1, instan_buffer.GetAddressOf());
			dc->PSSetConstantBuffers(0, 1, instan_buffer.GetAddressOf());
			
			dc->DrawIndexedInstanced(subset.indexCount, max, subset.startIndexLocation, 0, 0);
		}
	}
}

// 描画終了
void ShadowMapShader::End(ID3D11DeviceContext* dc)
{

	
	dc->VSSetShader(nullptr, nullptr, 0);
	dc->PSSetShader(nullptr, nullptr, 0);
	dc->IASetInputLayout(nullptr);
}
