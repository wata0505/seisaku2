#include "ParticleSpriteShader.h"
#include "misc.h"
#include "Graphics.h"
#include "Camera.h"
#include "Player.h"

using namespace DirectX;

ParticleSpriteShader::ParticleSpriteShader(ID3D11Device* device)
{
	
	// ���_�V�F�[�_�[
	{
		// ���̓��C�A�E�g
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PARTICLESIZE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SPEED",    0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PARTICLETYPE",    0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TIMER",    0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		create_vs_from_cso(device, "Shader\\ParticleSpriteVS.cso", vertexShader.ReleaseAndGetAddressOf(),
			inputLayout.ReleaseAndGetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
	}

	// �s�N�Z���V�F�[�_�[
	{
		create_ps_from_cso(device, "Shader\\ParticleSpritePS.cso", pixelShader.GetAddressOf());
	}
	// �W�I���g���V�F�[�_�[
	{
		create_gs_from_cso(device, "Shader\\ParticleSpriteGS.cso", geometryShader.GetAddressOf());
	}
	// �萔�o�b�t�@
	{
		//// �V�[���p�o�b�t�@
		
		D3D11_BUFFER_DESC cBufferDesc;
		cBufferDesc.Usage = D3D11_USAGE_DEFAULT;    // ���I(�_�C�i�~�b�N)�g�p�@
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // �萔�o�b�t�@
		cBufferDesc.CPUAccessFlags = 0;     // CPU���珑������
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;
		cBufferDesc.ByteWidth = sizeof(SceneConstants);

		HRESULT hr = device->CreateBuffer(&cBufferDesc, 0, sceneBuffers.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		cBufferDesc.ByteWidth = sizeof(ColorConstants);

		device->CreateBuffer(&cBufferDesc, 0, colorBuffers.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		cBufferDesc.ByteWidth = sizeof(CalculationConstants);

		device->CreateBuffer(&cBufferDesc, 0, calculationBuffers.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	{//�R���s���[�g�V�F�[�_�[
		create_cs_from_cso(device, "Shader\\ParticleSpriteCS.cso", computeShader.GetAddressOf());
	}
	

	//�p�[�e�B�N���̌`
	moveSprite[ParticleSprite::ParticleSoft] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Particle_Soft.png");
	moveSprite[ParticleSprite::ParticleLine] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Line9.png");
	moveSprite[ParticleSprite::ParticleImpact] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Shock_wave1.png");
	moveSprite[ParticleSprite::ParticleWind] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\wind04.png");
	moveSprite[ParticleSprite::ParticleSquare] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\hisigata1.png");
	moveSprite[ParticleSprite::ParticleBurst] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Burst_2.png");
	moveSprite[ParticleSprite::ParticleTriangle] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\sannkaku3.png");

	

	
}

// �`��J�n
void ParticleSpriteShader::Begin(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->GSSetShader(geometryShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	rc.deviceContext->CSSetShader(computeShader.Get(), nullptr, 0);
	Graphics& graphics = Graphics::Instance();
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);

	SceneConstants scene{};
	DirectX::XMMATRIX P =DirectX::XMLoadFloat4x4(&Camera::Instance().GetProjection());
	P = XMMatrixTranspose(P);//�]�n�s���
	scene.view = Camera::Instance().GetView();
	DirectX::XMStoreFloat4x4(&scene.projection, P);

	// �r���[�E�v���W�F�N�V�����s��ƌ��̓�����������V�[���萔�o�b�t�@�Ƃ��Đݒ�
	rc.deviceContext->UpdateSubresource(sceneBuffers.Get(), 0, 0, &scene, 0, 0);
	rc.deviceContext->VSSetConstantBuffers(0, 1, sceneBuffers.GetAddressOf());
	rc.deviceContext->PSSetConstantBuffers(0, 1, sceneBuffers.GetAddressOf());
	rc.deviceContext->GSSetConstantBuffers(0, 1, sceneBuffers.GetAddressOf());
	rc.deviceContext->CSSetConstantBuffers(0, 1, sceneBuffers.GetAddressOf());
	

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	rc.deviceContext->OMSetBlendState(graphics.GetBlendState(BlendState::Alpha), blend_factor, 0xFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(graphics.GetDepthStencilState(DepthStencilState::ONOFF), 0);
	rc.deviceContext->RSSetState(graphics.GetRasterizerState(RasterizerState::Solid));

	

}
HRESULT ParticleSpriteShader::CreateResource() {
	

	return S_OK;
}


HRESULT ParticleSpriteShader::ComputeShader(const RenderContext& rc, ParticleSprite* particleSprite)
{
	
	// **********************************************************
	// �R���s���[�g�E�V�F�[�_���g�������Z
	// **********************************************************
	HRESULT hr = S_OK;
	// �V�F�[�_��ݒ�
	Graphics::Instance();
	
	ID3D11ShaderResourceView* pViewNULL = NULL;

	CalculationConstants calculation{};
	//�v�Z�p�o�t�@����
	calculation.tail = Player::Instance().GetTailPos();
	calculation.pattern = Player::Instance().GetPatternPos();
	calculation.max = particleSprite->max;
	calculation.uvStatus.x = particleSprite->uvStatus.x;
	rc.deviceContext->UpdateSubresource(calculationBuffers.Get(), 0, 0, &calculation, 0, 0);
	rc.deviceContext->CSSetConstantBuffers(1, 1, calculationBuffers.GetAddressOf());
	rc.deviceContext->GSSetConstantBuffers(1, 1, calculationBuffers.GetAddressOf());
	rc.deviceContext->PSSetConstantBuffers(1, 1, calculationBuffers.GetAddressOf());
	// ���݂̃V�F�[�_ ���\�[�X �r���[������
	rc.deviceContext->CSSetShaderResources(0, 1, &pViewNULL);
	// �A���I�[�_�[�h�E�A�N�Z�X�E�r���[�̐ݒ�
	rc.deviceContext->CSSetUnorderedAccessViews(0, 1, particleSprite->gPUAV[particleSprite->chainB].GetAddressOf(), NULL);

	// �V�F�[�_ ���\�[�X �r���[�̐ݒ�
	rc.deviceContext->CSSetShaderResources(0, 1, particleSprite->gPSRV[particleSprite->chainA].GetAddressOf());

	// �R���s���[�g�E�V�F�[�_�̎��s

	rc.deviceContext->Dispatch(100, 1, 1);//�O���[�v�̐�
	rc.deviceContext->CopyResource(particleSprite->verBufferDrawFrom.Get(), particleSprite->gPBuffer[0].Get());


	
	return hr;
}
void ParticleSpriteShader::Update(const RenderContext& rc ,ParticleSprite* particleSprite) {
	ComputeShader(rc,particleSprite);
	particleSprite->chainA = particleSprite->chainA ? 0 : 1;//�o�b�t�@�[�̐؂�ւ�
	particleSprite->chainB = particleSprite->chainB ? 0 : 1;//�o�b�t�@�[�̐؂�ւ�
}
// �`��
void ParticleSpriteShader::Draw(const RenderContext& rc, const Sprite* sprite)
{
	Graphics& graphics = Graphics::Instance();
	int max = ParticleSpriteManager::Instance().GetParticleCount();
	for (int i = 0; i < max;i++) {
		ParticleSprite* particle = ParticleSpriteManager::Instance().GetParticleSprite(i);
		
		rc.deviceContext->OMSetDepthStencilState(graphics.GetDepthStencilState(particle->depth), 0);
		
		Update(rc, particle);

		UINT strides[1] = { sizeof(PVConstants) };
		UINT offsets[1] = { 0 };
		rc.deviceContext->IASetVertexBuffers(0, 1, particle->verBufferDrawFrom.GetAddressOf(), strides, offsets);
		// IA�ɓ��̓��C�A�E�g�E�I�u�W�F�N�g��ݒ�
		rc.deviceContext->IASetInputLayout(inputLayout.Get());
		// IA�Ƀv���~�e�B�u�̎�ނ�ݒ�
		rc.deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

		ColorConstants color{};
		color.materialColor = particle->materialColor;
		rc.deviceContext->UpdateSubresource(colorBuffers.Get(), 0, 0, &color, 0, 0);
		rc.deviceContext->PSSetConstantBuffers(2, 1, colorBuffers.GetAddressOf());
		//�p�[�e�B�N���`
		rc.deviceContext->PSSetShaderResources(0, 1, moveSprite[particle->spriteType]->GetShaderResourceView().GetAddressOf());
		//�e�N�X�`��
		rc.deviceContext->PSSetShaderResources(1, 1, EffectTexAll::Instance().GetSprite(particle->texType).get()->GetShaderResourceView().GetAddressOf());
		//�f�B�]���u
		rc.deviceContext->PSSetShaderResources(5, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::Dissolve)->GetShaderResourceView().GetAddressOf());

		rc.deviceContext->Draw(particle->max, 0);
	}
}


// �`��I��
void ParticleSpriteShader::End(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->PSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->GSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->CSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->IASetInputLayout(nullptr);
	rc.deviceContext->RSSetState(Graphics::Instance().GetRasterizerState(RasterizerState::Solid));
}