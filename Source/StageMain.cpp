#include "StageMain.h"

// �R���X�g���N�^
StageMain::StageMain()
{
	model = std::make_unique<Model>(".\\resources\\CyberStage\\CyberStage.fbx", true, false);
	model->ModelSerialize(".\\resources\\CyberStage\\CyberStage.fbx");
	model->ModelRegister(".\\resources\\CyberStage\\CyberStage.fbx", "Texture\\HoneycombStructure.png");

	position.y = -2.5f;						// Y���ʒu�ݒ�
	scale.x = scale.y = scale.z = 100.0f;	// �T�C�Y�ݒ�
	UpdateTransform();						// �s��X�V����
    model->UpdateBufferData(transform);		// �`����X�V����
}

// �X�V����
void StageMain::Update(float elapseTime)
{
	// �o�ߎ��ԉ��Z
	progressTimer += elapseTime * 4.0f;
	// �V�F�[�_�[��񒲐�
	model->ShaderAdjustment(0.0f, progressTimer, 0.0f);
}
// �`�揈��
void StageMain::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
	shader->Draw(deviceContext, model.get());
}

// ���C�L���X�g
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model.get(), hit, transform);
}

// �s��X�V����
void StageMain::UpdateTransform()
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