//#include "misc.h"
//#include "InstancingModelShader.h"
//#include "Graphics.h"
//
//using namespace DirectX;
//
////inline XMFLOAT4 to_xmfloat4(const FbxDouble4& fbxdouble4)
////{
////	XMFLOAT4 xmfloat4;
////	xmfloat4.x = static_cast<float>(fbxdouble4[0]);
////	xmfloat4.y = static_cast<float>(fbxdouble4[1]);
////	xmfloat4.z = static_cast<float>(fbxdouble4[2]);
////	xmfloat4.w = static_cast<float>(fbxdouble4[3]);
////	return xmfloat4;
////}
//
//InstancingModelShader::InstancingModelShader(ID3D11Device* device)
//{
//	// 頂点シェーダー
//	{
//		// 入力レイアウト
//		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
//		{
//			 {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
//			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
//			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	   { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
//			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
//			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "WEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
//			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "BONES", 0,DXGI_FORMAT_R32G32B32A32_UINT, 0,
//			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_UINT, 0,
//			D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		};
//
//		create_vs_from_cso(device, "Shader\\InstancingModelVS.cso", vertexShader.ReleaseAndGetAddressOf(),
//			inputLayout.ReleaseAndGetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
//	}
//
//	// ピクセルシェーダー
//	{
//
//		create_ps_from_cso(device, "Shader\\InstancingModelPS.cso", pixelShader.GetAddressOf());
//	}
//
//	// 定数バッファ
//	{
//		// シーン用バッファ
//		D3D11_BUFFER_DESC desc;
//		::memset(&desc, 0, sizeof(desc));
//		desc.Usage = D3D11_USAGE_DEFAULT;
//		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//		desc.CPUAccessFlags = 0;
//		desc.MiscFlags = 0;
//		//desc.ByteWidth = sizeof(StageConstants);
//		desc.StructureByteStride = 0;
//
//		HRESULT hr = device->CreateBuffer(&desc, 0, constant_buffer.GetAddressOf());
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//		desc.ByteWidth = sizeof(ShadowConstants);
//		hr = device->CreateBuffer(&desc, 0, shadow_buffer.GetAddressOf());
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//		desc.ByteWidth = sizeof(UvConstants);
//		hr = device->CreateBuffer(&desc, 0, uv_buffer.GetAddressOf());
//		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//
//	}
//
//
//	//
//	//// サンプラステート
//	D3D11_SAMPLER_DESC desc;
//	::memset(&desc, 0, sizeof(desc));
//	desc.MipLODBias = 0.0f;
//	desc.MaxAnisotropy = 1;
//	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	desc.MinLOD = -FLT_MAX;
//	desc.MaxLOD = FLT_MAX;
//	// シャドウマップ用
//	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
//	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
//	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
//	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
//	desc.BorderColor[0] = FLT_MAX;
//	desc.BorderColor[1] = FLT_MAX;
//	desc.BorderColor[2] = FLT_MAX;
//	desc.BorderColor[3] = FLT_MAX;
//	HRESULT hr = device->CreateSamplerState(&desc, shadowMapSamplerState.GetAddressOf());
//	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
//}
//
//// 描画開始
//void InstancingModelShader::Begin(ID3D11DeviceContext* dc, const RenderContext& rc)
//{
//
//	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
//	dc->PSSetShader(pixelShader.Get(), nullptr, 0);
//	Graphics& graphics = Graphics::Instance();
//	scene_buffers = graphics.GetConstant_buffers(0);
//
//
//	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
//	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);
//
//	scene_constants data{};
//	// ビュー行列とプロジェクション行列を掛け合わせる(乗算)
//	XMStoreFloat4x4(&data.view_projection, V * P);
//	// 光の当たる方向を設定
//	//data.light_direction = { 0, 0, 1, 0 };
//	data.light_direction = rc.lightDirection;
//	//カメラ位置を設定
//	data.camera_position = rc.viewPosition;
//	// ビュー・プロジェクション行列と光の当たる方向をシーン定数バッファとして設定
//	dc->UpdateSubresource(scene_buffers.Get(), 0, 0, &data, 0, 0);
//	// 設定したシーン定数バッファを GPU 側に登録
//	dc->VSSetConstantBuffers(1, 1, scene_buffers.GetAddressOf());
//	//設定したシーン定数バッファをピクセルシェーダー側に登録
//	dc->PSSetConstantBuffers(1, 1, scene_buffers.GetAddressOf());
//	ShadowConstants shadow{};
//	shadow.shadow_projection = rc.shadowMapData.lightViewProjection;
//	dc->UpdateSubresource(shadow_buffer.Get(), 0, 0, &shadow, 0, 0);
//	dc->VSSetConstantBuffers(5, 1, shadow_buffer.GetAddressOf());
//	//設定したシーン定数バッファをピクセルシェーダー側に登録
//	dc->PSSetConstantBuffers(5, 1, shadow_buffer.GetAddressOf());
//	dc->PSSetSamplers(10, 1, shadowMapSamplerState.GetAddressOf());
//	//深度テスト：オン、深度ライト：オンの深度ステンシルステートを設定しておく
//	dc->OMSetDepthStencilState(graphics.GetDepthStencilState(ONON), 1);
//	dc->RSSetState(graphics.GetRasterizerState(RasterizerState::Solid));
//
//}

// 描画
//void InstancingModelShader::Draw(ID3D11DeviceContext* dc, const Model* model, StageConstants buffer,int max, const DirectX::XMFLOAT4 color)
//{
//	Graphics& graphics = Graphics::Instance();
//	const Skinned_meshResouurce* resource = model->GetResource();
//
//	for (const Skinned_meshResouurce::Mesh& mesh : resource->GetMeshes())
//	{
//
//		StageConstants buffers = buffer;
//		uint32_t stride{ sizeof(Skinned_meshResouurce::Vertex) };
//		uint32_t offset{ 0 };
//		dc->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
//		dc->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		dc->IASetInputLayout(inputLayout.Get());
//
//		for (const Skinned_meshResouurce::Mesh::subset& subset : mesh.subsets)
//		{
//			// マテリアルの識別ID からマテリアルを取得し参照として設定
//			const Skinned_meshResouurce::Material& material{ resource->materials.find({subset.material_unique_id}).operator*() };
//
//			//XMStoreFloat4(&buffer.material_color, XMLoadFloat4(&color));
//			//constantbuffer = modle-Getconstant_buffer();
//			dc->UpdateSubresource(constant_buffer.Get(), 0, 0, &buffer, 0, 0);
//			dc->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
//			dc->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
//			dc->PSSetShaderResources(0, 1, resource->shader_resource_views.at(material.texture_filenames[0]).GetAddressOf());
//			//法線マップ
//			dc->PSSetShaderResources(1, 1, resource->shader_resource_views.at(material.texture_filenames[1]).GetAddressOf());
//			dc->DrawIndexedInstanced(subset.index_count, max, subset.start_index_location, 0, 0);
//		}
//	}
//}


// 描画終了
//void InstancingModelShader::End(ID3D11DeviceContext* dc)
//{
//	dc->VSSetShader(nullptr, nullptr, 0);
//	dc->PSSetShader(nullptr, nullptr, 0);
//	dc->IASetInputLayout(nullptr);
//}