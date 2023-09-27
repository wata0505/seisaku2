#include "StageMain.h"



//�R���X�g���N�^
StageMain::StageMain()
{

	model = std::make_unique<Model>(".\\resources\\Stag\\GameStagMin5.fbx", true, false);
	model->ModelSerialize(".\\resources\\Stag\\GameStagMin5.fbx");
	//model->ModelCreate(".\\resources\\ExampleStage\\ExampleStage.fbx");
	model->ModelRegister(".\\resources\\Stag\\GameStagMin5.fbx");

	// �s��X�V
	//angle.y = 3;
	position.y = 0;
	scale.x = scale.y = scale.z = 1.0;
	UpdateTransform();
    model->UpdateBufferDara(transform);
}

StageMain::~StageMain()
{
	//�X�e�[�W���f����j��
	
}

//�X�V����
void StageMain::Update(float elapseTime)
{
	
}

void StageMain::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	// �\���p�̂��߃��[���h�s��ɍX�V����
	//model->UpdateTransform(transform);

	//�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
	shader->Draw(dc, model.get(), model->GetBufferData());
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model.get(), hit,transform);
}

bool StageMain::PillarVS(const DirectX::XMFLOAT3& pos, const float radius, const float height, DirectX::XMFLOAT3& outPos)
{
	return false;
}
void StageMain::UpdateTransform()
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