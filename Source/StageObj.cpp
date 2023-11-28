#include"StageObj.h"
#include "Camera.h"
StageObj::StageObj(DirectX::XMFLOAT3 pos,float angle2)
{
	obj = std::make_unique<Model>(".\\resources\\Box.fbx", true, false);
	obj->ModelSerialize(".\\resources\\Box.fbx");
	//model->ModelCreate(".\\resources\\ExampleStage\\ExampleStage.fbx");
	obj->ModelRegister(".\\resources\\Box.fbx","Stag\\Stage.fbm\\Tile.png");
	// �s��X�V
	angle.y = DirectX::XMConvertToRadians(angle2);
	position = pos;
	scale.x = scale.y = scale.z = 10.0;
	UpdateTransform();
	transform2 = transform;
	obj->UpdateBufferDara(transform);
	transform = obj->GetBufferTransform();
	instsncing = true;//�C���X�^���V���O
}

StageObj::~StageObj()
{
	//�X�e�[�W���f����j��

}

//�X�V����
void StageObj::Update(float elapseTime)
{
	renderflag = Collision::IntersectFanVsSphere(
		Camera::Instance().GetEye(),
		Camera::Instance().GetFront(),
		Camera::Instance().GetFovY(),
		Camera::Instance().GetFarZ(),
		position,
		radius);

}

void StageObj::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	// �\���p�̂��߃��[���h�s��ɍX�V����
	//model->UpdateTransform(transform);

	//�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
	//if(renderflag)shader->Draw(dc, obj.get());
}

bool StageObj::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	float i = 0;
	return Collision::IntersectRayVsModel(start, end, obj.get(), hit, transform2);
}
bool StageObj::PillarVS(const DirectX::XMFLOAT3& pos, const float radius, const float height, DirectX::XMFLOAT3& outPos)
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

void StageObj::UpdateTransform()
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