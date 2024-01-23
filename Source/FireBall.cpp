#include"FireBall.h"
#include"StageManager.h"
#include"Light.h"
#include"AudioAll.h"
// �R���X�g���N�^
FireBall::FireBall(ObjectManager* manager,float size)
	:Object(manager)//���N���X�̃R���X�g���N�^�Ăяo��
{

	//�\���T�C�Y�𒲐�
	radius =size;
	type = 0;
	handle = EffectAll::Instance().fireBallEffect->Play(position,size);
	//EffectAll::Instance().swordEffect->SetScale(handle, { 0.5,0.5,0.5 });
	//���C�g�ݒ�
	light = new Light(position,0,1,1,1);
	LightManager::Instance().Register(light);
	damage = 2;
	invincibleTime = 2;
}

// �f�X�g���N�^
FireBall::~FireBall()
{
	EffectAll::Instance().fireBallEffect->Stop(handle);
}

//�X�V����
void FireBall::Update(float elapsedTime)
{
	//��������
	lifeTimer -= elapsedTime;
	if (lifeTimer < 0)
	{
		//�������폜
		Destroy();
		light->Destroy();
	}
	//�ړ�
	switch (type)
	{
	case FireBallType::FirBallStraight:
		StraightUpdate(elapsedTime);
		break;
	}
	light->SetParameter(position,100, 2, 1, 0);//���C�g�X�V
	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

}
void FireBall::StraightUpdate(float elapsedTime) {
	float speed = this->speed * elapsedTime;
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;

	DirectX::XMFLOAT3 start = { position.x,position.y,position.z };
	// ���C�̏I�_�ʒu�͈ړ���̈ʒu
	DirectX::XMFLOAT3 end = { position.x + direction.x,position.y + direction.y,position.z + direction.z };

	// ���C�L���X�g�ɂ��n�ʔ���
	HitResult hit;
	//if (health > 0) {
	if (StageManager::Instance().RayCast(start, end, hit)) {//�n�ʂɒ��e������
		EffectAll::Instance().swordEffect->Play(hit.position, 0.4);
		EffectAll::Instance().fireBallEffect->Stop(handle);
		AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::DragonFBE)->Play(false, SE);
		direction = { NULL,NULL,NULL};
		lifeTimer = 1.0f;
	}

	EffectAll::Instance().fireBallEffect->SetPosition(handle, position);
	EffectAll::Instance().fireBallEffect->SetRotation(handle, { 0,angle,0 });

}

//�`�揈��
void FireBall::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
}


//����
void FireBall::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float angle)
{
	this->direction = direction;
	this->position = position;
	this->angle = angle;
	this->lifeTimer = lifeTimer;
	UpdateTransform();

}