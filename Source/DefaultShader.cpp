#include "misc.h"
#include "DefaultShader.h"
#include "Graphics.h"

using namespace DirectX;

//inline XMFLOAT4 to_xmfloat4(const FbxDouble4& fbxdouble4)
//{
//	XMFLOAT4 xmfloat4;
//	xmfloat4.x = static_cast<float>(fbxdouble4[0]);
//	xmfloat4.y = static_cast<float>(fbxdouble4[1]);
//	xmfloat4.z = static_cast<float>(fbxdouble4[2]);
//	xmfloat4.w = static_cast<float>(fbxdouble4[3]);
//	return xmfloat4;
//}

DefaultShader::DefaultShader(ID3D11Device* device)
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

		create_vs_from_cso(device, "Shader\\SkinnedMeshVS.cso", vertexShader[ModelShader::Default ].ReleaseAndGetAddressOf(),
			inputLayout.ReleaseAndGetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
		create_vs_from_cso(device, "Shader\\InstancingModelVS.cso", vertexShader[ModelShader::Instancing].ReleaseAndGetAddressOf(),
			inputLayout.ReleaseAndGetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
	}

	// ピクセルシェーダー
	{
		
		create_ps_from_cso(device, "Shader\\SkinnedMeshPS.cso", pixelShader[ModelShader::Default].GetAddressOf());
		create_ps_from_cso(device, "Shader\\InstancingModelPS.cso", pixelShader[ModelShader::Instancing].GetAddressOf());
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
		desc.ByteWidth = sizeof(ShadowConstants);
		hr = device->CreateBuffer(&desc, 0, shadowBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		desc.ByteWidth = sizeof(UvConstants);
		hr = device->CreateBuffer(&desc, 0, uvBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// サブセット用バッファ
		desc.ByteWidth = sizeof(SubsetConstantBuffer);
		hr = device->CreateBuffer(&desc, 0, subsetConstantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		desc.ByteWidth = sizeof(InstancingConstants);
		hr = device->CreateBuffer(&desc, 0, instanBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// サンプラステート
	D3D11_SAMPLER_DESC desc;
	::memset(&desc, 0, sizeof(desc));
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = FLT_MAX;
	// シャドウマップ用
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	desc.BorderColor[0] = FLT_MAX;
	desc.BorderColor[1] = FLT_MAX;
	desc.BorderColor[2] = FLT_MAX;
	desc.BorderColor[3] = FLT_MAX;
	HRESULT hr = device->CreateSamplerState(&desc, shadowMapSamplerState.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// 輪郭線用の初期化処理
	//InitializeOutline(device);
}

// 描画開始
void DefaultShader::Begin(ID3D11DeviceContext* dc, const RenderContext& rc)
{
	dc->VSSetShader(vertexShader[ModelShader::Default].Get(), nullptr, 0);
	dc->PSSetShader(pixelShader[ModelShader::Default].Get(), nullptr, 0);
	Graphics& graphics = Graphics::Instance();
	sceneBuffers = graphics.GetConstant_buffers(0);

	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);

	scene_constants data{};
	// ビュー行列とプロジェクション行列を掛け合わせる(乗算)
	XMStoreFloat4x4(&data.viewProjection, V * P);
	XMStoreFloat4x4(&VP, V * P);
	// 光の当たる方向を設定
	data.lightDirection = rc.lightDirection;
	//カメラ位置を設定
	data.cameraPosition = rc.viewPosition;
	// ビュー・プロジェクション行列と光の当たる方向をシーン定数バッファとして設定
	dc->UpdateSubresource(sceneBuffers.Get(), 0, 0, &data, 0, 0);
	// 設定したシーン定数バッファを GPU 側に登録
    dc->VSSetConstantBuffers(1, 1, sceneBuffers.GetAddressOf());
	//設定したシーン定数バッファをピクセルシェーダー側に登録
	dc->PSSetConstantBuffers(1, 1, sceneBuffers.GetAddressOf());
	ShadowConstants shadow{};
	shadow.shadowProjection = rc.shadowMapData.lightViewProjection;
	dc->UpdateSubresource(shadowBuffer.Get(), 0, 0, &shadow, 0, 0);
	dc->VSSetConstantBuffers(5, 1, shadowBuffer.GetAddressOf());
	//設定したシーン定数バッファをピクセルシェーダー側に登録
	dc->PSSetConstantBuffers(5, 1, shadowBuffer.GetAddressOf());
	dc->PSSetSamplers(10, 1, shadowMapSamplerState.GetAddressOf());
	//深度テスト：オン、深度ライト：オンの深度ステンシルステートを設定しておく
	dc->OMSetDepthStencilState(graphics.GetDepthStencilState(ONON), 1);
	dc->RSSetState(graphics.GetRasterizerState(RasterizerState::Solid));

	// サブセット用定数バッファ
	dc->VSSetConstantBuffers(6, 1, subsetConstantBuffer.GetAddressOf());
	dc->PSSetConstantBuffers(6, 1, subsetConstantBuffer.GetAddressOf());

	// アウトライン用定数バッファ
	dc->VSSetConstantBuffers(7, 1, outlineConstantBuffer.GetAddressOf());
	dc->PSSetConstantBuffers(7, 1, outlineConstantBuffer.GetAddressOf());
}

// 描画
void DefaultShader::Draw(ID3D11DeviceContext* dc, Model* model, const DirectX::XMFLOAT4 uvstatus, const DirectX::XMFLOAT4 color)
{
	Graphics& graphics = Graphics::Instance();
	ID3D11VertexShader* vs[] =
	{
		vertexShader[ModelShader::Default].Get(),
		outlineVertexShader.Get(),
	};
	ID3D11PixelShader* ps[] =
	{
		pixelShader[ModelShader::Default].Get(),
		outlinePixelShader.Get(),
	};
	ID3D11InputLayout* il[] =
	{
		inputLayout.Get(),
		outlineInputLayout.Get(),
	};
	ID3D11RasterizerState* rs[] =
	{
		graphics.GetRasterizerState(RasterizerState::Solid),
		outlineRasterizerState.Get(),
	};
	
	const SkinnedMeshResouurce* resource = model->GetResource();
	
	for (int i = 0; i < 1; ++i)
	{
		dc->VSSetShader(vs[i], nullptr, 0);
		dc->PSSetShader(ps[i], nullptr, 0);
		dc->IASetInputLayout(il[i]);
		dc->RSSetState(rs[i]);
		int size = 0;
		//std::vector<SkinnedMeshResouurce::constants> buffers = buffer;
		for (const SkinnedMeshResouurce::Mesh& mesh : resource->GetMeshes())
		{
			SkinnedMeshResouurce::constants buffers = model->GetBufferData().at(size);
			UvConstants uv;
			UINT stride = sizeof(SkinnedMeshResouurce::Vertex);
			UINT offset = 0;
			dc->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
			dc->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			for (const SkinnedMeshResouurce::Mesh::subset& subset : mesh.subsets)
			{
				// マテリアルの識別ID からマテリアルを取得し参照として設定
				const SkinnedMeshResouurce::Material& material{ resource->materials.find({subset.materialUniqueId}).operator*() };
				//const SkinnedMeshResouurce::Material& material{ resource->materials.at(subset.materialUniqueId) };
				SubsetConstantBuffer cbSubset;
				cbSubset.adjustMetalness = material.pbr.adjustMetalness;
				cbSubset.adjustSmoothness = material.pbr.adjustSmoothness;
				cbSubset.emissiveStrength = material.pbr.emissiveStrength;
#if 0
				cbSubset.emissiveColor = subset.material->pbr.emissiveColor;
				cbSubset.hueShift = subset.material->pbr.hueShift;
				cbSubset.hemisphereLightData = renderContext.hemisphereLightData;
				//cbSubset.timer = subset.material->pbr.timer;
				cbSubset.timer = renderContext.timer / 60;
				cbSubset.scanTiling = subset.material->pbr.scanTiling;
				cbSubset.scanSpeed = subset.material->pbr.scanSpeed;
				cbSubset.scanBorder = subset.material->pbr.scanBorder;
				cbSubset.glowTiling = subset.material->pbr.glowTiling;
				cbSubset.glowSpeed = subset.material->pbr.glowSpeed;
				cbSubset.glowBorder = subset.material->pbr.glowBorder;
				cbSubset.glitchSpeed = subset.material->pbr.glitchSpeed;
				cbSubset.glitchIntensity = subset.material->pbr.glitchIntensity;
				//cbSubset.glitchIntensity = static_cast<float>(rand() / RAND_MAX) * 3.0f - 1.5f;
				cbSubset.glitchScale = subset.material->pbr.glitchScale;
				cbSubset.hologramBorder = subset.material->pbr.hologramBorder;
#endif
				dc->UpdateSubresource(subsetConstantBuffer.Get(), 0, 0, &cbSubset, 0, 0);
#if 0
				// アウトライン用定数バッファ更新
				OutlineConstantBuffer cbOutline;
				cbOutline.viewProjection = VP;
				cbOutline.outlineSize = material.pbr.outlineSize;
				cbOutline.outlineColor = material.pbr.outlineColor;
				//cbOutline.outlineColor.x = material.pbr.outlineColor.x;
				//cbOutline.outlineColor.y = material.pbr.outlineColor.y;
				//cbOutline.outlineColor.z = material.pbr.outlineColor.z;
				//cbOutline.outlineColor.w = 1.0f;
				//// alpha : 0.025 = 1.0 : 0.1 = 0.0
				//// size : 0.025 ~ 0.1
				//cbOutline.outlineColor.w = (1.0f - material.pbr.outlineSize * 10.0f);
				dc->UpdateSubresource(outlineConstantBuffer.Get(), 0, 0, &cbOutline, 0, 0);
#endif
				XMStoreFloat4(&buffers.materialColor, XMLoadFloat4(&color) * XMLoadFloat4(&material.Kd));
				dc->UpdateSubresource(constantBuffer.Get(), 0, 0, &buffers, 0, 0);
				dc->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
				dc->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
				uv.uvStatus = uvstatus;
				dc->UpdateSubresource(uvBuffer.Get(), 0, 0, &uv, 0, 0);
				dc->PSSetConstantBuffers(2, 1, uvBuffer.GetAddressOf());
				//テクスチャ
				dc->PSSetShaderResources(0, 1, resource->shaderResourceViews.at(material.textureFilenames[0]).GetAddressOf());
				//法線マップ
				dc->PSSetShaderResources(1, 1, resource->shaderResourceViews.at(material.textureFilenames[1]).GetAddressOf());
				// メタリックマップ
				dc->PSSetShaderResources(3, 1, resource->shaderResourceViews.at(material.textureFilenames[2]).GetAddressOf());
				// 粗さマップ
				dc->PSSetShaderResources(4, 1, resource->shaderResourceViews.at(material.textureFilenames[3]).GetAddressOf());
				// エミッシブマップ
				dc->PSSetShaderResources(7, 1, resource->shaderResourceViews.at(material.textureFilenames[4]).GetAddressOf());
				//ディゾルブ
				dc->PSSetShaderResources(5, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::Dissolve)->GetShaderResourceView().GetAddressOf());
				//ディゾルブ 円
				dc->PSSetShaderResources(6, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::DissolveCircle)->GetShaderResourceView().GetAddressOf());
				dc->DrawIndexed(subset.indexCount, subset.startIndexLocation, 0);
			}
			size++;
		}
	}
}

void DefaultShader::Draw(ID3D11DeviceContext* dc, const Model* model,InstancingConstants buffer, int max, const DirectX::XMFLOAT4 color)
{

	
	dc->VSSetShader(vertexShader[ModelShader::Instancing].Get(), nullptr, 0);
	dc->PSSetShader(pixelShader[ModelShader::Instancing].Get(), nullptr, 0);
	Graphics& graphics = Graphics::Instance();
	const SkinnedMeshResouurce* resource = model->GetResource();
	dc->OMSetDepthStencilState(graphics.GetDepthStencilState(ONON), 1);
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
			//const SkinnedMeshResouurce::Material& material{ resource->materials.at(subset.materialUniqueId) };

			//XMStoreFloat4(&buffer.materialColor, XMLoadFloat4(&color));
			//constantbuffer = modle-Getconstant_buffer();
			dc->UpdateSubresource(instanBuffer.Get(), 0, 0, &buffer, 0, 0);
			dc->VSSetConstantBuffers(0, 1, instanBuffer.GetAddressOf());
			dc->PSSetConstantBuffers(0, 1, instanBuffer.GetAddressOf());
			dc->PSSetShaderResources(0, 1, resource->shaderResourceViews.at(material.textureFilenames[0]).GetAddressOf());
			//法線マップ
			dc->PSSetShaderResources(1, 1, resource->shaderResourceViews.at(material.textureFilenames[1]).GetAddressOf());
			dc->DrawIndexedInstanced(subset.indexCount, max, subset.startIndexLocation, 0, 0);
		}
	}
}

// 描画終了
void DefaultShader::End(ID3D11DeviceContext* dc)
{
	dc->VSSetShader(nullptr, nullptr, 0);
	dc->PSSetShader(nullptr, nullptr, 0);
	dc->IASetInputLayout(nullptr);
}

#if 0
// 輪郭線用の初期化処理
void DefaultShader::InitializeOutline(ID3D11Device* device)
{
	// 頂点シェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\OutlineVS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// 頂点シェーダー生成
		HRESULT hr = device->CreateVertexShader(csoData.get(), csoSize, nullptr, outlineVertexShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONES",    0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), csoData.get(), csoSize, outlineInputLayout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ピクセルシェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\OutlinePS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// ピクセルシェーダー生成
		HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr, outlinePixelShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
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
		desc.ByteWidth = sizeof(OutlineConstantBuffer);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, outlineConstantBuffer.GetAddressOf());
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
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_FRONT;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, outlineRasterizerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}
#endif