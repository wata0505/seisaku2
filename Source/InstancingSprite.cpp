#include"InstancingSprite.h"
#include"InstancingSpriteManager.h"
#include"Player.h"

// �R���X�g���N�^
InstancingSprite::InstancingSprite()//���N���X�̃R���X�g���N�^�Ăяo��
{

	//�\���T�C�Y�𒲐�
	scale.x = scale.y = scale.z = 1.0f;
	//scale.x = scale.y = scale.z = 1.0f;
}

// �f�X�g���N�^
InstancingSprite::~InstancingSprite()
{

}

//�X�V����
void InstancingSprite::Update(float elapsedTime)
{
	//��������

	lifeTimer -= elapsedTime;
	if (lifeTimer < 0)
	{
		//�������폜
		uvStatus.x -= elapsedTime;
		if (uvStatus.x < 0) {
			Destroy();
		}
	}
	//�ړ�
	switch (type)
	{
	case InstancingSpriteType::FlameBreath:
		FlameBreathUpdate(elapsedTime);
		break;
	}


}
void InstancingSprite::FlameBreathUpdate(float elapsedTime) {
	float speed = this->speed * elapsedTime;
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;
	scale.x += speed;
	scale.y += speed;
	texTimer += speed;//�e�N�X�`���A�j���[�V����
	if (texTimer > texTimerMax) {
		texTimer = 0;
	}
	uvStatus.z += elapsedTime;//UV�A�j���[�V����
	uvStatus.y = 1.5;//�f�B�]���u�l
	radius += speed * 0.4;//�����蔻��̓X�s�[�h�̔���
}

void InstancingSprite::Destroy()
{
	InstancingSpriteManager::Instance().Remove(this);
}


//����
void InstancingSprite::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, int type, int spriteType, float lifeTimer, float speed, DirectX::XMFLOAT4 materialColor)
{
	this->position = { position.x,position.y,position.z, 1};
	this->direction = direction;
	this->type = type;
	this->lifeTimer = lifeTimer;
	this->speed = speed;
	this->materialColor = materialColor;
	this->spriteType = spriteType;
}