#include "Character.h"
#include "StageManager.h"
#include "Mathf.h"
#include "Base.h"
#include "Graphics.h"
#include "AudioAll.h"
#include "Camera.h"

//�s��X�V����
void Character::UpdateTransform(int axisType,int lengthType)
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
	//DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	//DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	//DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	//DirectX::XMMATRIX R = Y * X * Z;
	//�ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	//3�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = C * S * R * T;
	//�v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

// �_���[�W��^����
bool Character::ApplyDamage(int damage, float invincibleTime, bool DamageFlag)
{
	// �_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
	if (damage <= 0)
		return false;

	// ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
	if (health <= 0)return false;
	saveDamage = 0;
	if (invincibleTimer <= 0) {
		// �_���[�W����
		health -= damage;
		saveDamage = damage;
		invincibleTimer = invincibleTime;
		damageFlag = DamageFlag;
	}
	else {
		return false;
	}

	// ���S�ʒm
	if (health <= 0)
	{
		OnDead();
	}
	// �_���[�W�ʒm
	else {
		OnDamaged();
	}

	// ���N��Ԃ��ύX�����ꍇ��true��Ԃ�
	return true;
}
void Character::SetInvincibleTime(float invincibleTime) {
	invincibleTimer = invincibleTime;
}

// �Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	// ���͂ɗ͂�������
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;

}

// �m�b�N�o�b�N
void Character::KnockBack(Character& taisyou, DirectX::XMFLOAT3 mypos, DirectX::XMFLOAT3 targetpos, float power, float powerY)
{
	DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&mypos);
	DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&targetpos);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(B, A);
	DirectX::XMVECTOR Nvec = DirectX::XMVector3Normalize(Vec);
	Nvec = DirectX::XMVectorScale(Nvec, power / taisyou.GetMass());
	DirectX::XMFLOAT3 inpulse;
	DirectX::XMStoreFloat3(&inpulse, Nvec);
	inpulse.y = powerY / taisyou.GetMass();
	taisyou.AddImpulse(inpulse);
}

//�ړ�����
void Character::MoveInput(float vx, float vz, float speed)
{
	// �ړ������x�N�g����ݒ�
	moveVecX = vx;
	moveVecZ = vz;
	// �ő呬�x����
	maxMoveSpeed = speed;
}

//���񏈗�
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	float rotateSpeed = speed * elapsedTime;

	//�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	float Length = sqrtf((vx * vx) + (vz * vz));
	if (Length == 0.0f) return;

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
	}
	else
	{
		angle.y += rotateSpeed;
	}


}
//�������񏈗�
void Character::VerticalTurn(float elapsedTime, float vx, float vz, float vy, float speed)
{
	float rotateSpeed = speed * elapsedTime;

	//�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	float Length = sqrtf((vy * vy));
	if (Length == 0.0f) return;

	//�i�s�x�N�g����P�ʃx�N�g����
	vx /= Length;
	vz /= Length;
	//vy /= Length;

	//���g�̉�]�l����O���������߂�
	float frontX = cosf(angle.x)*sinf(angle.y);
	float frontY = sinf(angle.x);
	float frontZ = cosf(angle.x)*cosf(angle.y);
	float a = 1.0 - (vy * vy);
	// ��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = -(frontY * vy) + a;

	// ���ϒl��-1.0~1.0�ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
	// �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;
	if (rotateSpeed > rot)rotateSpeed = rot;

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
	// ���E������s�����Ƃɂ���č��E��]��I������
	
	angle.x += rotateSpeed;

}
void Character::JumpInput(float speed)
{
	// ������̗͂�ݒ�
	velocity.y = speed;
	jumpflag = true;
}

//���x�����X�V
void Character::UpdateVelocity(float elapsedTime)
{
	// �o�߃t���[��
	float elapsedFrame = 60.0f * elapsedTime;

	// �������͍X�V����
	UpdateVerticalVelocity(elapsedFrame);

	// �������͍X�V����
	UpdateHorizontalVelocity(elapsedFrame);

	// �����ړ��X�V����
	UpdateVerticalMove(elapsedTime);

	// �����ړ��X�V����
	UpdateHorizontalMove(elapsedTime);

}

void Character::UpdateInvincibleTimer(float elapsedTime)
{
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= elapsedTime;
	}
	else
	{
		damageFlag = false;
	}
}

// �������͍X�V����
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
	// �d�͏���
	velocity.y += gravity * elapsedFrame;
}

// �����ړ��X�V����
void Character::UpdateVerticalMove(float elapsedTime)
{
	// ���������̈ړ���
	float my = velocity.y * elapsedTime;

	slopeRate = 0.0f;

	// �L�����N�^�[��Y�������ƂȂ�@���x�N�g��
	DirectX::XMFLOAT3 normal = { 0,1,0 };

	// ������
	if (my < 0.0f)
	{
		// ���C�̊J�n�ʒu�͑�����菭����
		DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
		// ���C�̏I�_�ʒu�͈ړ���̈ʒu
		DirectX::XMFLOAT3 end = { position.x,position.y + my - 0.1f,position.z };
		if (velocity.x == 0 && velocity.z == 0) {
			if (isGround) return;
		}
		 
		// ���C�L���X�g�ɂ��n�ʔ���
		HitResult hit;
		//if (health > 0) {
			if (StageManager::Instance().RayCast(start, end, hit))
		    //if (position.y < 0.1)
			{
				// �@���x�N�g���擾
				normal = hit.normal;

				// �n�ʂɐڒn���Ă���
				position = hit.position;
				//position.y = 0;
				// �X�Η��̌v�Z
				//float teihen = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
				//slopeRate = hit.normal.y / (teihen + hit.normal.y);

				// ���n����
				if (!isGround)
				{
					OnLanding();
					jumpflag = false;
				}
				isGround = true;
				velocity.y = 0.0f;
			}
			else
			{
				// �󒆂ɕ����Ă���
				position.y += my;
				isGround = false;
			}
			if (position.y < deathHight) {
				OnDead();
				health = 0;
				fallDeath = true;
			}
	}
	// �㏸��
	else if (my > 0.0f)
	{
		
		// ���C�̊J�n�ʒu�͑�����菭����
		DirectX::XMFLOAT3 start = { position.x,position.y,position.z };
		// ���C�̏I�_�ʒu�͈ړ���̈ʒu
		DirectX::XMFLOAT3 end = { position.x,position.y - 30,position.z };

		// ���C�L���X�g�ɂ��n�ʔ���
		HitResult hit;
		//if (health > 0) {
		//	if (StageManager::Instance().RayCast(start, end, hit))
		//	{
		//		// �n�ʂ���̋��������ꂷ���Ă���Ώ㏸��~
		//		if (hit.position.y + altitude < position.y) {
		//			//position.y = hit.position.y + altitude;
		//			my = 0.0f;
		//		}			
		//	}
		//	
		//}
		position.y += my;
		isGround = false;
	}

	// �n�ʂ̌����ɉ����悤��XZ����]
	{
		//Y�����@���x�N�g�������Ɍ����I�C���[�p��]���Z�o����
		//angle.x = atan2f(normal.z, normal.y);
		//angle.z = -atan2f(normal.x, normal.y);
		//float anglex = atan2f(normal.z, normal.y);
		//float anglez = -atan2f(normal.x, normal.y);
		//// ���`�⊮�Ŋ��炩�ɉ�]����
		//angle.x = Mathf::Lerp(angle.x, anglex, 0.2f);
		//angle.z = Mathf::Lerp(angle.z, anglez, 0.2f);

	}
	
}

// �������͍X�V����
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
#if 0
	DirectX::XMFLOAT3 outPos;
	if (StageManager::Instance().PillarVS(position, radius, height, outPos)) {
		position = outPos;
	}
	if (Base::Instance().PillarVS(position, radius, height, outPos)) {
		position = outPos;
	}
#endif
	// XZ���ʂ̑��͂���������
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// ���C��
		float friction = this->friction * elapsedFrame;

		// ���C�ɂ�鉡�����̌�������
		if (length > friction)
		{
			//�P�ʃx�N�g����
			float vx = velocity.x / length;
			float vz = velocity.z / length;

			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
		}
		// �������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
		else
		{
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}
	// XZ���ʂ̑��͂���������
	if (length <= maxMoveSpeed)
	{
		// �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
		float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		if (moveVecLength > 0.0f)
		{
			//������
			float acceleration = this->acceleration * elapsedFrame;
			//�ړ��x�N�g���ɂ���������
			velocity.x += moveVecX * acceleration;
			velocity.z += moveVecZ * acceleration;

			//�ő呬�x����
			float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
			if (length > maxMoveSpeed)
			{
				float vx = velocity.x / length;
				float vz = velocity.z / length;

				velocity.x = vx * maxMoveSpeed;
				velocity.z = vz * maxMoveSpeed;
			}
			// �����ŃK�^�K�^���Ȃ��悤�ɂ���
			if (isGround && slopeRate > 0.0f)
			{
				velocity.y -= slopeRate * elapsedFrame;
			}


		}
	}

	// �ړ��x�N�g�������Z�b�g
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

// �����ړ��X�V����
void Character::UpdateHorizontalMove(float elapsedTime)
{
	// �������͗ʌv�Z
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

	if (velocityLengthXZ > 0.0f)
	{
		// �����ړ��l
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;
		//float length = sqrtf(mx * mx + mz * mz);
		// ���C�̊J�n�ʒu�ƏI�_�ʒu
		DirectX::XMFLOAT3 start = { position.x,position.y + height * 0.5f,position.z };
		DirectX::XMFLOAT3 end = { position.x + mx, position.y + height * 0.5f, position.z + mz };
#if 0
		// �����ړ��l
		DirectX::XMFLOAT3 velo = { velocity.x, 0.0f, velocity.z };
		//float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&velo)));
		//float length = abs(mx) + abs(mz);
		float length = 0.0f;
		//DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velocity)), radius);
		DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velo)), radius);
		//Velocity = DirectX::XMVectorSetY(Velocity, 0.0f);
		//DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velocity)), radius);
		//Velocity = DirectX::XMVectorSetY(Velocity, 0.0f);
		end = { position.x + mx + DirectX::XMVectorGetX(Velocity), position.y + height * 0.5f, position.z + mz + DirectX::XMVectorGetZ(Velocity) };
		DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
		//�Փ˔���p�̃f�o�b�N����`��
		debugRenderer->DrawSphere(end, radius * 0.5f, DirectX::XMFLOAT4(1, 0, 0, 1));
#if 1
		if (!OmissionRaycastContents(start, end, Velocity))
		{
			// ��
			DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
			DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
			DirectX::XMVECTOR Cross = DirectX::XMVector3Cross(Up, Velocity);
			end = { position.x + mx + DirectX::XMVectorGetX(Cross), position.y + height * 0.5f, position.z + mz + DirectX::XMVectorGetZ(Cross) };
			debugRenderer->DrawSphere(end, radius * 0.5f, DirectX::XMFLOAT4(0, 1, 0, 1));
			if (!OmissionRaycastContents(start, end, Cross))
			{
				// �E
				Cross = DirectX::XMVectorNegate(Cross);
				end = { position.x + mx + DirectX::XMVectorGetX(Cross), position.y + height * 0.5f, position.z + mz + DirectX::XMVectorGetZ(Cross) };
				debugRenderer->DrawSphere(end, radius * 0.5f, DirectX::XMFLOAT4(0, 0, 1, 1));
				if (!OmissionRaycastContents(start, end, Cross))
				{
					//end = { position.x + lastPoint.x, position.y + height * 0.5f, position.z + lastPoint.y };
					//debugRenderer->DrawSphere(end, radius * 0.5f, DirectX::XMFLOAT4(1, 1, 0, 1));
					//if (!OmissionRaycastContents(start, end, Velocity))
					{
						// �ړ�
						position.x += mx;
						position.z += mz;
					}
					if (position.z < -139) {
						position.z = -139;
					}
					if (position.z > 59) {
						position.z = 59;
					}
					if (position.x > 159) {
						position.x = 159;
					}
					if (position.x < 41) {
						position.x = 41;
				}
				}
			}
		}
#else
		if (!OmissionRaycastContents(start, end, Velocity))
		{
			// �ړ�
			position.x += mx;
			position.z += mz;
		}
#endif
#else
		// ���C�L���X�g�ɂ��ǔ���
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			// �ǂ܂ł̃x�N�g��
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

			// �ǂ̖@��
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			// ���˃x�N�g����@���Ɏˉe
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			// �␳�ʒu�̌v�Z
			DirectX::XMVECTOR apos = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			DirectX::XMFLOAT3 aPos;
			DirectX::XMStoreFloat3(&aPos, apos);

			// �ǎC������Ƀ��C�L���X�g
			HitResult hit2;
			if (!StageManager::Instance().RayCast(hit.position, aPos, hit2))
			{
				// �ǎC��ŕ␳�ʒu�Ɉړ�
				position.x = aPos.x;
				position.z = aPos.z;
			}
			else
			{
				position.x = hit2.position.x;
				position.z = hit2.position.z;
			}
		}
		else
		{
			// �ړ�
			position.x += mx;
			position.z += mz;
		}
		if (position.z < -139.0f)
		{
			position.z = -139.0f;
		}
		if (position.z > 59.0f)
		{
			position.z = 59.0f;
		}
		if (position.x > 159.0f)
		{
			position.x = 159.0f;
		}
		if (position.x < 41.0f)
		{
			position.x = 41.0f;
		}
#endif
	}
}
// ���C�L���X�g�֐��ȗ�(�g���܂킵)
bool Character::OmissionRaycastContents(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, DirectX::XMVECTOR Velocity)
{
	// ���C�L���X�g�ɂ��ǔ���
	HitResult hit;
	if (StageManager::Instance().RayCast(start, end, hit))
	{
		lastPoint.x = end.x - start.x;
		lastPoint.y = end.z - start.z;

		// �ǂ܂ł̃x�N�g��
		DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
		DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);

		End = DirectX::XMVectorSubtract(End, Velocity);
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

		// �ǂ̖@��
		DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

		// ���˃x�N�g����@���Ɏˉe
		DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

		// �␳�ʒu�̌v�Z
		DirectX::XMVECTOR apos = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
		DirectX::XMFLOAT3 aPos;
		DirectX::XMStoreFloat3(&aPos, apos);

		DirectX::XMFLOAT3 collectPosition;
		collectPosition = aPos;
		aPos.y = position.y;
		hit.position.y = position.y;

		// �ǎC������Ƀ��C�L���X�g
		HitResult hit2;
		DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
		if (!StageManager::Instance().RayCast(hit.position, aPos, hit2))
		{
			//hit.position.y = position.y;
			hit.normal.y = 0;
			Normal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&hit.normal));
			Vec = DirectX::XMVectorSubtract(Start, DirectX::XMLoadFloat3(&hit.position));
			Dot = DirectX::XMVector3Dot(Vec, Normal);
			DirectX::XMVECTOR Collect = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&aPos), DirectX::XMVectorScale(Normal, radius - DirectX::XMVectorGetX(Dot)));
			float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&hit.position), Collect), Normal));
			//DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&collectPosition), DirectX::XMVectorScale(Normal, radius - DirectX::XMVectorGetX(Dot))));
			//float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&hit.position), DirectX::XMLoadFloat3(&position)), Normal));
			DirectX::XMFLOAT3 collect;
			DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(Collect, DirectX::XMVectorScale(Normal, radius + dot)));
			//DirectX::XMStoreFloat3(&collect, Collect);
			debugRenderer->DrawSphere(position, radius * 0.5f, DirectX::XMFLOAT4(0, 1, 1, 1));
		}
		else
		{
#if 1
			hit2.position.y = position.y;
			hit2.normal.y = 0;
			Normal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&hit2.normal));
			Vec = DirectX::XMVectorSubtract(Start, DirectX::XMLoadFloat3(&hit2.position));
			Dot = DirectX::XMVector3Dot(Vec, Normal);
			//debugRenderer->DrawSphere(collect, radius * 0.5f, DirectX::XMFLOAT4(1, 0, 1, 1));
			DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&hit2.position), DirectX::XMVectorScale(Normal, radius - DirectX::XMVectorGetX(Dot))));
#else
			hit.position.y = position.y;
			hit.normal.y = 0;
			hit2.position.y = position.y;
			hit2.normal.y = 0;
			DirectX::XMVECTOR Normal1 = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&hit.normal));
			DirectX::XMVECTOR Normal2 = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&hit2.normal));
			DirectX::XMVECTOR Pos1 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&hit.position), DirectX::XMVectorScale(Normal1, radius));
			DirectX::XMVECTOR Pos2 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&hit2.position), DirectX::XMVectorScale(Normal2, radius));
			Dot = DirectX::XMVector3Dot(Normal1, Normal2);
			DirectX::XMVECTOR add = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&hit2.position), DirectX::XMLoadFloat3(&hit.position));
			DirectX::XMVECTOR nor = DirectX::XMVectorAdd(Normal1, Normal2);
			DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMVectorScale(add, 0.5f), DirectX::XMVectorScale(DirectX::XMVectorScale(nor, 0.5f), radius)));
#endif
		}
		return true;
	}
	return false;
}
// �z���O�����V�F�[�_�[��񏉊���
void Character::HologramShaderDataInitialize(float minHeight, float maxHeight)
{
	scanTimer = 0.0f;		// �X�L�������C���̎���
	scanBorder = 0.0f;		// �X�L�������C���̕`��͈�
	glowTimer = 0.0f;		// �O���E���C���̎���
	glowBorder = 0.0f;		// �O���E���C���̕`��͈�
	hologramTimer = 0.0f;	// �z���O�������C���̎���
	hologramBorder = 0.0f;	// �z���O�������C���̕`��͈�
	this->minHeight = minHeight;	// �Œ�_
	this->maxHeight = maxHeight;	// �ō��_
	isActiveStart = false;	// �z���O�����V�F�[�_�[���s���t���O
}
// �z���O�����V�F�[�_�[�X�V����
bool Character::UpdateHologramShader(float elapsedTime)
{
	// minHeight : �����ʒu
	// maxHeight : �����ʒu
	// �����Z�o
	float length = maxHeight - minHeight;

	float inverse = -1.0f;
	length *= inverse;
	float startPosition = minHeight;
	if (minHeight < 0.0f)
	{
		startPosition = maxHeight;
	}

	// ���e����
	static constexpr float permissionTimer = 1.0f;
	// �X�L�������C���̎��ԉ��Z(�{��)
	scanTimer += elapsedTime * 2.0f;
	// �X�L�������C���̎��Ԃ����e���Ԉȏ�Ȃ�Œ�
	if (scanTimer >= permissionTimer)
	{
		scanTimer = permissionTimer;
	}
	// �X�L�������C���̎��Ԃ����l(0.4f)�ȏ�Ȃ�
	if (scanTimer >= 0.4f)
	{
		// �O���E���C���̎��ԉ��Z(3/4�{��)
		glowTimer += elapsedTime * 0.75f;
		// �O���E���C���̎��Ԃ����e���Ԉȏ�Ȃ�Œ�
		if (glowTimer >= permissionTimer)
		{
			glowTimer = permissionTimer;
		}
	}
	// �O���E���C���̎��Ԃ����l(0.4f)�ȏ�Ȃ�
	if (glowTimer >= permissionTimer)
	{
		if (hologramTimer <= 0.0f)
		{
			float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&Camera::Instance().GetEye()), DirectX::XMLoadFloat3(&position))));
			float volume = 0.3f;
			if (length > 20.0f)
			{
				volume = (20.0f / length) * 0.3f;
			}
			else
			{
				volume = 0.3f;
			}

			// �z���O��������~�ƍĐ�
			AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Hologram)->Stop();
			AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Hologram)->Play(false, volume);
		}
		// �z���O�������C���̎��ԉ��Z
		hologramTimer += elapsedTime;
		// �z���O�������C���̎��Ԃ����e���Ԉȏ�Ȃ�Œ�
		if (hologramTimer >= permissionTimer)
		{
			hologramTimer = permissionTimer;
		}
	}
		
	// �X�L�������C���̕`��͈͂𑫌��ʒu���獂���Ɏ��Ԋ����ŎZ�o
	scanBorder = length * scanTimer + startPosition;
	// �O���E���C���̕`��͈͂𑫌��ʒu���獂���Ɏ��Ԋ����ŎZ�o
	glowBorder = length * glowTimer + startPosition;
	// �z���O�������C���̕`��͈͂𑫌��ʒu���獂���Ɏ��Ԋ����ŎZ�o
	hologramBorder = length * hologramTimer + startPosition;

	// �z���O�������C���̎��Ԃ����e���Ԉȏ�Ȃ�Œ�
	if (hologramTimer >= permissionTimer)
	{
		return true;
	}
	return false;
}