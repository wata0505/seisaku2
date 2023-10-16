#include "ParticleShader.h"
#include "misc.h"
#include "Graphics.h"
#include "EffectTexAll.h"

using namespace DirectX;

ParticleShader::ParticleShader(ID3D11Device* device)
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

		create_vs_from_cso(device, "Shader\\ParticleVS.cso", vertexShader.ReleaseAndGetAddressOf(),
			inputLayout.ReleaseAndGetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
	}

	// ピクセルシェーダー
	{

		create_ps_from_cso(device, "Shader\\ParticlePS.cso", pixelShader[static_cast<int>(ModelPSType::Toon)].GetAddressOf());
		create_ps_from_cso(device, "Shader\\DistortionPS.cso", pixelShader[static_cast<int>(ModelPSType::Distortion)].GetAddressOf());
		create_ps_from_cso(device, "Shader\\DefaultPS.cso", pixelShader[static_cast<int>(ModelPSType::Default)].GetAddressOf());
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
		desc.ByteWidth = sizeof(ParticleConstants);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	
}

// 描画開始
void ParticleShader::Begin(ID3D11DeviceContext* dc, const RenderContext& rc)
{

	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	
	Graphics& graphics = Graphics::Instance();

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->OMSetBlendState(graphics.GetBlendState(BlendState::Alpha), blend_factor, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(graphics.GetDepthStencilState(DepthStencilState::ONOFF), 0);
	dc->RSSetState(graphics.GetRasterizerState(RasterizerState::Solid));

}

// 描画
void ParticleShader::Draw(ID3D11DeviceContext* dc, const Model* model, ParticleConstants& buffers, int partialCount,int psType, int texType, const DirectX::XMFLOAT4 color)
{
	Graphics& graphics = Graphics::Instance();
	const SkinnedMeshResouurce* resource = model->GetResource();
	int instancingCount = partialCount;
	dc->PSSetShader(pixelShader[psType].Get(), nullptr, 0);
	if (!instancingCount)return;
	
		for (const SkinnedMeshResouurce::Mesh& mesh : resource->GetMeshes())
		{

			ParticleConstants buffer = buffers;
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
				dc->UpdateSubresource(constantBuffer.Get(), 0, 0, &buffer, 0, 0);
				dc->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
				dc->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
				dc->PSSetShaderResources(0, 1, resource->shaderResourceViews.at(material.textureFilenames[0]).GetAddressOf());
				dc->PSSetShaderResources(2, 1, EffectTexAll::Instance().GetSprite(texType).get()->GetShaderResourceView().GetAddressOf());
				//ディゾルブ
				dc->PSSetShaderResources(5, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::Dissolve)->GetShaderResourceView().GetAddressOf());
				//ディゾルブ 円
				dc->PSSetShaderResources(6, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::DissolveCircle)->GetShaderResourceView().GetAddressOf());
				dc->DrawIndexedInstanced(subset.indexCount, instancingCount, subset.startIndexLocation, 0, 0);
			}
		}
	
}

// 描画終了
void ParticleShader::End(ID3D11DeviceContext* dc)
{
	dc->VSSetShader(nullptr, nullptr, 0);
	dc->PSSetShader(nullptr, nullptr, 0);
	dc->IASetInputLayout(nullptr);
	dc->RSSetState(Graphics::Instance().GetRasterizerState(RasterizerState::Solid));
}