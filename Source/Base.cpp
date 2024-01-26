#include "Base.h"
//#include "Camera.h"
#include "Graphics.h"
//#include "Collision.h"

static Base* instance = nullptr;

// �C���X�^���X�擾
Base& Base::Instance() 
{
	return *instance;
};

// �R���X�g���N�^
Base::Base(DirectX::XMFLOAT3 position)
{
	instance = this;

	obj = std::make_unique<Model>(".\\resources\\SecurityGate\\SecurityGate.fbx", true, false);
	obj->ModelSerialize(".\\resources\\SecurityGate\\SecurityGate.fbx");
	obj->ModelRegister(".\\resources\\SecurityGate\\SecurityGate.fbx","Texture\\Albedo.png");
	
	this->position = position;					// �ʒu�ݒ�
	scale.x = scale.y = scale.z = 10.0f;		// �T�C�Y�ݒ�
	UpdateTransform();							// �s��X�V����
	obj->UpdateBufferData(transform);			// �`����X�V����
	//transform = obj->GetBufferTransform();	// �C���X�^���V���O���̕`����s��擾

	hpUISprite = std::make_unique<Sprite>(L".\\resources\\UI\\TowerHP.png");	// HPUI�e�N�X�`���ǂݍ���
	lossHPUISprite = std::make_unique<Sprite>(L".\\resources\\UI\\TowerLossHP.png");	// ����HPUI�e�N�X�`���ǂݍ���

	// �z���O�����V�F�[�_�[��񏉊���
	minHeight = -4.0f;	// �Œፂ��
	maxHeight = 4.0f;	// �ō�����
}

// �X�V����
void Base::Update(float elapseTime)
{
	// �o�ߎ��ԉ��Z
	progressTimer += elapseTime;
	// �V�F�[�_�[��񒲐�
	obj->ShaderAdjustment(50.0f, progressTimer, maxHeight);
#if 0
	isRender = Collision::IntersectFanVsSphere(
		Camera::Instance().GetEye(),
		Camera::Instance().GetFront(),
		Camera::Instance().GetFovY(),
		Camera::Instance().GetFarZ(),
		position,
		radius);

	// �s��X�V����
	UpdateTransform();
#endif
}
// �`�揈��
void Base::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
	// �\���p�̂��߃��[���h�s��ɍX�V����
	//obj->UpdateTransform();
	//obj->UpdateBufferData(transform);

	shader->Draw(deviceContext, obj.get(), { 0.0f, minHeight, minHeight, maxHeight }, {});
	skyboxColor = 0.0f;
}

// �s��X�V����
void Base::UpdateTransform()
{
	const DirectX::XMFLOAT4X4 coordinate_system_transforms[]
	{
		{ -1,0,0,0, 0,1, 0,0, 0,0,1,0, 0,0,0,1 },		//0:RHS Y-UP�@�E��n
		{  1,0,0,0, 0,1, 0,0, 0,0,1,0, 0,0,0,1 },		//1:LHS Y-UP�@����n
		{ -1,0,0,0, 0,0,-1,0, 0,1,0,0, 0,0,0,1 },		//2:RHS Z-UP�@�E��n
		{  1,0,0,0, 0,0, 1,0, 0,1,0,0, 0,0,0,1 },		//3:LHS Z-UP�@����n
	};

	const float scaleFactor = 0.01f;

	DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) };
	// �X�P�[���s��쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// ��]�s��쐬
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;
	// �ʒu�s��쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// 3�̍s���g�ݍ��킹�A���[���h�s��쐬
	DirectX::XMMATRIX W = C * S * R * T;
	// �v�Z�������[���h�s��擾
	DirectX::XMStoreFloat4x4(&transform, W);
}

// UI�\������
void Base::HpDisplay(RenderContext& renderContext, SpriteShader* shader)
{
#if 0
	if (!isRender) return;

	// �̗͎c�ʊ���
	float gaugeThrate = this->hp / static_cast<float>(this->hpMax);
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	renderContext.deviceContext->RSGetViewports(&numViewports, &viewport);

	// �ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&renderContext.view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&renderContext.projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
	
	// �̗͂��c���Ă����HP�Q�[�W�\��
	if (hp > 0)
	{
		DirectX::XMFLOAT3 sPosition = { position.x, position.y + 5.0f, position.z };
		DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&sPosition);
		// ���[���h���W����X�N���[�����W��
		DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
			WorldPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			Graphics::Instance().GetScreenWidth(),
			Graphics::Instance().GetScreenHeight(),
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World);
		// �X�N���[�����W
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

		// ���E����
		static constexpr float width = 200.0f;
		static constexpr float height = 20.0f;
		uiSprite->Render(renderContext.deviceContext,
			screenPosition.x - width * 0.5f,  screenPosition.y - height * 0.5f,
			width * gaugeThrate, height,
			0.0f, 0.0f,
			static_cast<float>(uiSprite->GetTextureWidth() * gaugeThrate), static_cast<float>(uiSprite->GetTextureHeight()),
			0.0f,
			3.0f, 1.0f, 1.0f, 1.0f);
		shader->Draw(renderContext, uiSprite.get());
	}
#else
	Graphics& graphics = Graphics::Instance();
	static constexpr float scale = 0.025f;
	static constexpr float size = 0.5f;
	// �X�N���[�����E����
	float screenWidth = graphics.GetScreenWidth() * scale;
	float screenHeight = graphics.GetScreenHeight() * scale;
	float textureWidth = static_cast<float>(hpUISprite->GetTextureWidth());
	float textureHeight = static_cast<float>(hpUISprite->GetTextureHeight());
	// �̗͎c�ʊ���
	float gaugeThrate = this->hp / static_cast<float>(this->hpMax);
	lossHPUISprite->Render(renderContext.deviceContext,
		screenHeight, screenHeight,
		textureWidth * size, textureHeight * size,
		0.0f, 0.0f,
		textureWidth, textureHeight,
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
	shader->Draw(renderContext, lossHPUISprite.get());
	hpUISprite->Render(renderContext.deviceContext,
		screenHeight, screenHeight + (textureHeight * (1.0f - gaugeThrate)) * 0.5f,
		textureWidth * size, textureHeight * size - (textureHeight * (1.0f - gaugeThrate)) * 0.5f,
		0.0f, (textureHeight * (1.0f - gaugeThrate)),
		textureWidth, (textureHeight * gaugeThrate),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
	shader->Draw(renderContext, hpUISprite.get());
#endif
}

// �_���[�W����
void Base::InputDamage(int damage) 
{
	// �̗͂��c���Ă����
	if (hp > 0) 
	{
		hp -= damage;			// �̗͌���
		skyboxColor = 1.0f;		// �X�J�C�{�b�N�X�F(����)�ݒ�
		// �̗͂��c���Ă��Ȃ����
		if (hp < 0) 
		{
			hp = 0;	// �̗͂��Œ�
		}
	}
}