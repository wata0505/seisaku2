#include "Trap.h"
#include "Graphics.h"
#include"TrapManager.h"
#include "EnemyManager.h"
#include "Camera.h"
#include <math.h>

//�f�o�b�N�p
#include "Player.h"

//�f�o�b�O�v���~�e�B�u�`��
void Trap::DrawDebugPrimitive()
{
	DebugRenderer* debugRendere = Graphics::Instance().GetDebugRenderer();

	//�Փ˔���p�̃f�o�b�N�~����`��
	debugRendere->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//�j��
void Trap::Destroy()
{
	TrapManager::Instance().Remove(this);
}

//�s��X�V����
void Trap::UpdateTransform(int axisType, int lengthType)
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
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	//�ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	//3�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = C * S * R * T;
	//�v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

bool Trap::SearchEnemy(float territoryRange, float radius)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	//dist = FLT_MAX;//�꒣��͈�
	dist = territoryRange;//�꒣��͈�

	for (int i = 0; i < enemyManager.GetEnemyCount(); i++) {
		//�G�Ƃ̋�������
		Enemy* e = enemyManager.GetEnemy(i);
		float vx = e->GetPosition().x - position.x;
		float vz = e->GetPosition().z - position.z;
		float d = sqrtf(vx * vx + vz * vz);

		if (d < dist && radius < d)//�߂�����Ɣ������Ȃ�
		{
			dist = d;
			targetPosition = e->GetPosition();

		}
	}

	//�f�o�b�N�p
	//float vx = Player::Instance().GetPosition().x - position.x;
	//float vz = Player::Instance().GetPosition().z - position.z;
	//float d = sqrtf(vx * vx + vz * vz);
	//if (d < dist && radius < d)//�߂�����Ɣ������Ȃ�
	//{
	//	dist = d;
	//	targetPosition = Player::Instance().GetPosition();
	//}



	if (dist != territoryRange)//�G���������Ă�����
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Trap::MoveToTarget(float elapsedTime, float speedRate)
{
	//�^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	vx /= dist;
	vy /= dist;
	vz /= dist;
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

void Trap::TurnToTarget(float elapsedTime, float speedRate)
{
	//�^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	vx /= dist;
	vy /= dist;
	vz /= dist;

	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
	TurnVertical();
}

//���񏈗�
void Trap::TurnVertical()
{
	DirectX::XMVECTOR Operator = DirectX::XMLoadFloat3(&position);        // ����L�����̈ʒu�x�N�g��
	DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&targetPosition);    // �^�[�Q�b�g�̈ʒu�x�N�g��

	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&Camera::Instance().GetEye());                // �J�����ʒu�x�N�g��

	// ����L����->�^�[�Q�b�g�x�N�g��
	DirectX::XMVECTOR OperatorToTarget = DirectX::XMVectorSubtract(Target, Operator);
	// �^�[�Q�b�g->����L�����x�N�g��
	DirectX::XMVECTOR TargetToOperator = DirectX::XMVectorSubtract(Operator, Target);
	// ����L����->�J�����x�N�g��
	DirectX::XMVECTOR OperatorToEye = DirectX::XMVectorSubtract(Eye, Operator);

	// X����]�̌v�Z
	{
		// ����L����->�^�[�Q�b�g�x�N�g���̒����Z�o
		float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(OperatorToTarget));
		// XZ���ʂ̑���L����->�^�[�Q�b�g�x�N�g���̒����Z�o
		DirectX::XMVECTOR XZFrogToTarget = DirectX::XMVectorSetY(OperatorToTarget, 0);
		float xzLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(XZFrogToTarget));
		// XZ���ʂ̒��������̎Εӂ̒����Ŋ����ē��ϒl�Z�o
		float dot = xzLength / length;
		dot = acosf(dot);
		DirectX::XMFLOAT3 up = { 0,1,0 };
		// ��x�N�g���Ƒ���L����->�^�[�Q�b�g�x�N�g���̓���			
		float axisXDot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&up), OperatorToTarget));
		// ��x�N�g���Ƒ���L����->�^�[�Q�b�g�x�N�g���ɑ΂��ăJ�������㉺�ǂ���ɂ��邩�̔���
		// �J�������������̏ꍇ
		if (axisXDot < 0)
		{
			angle.x = dot;
		}
		// �J������������̏ꍇ
		else
		{
			angle.x = -dot;
		}
	}
}

//���񏈗�
void Trap::Turn(float elapsedTime, float vx, float vz, float speed)
{
	float rotateSpeed = speed * elapsedTime;

	//�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	float Length = sqrtf((vx * vx) + (vz * vz));
	if (Length == 0.0f)
	{
		center = true;
		return;
	}

	//�i�s�x�N�g����P�ʃx�N�g����
	vx /= Length;
	vz /= Length;

	//���g�̉�]�l����O���������߂�
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// ��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = (frontX * vx) + (frontZ * vz);

	// ���ϒl��-1.0~1.0�ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
	// �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;
	if (rotateSpeed > rot)rotateSpeed = rot;

	//���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = (frontZ * vx) - (frontX * vz);

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
	// ���E������s�����Ƃɂ���č��E��]��I������
	if (cross < 0.0f)
	{
		angle.y -= rotateSpeed;
		center = false;
	}
	else
	{
		angle.y += rotateSpeed;
		center = false;
	}
}

// �꒣��ݒ�
void Trap::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}

void Trap::InputDamage(int damage) {
	if (health > 0)
	{
		health -= damage;
		if (health < 0)
		{
			health = 0;
		}
	}
}