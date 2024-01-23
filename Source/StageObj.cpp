#include "StageObj.h"
//#include "Camera.h"

// �R���X�g���N�^
StageObj::StageObj(DirectX::XMFLOAT3 position, float angleY, float angleZ)
{
	obj = std::make_unique<Model>(".\\resources\\Box.fbx", true, false);
	obj->ModelSerialize(".\\resources\\Box.fbx");
	obj->ModelRegister(".\\resources\\Box.fbx", "CyberStage\\Texture\\Emission.png");
	
	angle.y = DirectX::XMConvertToRadians(angleY);	// Y���p�x�ݒ�
	angle.z = DirectX::XMConvertToRadians(angleZ);	// Z���p�x�ݒ�
	this->position = position;						// �ʒu�ݒ�
	scale.x = scale.y = scale.z = 10.0f;			// �T�C�Y�ݒ�
	UpdateTransform();								// �s��X�V����
	raycastTransform = transform;					// ���C�L���X�g�p�s��ݒ�
	obj->UpdateBufferData(transform);				// �`����X�V����
	transform = obj->GetBufferTransform();			// �C���X�^���V���O���̕`����s��擾
	instancing = true;								// �C���X�^���V���O��
}

// �X�V����
void StageObj::Update(float elapseTime)
{
#if 0
	isRender = Collision::IntersectFanVsSphere(
		Camera::Instance().GetEye(),
		Camera::Instance().GetFront(),
		Camera::Instance().GetFovY(),
		Camera::Instance().GetFarZ(),
		position,
		radius);
#endif
}
// �`�揈��
void StageObj::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{

}

// ���C�L���X�g
bool StageObj::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, obj.get(), hit, raycastTransform);
}

// �s��X�V����
void StageObj::UpdateTransform()
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