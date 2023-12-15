#include"Base.h"
#include "Camera.h"
#include "Graphics.h"

static Base* instance = nullptr;

Base& Base::Instance() {
	return *instance;
};

Base::Base(DirectX::XMFLOAT3 pos)
{
	instance = this;

	obj = std::make_unique<Model>(".\\resources\\SecurityGate\\SecurityGate.fbx", true, false);
	obj->ModelSerialize(".\\resources\\SecurityGate\\SecurityGate.fbx");
	//model->ModelCreate(".\\resources\\ExampleStage\\ExampleStage.fbx");
	obj->ModelRegister(".\\resources\\SecurityGate\\SecurityGate.fbx","Texture//Albedo.png");
	// �s��X�V
	uiSprite = std::make_unique<Sprite>(L".\\resources\\HP.png");
	position = pos;
	scale.x = scale.y = scale.z = 10.0;
	UpdateTransform();
	obj->UpdateBufferDara(transform);
	transform = obj->GetBufferTransform();
}

Base::~Base()
{
	//�X�e�[�W���f����j��

}

//�X�V����
void Base::Update(float elapseTime)
{
	renderflag = Collision::IntersectFanVsSphere(
		Camera::Instance().GetEye(),
		Camera::Instance().GetFront(),
		Camera::Instance().GetFovY(),
		Camera::Instance().GetFarZ(),
		position,
		radius);
	UpdateTransform();
}

void Base::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	// �\���p�̂��߃��[���h�s��ɍX�V����
	obj->UpdateTransform();
	obj->UpdateBufferDara(transform);
	//�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
	shader->Draw(dc,obj.get());
	jitterStrength = 0;
}

void Base::HpDisplay(RenderContext& rc, SpriteShader* shader) {
	float gaugeThrate = this->hp / static_cast<float>(this->hpMax);
	if (!renderflag)return;
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	rc.deviceContext->RSGetViewports(&numViewports, &viewport);

	// �ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	// �S�Ă̓G�̓����HP�Q�[�W��\��
	
	if (hp > 0)
	{
		DirectX::XMFLOAT3 sPosition = {position.x,position.y +5,position.z};
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
			World
		);
		//�X�N���[�����W
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);
		//�G�Ƃ̋����Ń��b�N�I���̑傫����������

		uiSprite->Render(rc.deviceContext,
			screenPosition.x - 200 * 0.5, 
			screenPosition.y - 20 * 0.5,
			200 * gaugeThrate,
			20,
			0, 0,
			static_cast<float>(uiSprite->GetTextureWidth()* gaugeThrate),
			static_cast<float>(uiSprite->GetTextureHeight()),
			0,
			3.0f, 1.0f, 1.0f, 1.0f, 0, 0
		);
		shader->Draw(rc, uiSprite.get());
	}
}

void Base::InputDamage(int damage) {
	if (hp > 0) {
		hp -= damage;
		jitterStrength = 0.05;
		if (hp < 0) {
			hp = 0;
		}
	}
}
bool Base::PillarVS(const DirectX::XMFLOAT3& pos, const float radius, const float height, DirectX::XMFLOAT3& outPos)
{
	if (Collision::IntersectCylinderVsCylinder(
		position, this->radius, this->height,
		pos, radius, height,
		outPos
	)) {

		return true;

	}
	return false;
}

void Base::UpdateTransform()
{
	const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
		{ -1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},		//0:RHS Y-UP�@�E��n
		{  1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},		//1:LHS Y-UP�@����n
		{ -1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1},		//2:RHS Z-UP�@�E��n
		{  1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},		//3:LHS Z-UP�@����n
	};

	const float scale_factor = 0.01f;

	DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor,scale_factor,scale_factor) };
	//�X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//��]�s����쐬
	//DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;
	//�ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	//3�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = C * S * R * T;
	//�v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, W);

}