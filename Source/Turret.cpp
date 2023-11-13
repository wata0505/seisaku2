#include"Turret.h"
#include "Graphics.h"
Turret::Turret()
{
	

	

	model = std::make_unique<Model>(".\\resources\\Sentorygun\\Taret_GUN.fbx", true);
	model->AppendAnimations(".\\resources\\Sentorygun\\GUN_motionBullet.fbx", 0);
	
	model->ModelSerialize(".\\resources\\Sentorygun\\Taret_GUN.fbx");
	model->ModelRegister(".\\resources\\Sentorygun\\Taret_GUN.fbx", "Taret_BODY.fbm\\TARET_GUN_BaseColor.png");
	
	model2 = std::make_unique<Model>(".\\resources\\Sentorygun\\Taret.fbx", true);
	model2->ModelSerialize(".\\resources\\Sentorygun\\Taret.fbx");
	model2->ModelRegister(".\\resources\\Sentorygun\\Taret.fbx", "Taret.fbm\\TARET_BODY_BaseColor.png");

	UpdateTransform(0, 0);
	UpdateTransform2(0, 0);

	model->UpdateBufferDara(transform);
	model2->UpdateBufferDara(transform2);

	renderdata = model->GetBufferData();
	renderdata2 = model2->GetBufferData();


	radius = 1.0f;
	notAttackRange=3.0f;

	scale.x = scale.y = scale.z = 80.0f;
	TransitionIdleState();
}
Turret::~Turret()
{

}

void Turret::Update(float elapsedTime)
{
	// �X�e�[�g���̍X�V����
	switch (state)
	{
	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;
	case State::Attack:
		UpdateAttackState(elapsedTime);
		break;
	}


	//SearchEnemy(territoryRange,attackRange);
	//TurnToTarget(elapsedTime, turnSpeed);


	UpdateTransform(0, 0);
	UpdateTransform2(0, 0);
	
	//�A�j���[�V�����X�V
	model->UpdateAnimation(elapsedTime, "root_x");
	model->UpdateSubAnimation(elapsedTime, "root_x");
	
	model->UpdateBufferDara(transform);
	model2->UpdateBufferDara(transform2);

	//���f���`����󂯓n��
	renderdata = model->GetBufferData();
	renderdata2 = model2->GetBufferData();

}

void Turret::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	shader->Draw(dc, model.get());
	shader->Draw(dc, model2.get());
	
}

void Turret::DrawDebugPrimitive()
{
	// ���N���X�̃f�o�b�O�v���~�e�B�u�`��
	Trap::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//�Փ˔���p�̃f�o�b�N�~����`��
	debugRenderer->DrawCylinder({ position.x,position.y -position.y,position.z } , radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

	//�Փ˔���p�̃f�o�b�N�~����`��
	debugRenderer->DrawCylinder({ position.x,position.y - position.y,position.z }, notAttackRange, height, DirectX::XMFLOAT4(1, 0, 0, 1));


	// �꒣��͈͂��f�o�b�O�~���`��
	debugRenderer->DrawCylinder({ territoryOrigin.x,territoryOrigin.y - territoryOrigin.y,territoryOrigin.z }, territoryRange, height, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}

//�s��X�V����
void Turret::UpdateTransform2(int axisType, int lengthType)
{
	const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
		{ -1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},		//0:RHS Y-UP�@�E��n
		{  1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},		//1:LHS Y-UP�@����n
		{ -1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1},		//2:RHS Z-UP�@�E��n
		{  1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},		//3:LHS Z-UP�@����n
	};

	const float scale_factor[] = { 0.01f,1.0f };

	DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[axisType]) * DirectX::XMMatrixScaling(scale_factor[lengthType],scale_factor[lengthType],scale_factor[lengthType]) };
	//�X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//��]�s����쐬
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0, 0, angle.z);
	//�ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y - 1.5f, position.z);
	//3�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = C * S * R * T;
	//�v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform2, W);
}


// �ҋ@�X�e�[�g�֑J��
void Turret::TransitionIdleState()
{
	state = State::Idle;
	model->PlayAnimation(-1, false);
}

// �ҋ@�X�e�[�g�X�V����
void Turret::UpdateIdleState(float elapsedTime)
{
	// �v���C���[���G
	if (SearchEnemy(territoryRange, notAttackRange))
	{
		// ����������ǐՃX�e�[�g�֑J��
		TransitionAttackState();
	}
}

// �U���X�e�[�g�֑J��
void Turret::TransitionAttackState()
{
	state = State::Attack;
	model->PlayAnimation(static_cast<int>(TurretAnimation::Attack), true);
}

// �U���X�e�[�g�X�V����
void Turret::UpdateAttackState(float elapsedTime)
{
	TurnToTarget(elapsedTime, turnSpeed);
	// �U���A�j���[�V�������I����Ĕ͈͓��ɓG�����Ȃ����
	if (SearchEnemy(territoryRange, notAttackRange) == false)
	{
		//�ҋ@�X�e�[�g��
		TransitionIdleState();
	}
}
