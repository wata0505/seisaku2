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
	// ���_�V�F�[�_�[
	{
		// ���̓��C�A�E�g
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

	// �s�N�Z���V�F�[�_�[
	{
		
		create_ps_from_cso(device, "Shader\\SkinnedMeshPS.cso", pixelShader[ModelShader::Default].GetAddressOf());
		create_ps_from_cso(device, "Shader\\InstancingModelPS.cso", pixelShader[ModelShader::Instancing].GetAddressOf());
	}

	// �萔�o�b�t�@
	{
		// �V�[���p�o�b�t�@
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

		desc.ByteWidth = sizeof(InstancingConstants);
		hr = device->CreateBuffer(&desc, 0, instanBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	}

	
	//
	//// �T���v���X�e�[�g
	D3D11_SAMPLER_DESC desc;
	::memset(&desc, 0, sizeof(desc));
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = FLT_MAX;
	// �V���h�E�}�b�v�p
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
}

// �`��J�n
void DefaultShader::Begin(ID3D11DeviceContext* dc, const RenderContext& rc)
{
	
	dc->VSSetShader(vertexShader[ModelShader::Default].Get(), nullptr, 0);
	dc->PSSetShader(pixelShader[ModelShader::Default].Get(), nullptr, 0);
	Graphics& graphics = Graphics::Instance();
	sceneBuffers = graphics.GetConstant_buffers(0);

	
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);

	scene_constants data{};
	// �r���[�s��ƃv���W�F�N�V�����s����|�����킹��(��Z)
	XMStoreFloat4x4(&data.viewProjection, V * P);
	// ���̓����������ݒ�
	data.lightDirection = rc.lightDirection;
	//�J�����ʒu��ݒ�
	data.cameraPosition = rc.viewPosition;
	// �r���[�E�v���W�F�N�V�����s��ƌ��̓�����������V�[���萔�o�b�t�@�Ƃ��Đݒ�
	dc->UpdateSubresource(sceneBuffers.Get(), 0, 0, &data, 0, 0);
	// �ݒ肵���V�[���萔�o�b�t�@�� GPU ���ɓo�^
    dc->VSSetConstantBuffers(1, 1, sceneBuffers.GetAddressOf());
	//�ݒ肵���V�[���萔�o�b�t�@���s�N�Z���V�F�[�_�[���ɓo�^
	dc->PSSetConstantBuffers(1, 1, sceneBuffers.GetAddressOf());
	ShadowConstants shadow{};
	shadow.shadowProjection = rc.shadowMapData.lightViewProjection;
	dc->UpdateSubresource(shadowBuffer.Get(), 0, 0, &shadow, 0, 0);
	dc->VSSetConstantBuffers(5, 1, shadowBuffer.GetAddressOf());
	//�ݒ肵���V�[���萔�o�b�t�@���s�N�Z���V�F�[�_�[���ɓo�^
	dc->PSSetConstantBuffers(5, 1, shadowBuffer.GetAddressOf());
	dc->PSSetSamplers(10, 1, shadowMapSamplerState.GetAddressOf());
	//�[�x�e�X�g�F�I���A�[�x���C�g�F�I���̐[�x�X�e���V���X�e�[�g��ݒ肵�Ă���
	dc->OMSetDepthStencilState(graphics.GetDepthStencilState(ONON), 1);
	dc->RSSetState(graphics.GetRasterizerState(RasterizerState::Solid));
	
}

// �`��
void DefaultShader::Draw(ID3D11DeviceContext* dc, Model* model, const DirectX::XMFLOAT4 uvstatus, const DirectX::XMFLOAT4 color)
{
	
	Graphics& graphics = Graphics::Instance();
	const SkinnedMeshResouurce* resource = model->GetResource();
	//std::vector<SkinnedMeshResouurce::constants> buffers = buffer;
	int size = 0;
	for (const SkinnedMeshResouurce::Mesh& mesh : resource->GetMeshes())
	{
	   
		SkinnedMeshResouurce::constants buffers = model->GetBufferData().at(size);
		UvConstants uv;
		uint32_t stride{ sizeof(SkinnedMeshResouurce::Vertex) };
		uint32_t offset{ 0 };
		dc->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
		dc->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dc->IASetInputLayout(inputLayout.Get());
	
		for (const SkinnedMeshResouurce::Mesh::subset& subset : mesh.subsets)
		{
			// �}�e���A���̎���ID ����}�e���A�����擾���Q�ƂƂ��Đݒ�
			const SkinnedMeshResouurce::Material& material{ resource->materials.find({subset.materialUniqueId}).operator*() };
			
			XMStoreFloat4(&buffers.materialColor, XMLoadFloat4(&color) * XMLoadFloat4(&material.Kd));
			dc->UpdateSubresource(constantBuffer.Get(), 0, 0, &buffers, 0, 0);
			dc->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
			dc->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
			uv.uvStatus = uvstatus;
			dc->UpdateSubresource(uvBuffer.Get(), 0, 0, &uv, 0, 0);
			dc->PSSetConstantBuffers(2, 1, uvBuffer.GetAddressOf());
			//�e�N�X�`��
			dc->PSSetShaderResources(0, 1, resource->shaderResourceViews.at(material.textureFilenames[0]).GetAddressOf());
			//�@���}�b�v
			dc->PSSetShaderResources(1, 1, resource->shaderResourceViews.at(material.textureFilenames[1]).GetAddressOf());
			// ���^���b�N�}�b�v
			dc->PSSetShaderResources(3, 1, resource->shaderResourceViews.at(material.textureFilenames[2]).GetAddressOf());
			// �e���}�b�v
			dc->PSSetShaderResources(4, 1, resource->shaderResourceViews.at(material.textureFilenames[3]).GetAddressOf());
			// �G�~�b�V�u�}�b�v
			dc->PSSetShaderResources(7, 1, resource->shaderResourceViews.at(material.textureFilenames[4]).GetAddressOf());
			//�f�B�]���u
			dc->PSSetShaderResources(5, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::Dissolve)->GetShaderResourceView().GetAddressOf());
			//�f�B�]���u �~
			dc->PSSetShaderResources(6, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::DissolveCircle)->GetShaderResourceView().GetAddressOf());
			dc->DrawIndexed(subset.indexCount, subset.startIndexLocation, 0);
		}
		size++;
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
			// �}�e���A���̎���ID ����}�e���A�����擾���Q�ƂƂ��Đݒ�
			const SkinnedMeshResouurce::Material& material{ resource->materials.find({subset.materialUniqueId}).operator*() };

			//XMStoreFloat4(&buffer.materialColor, XMLoadFloat4(&color));
			//constantbuffer = modle-Getconstant_buffer();
			dc->UpdateSubresource(instanBuffer.Get(), 0, 0, &buffer, 0, 0);
			dc->VSSetConstantBuffers(0, 1, instanBuffer.GetAddressOf());
			dc->PSSetConstantBuffers(0, 1, instanBuffer.GetAddressOf());
			dc->PSSetShaderResources(0, 1, resource->shaderResourceViews.at(material.textureFilenames[0]).GetAddressOf());
			//�@���}�b�v
			dc->PSSetShaderResources(1, 1, resource->shaderResourceViews.at(material.textureFilenames[1]).GetAddressOf());
			dc->DrawIndexedInstanced(subset.indexCount, max, subset.startIndexLocation, 0, 0);
		}
	}
}

// �`��I��
void DefaultShader::End(ID3D11DeviceContext* dc)
{
	dc->VSSetShader(nullptr, nullptr, 0);
	dc->PSSetShader(nullptr, nullptr, 0);
	dc->IASetInputLayout(nullptr);
}
