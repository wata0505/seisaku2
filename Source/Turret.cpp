#include "Turret.h"
#include "TrapManager.h"
#include "Graphics.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "ProjectileStraite.h"
#include "AudioAll.h"
#include "Calculation.h"

// �R���X�g���N�^
Turret::Turret()
{
	beem = std::make_unique<Model>(".\\resources\\Cube.fbx", true);
	beem->ModelSerialize(".\\resources\\Cube.fbx");
	beem->ModelRegister(".\\resources\\Cube.fbx");
	beem->UpdateBufferData(transform);

	gunModel = std::make_unique<Model>(".\\resources\\Trap\\Sentorygun\\Taret_GUN.fbx", true);
	gunModel->AppendAnimations(".\\resources\\Sentorygun\\GUN_motionBullet.fbx", 0);
	gunModel->ModelSerialize(".\\resources\\Trap\\Sentorygun\\Taret_GUN.fbx");
	gunModel->ModelRegister(".\\resources\\Trap\\Sentorygun\\Taret_GUN.fbx", "Taret_BODY.fbm\\TARET_GUN_BaseColor.png");

	bodyModel = std::make_unique<Model>(".\\resources\\Trap\\Sentorygun\\Taret.fbx", true);
	bodyModel->ModelSerialize(".\\resources\\Trap\\Sentorygun\\Taret.fbx");
	bodyModel->ModelRegister(".\\resources\\Trap\\Sentorygun\\Taret.fbx", "Taret.fbm\\TARET_BODY_BaseColor.png");

	scale.x = scale.y = scale.z = 150.0f;			// �T�C�Y�ݒ�
	radius = 2.3f;									// ���a�ݒ�
	height = 10.0f;									// �����ݒ�
	territoryRange = 40.0f;							// �e���g���[�͈͐ݒ�
	notAttackRange = 15.0f;							// �U���s�͈͐ݒ�
	maxHealth = 100;								// �ő�̗͐ݒ�
	health = maxHealth;								// �̗͐ݒ�
	attack = 1;										// �_���[�W�ʐݒ�
	type = Trap::TrapType::TrapTurret;				// �g���b�v��ސݒ�
	
	// �ҋ@�X�e�[�g�֑J��
	TransitionIdleState();

	// �z���O�����V�F�[�_�[��񏉊���
	HologramShaderDataInitialize(-2.5f, 2.0f);

	UpdateTransform(0, 0);							// �e�g�s��X�V����
	UpdateTransformBody(0, 0);						// �y��s��X�V����
	gunModel->UpdateBufferData(transform);			// �e�g�`����X�V����
	bodyModel->UpdateBufferData(bodyTransform);		// �y��`����X�V����
}

// �X�V����
void Turret::Update(float elapsedTime)
{
	// �o�ߎ��ԉ��Z
	progressTimer += elapsedTime;
	// �V�F�[�_�[��񒲐�
	gunModel->ShaderAdjustment(glitchScale, progressTimer, maxHeight);
	bodyModel->ShaderAdjustment(glitchScale, progressTimer, maxHeight);

	// �z���O�����V�F�[�_�[�X�V����
	UpdateHologramShader(elapsedTime, isActivate);

	// �ݒu��
	if (isActivate)
	{
		hologramColor = 2.0f;										// �z���O�����F(��)�ݒ�

		// �X�e�[�g���̍X�V����
		switch (state)
		{
		case State::Idle:		// �ҋ@
			UpdateIdleState(elapsedTime);
			break;
		case State::Attack:		// �U��
			UpdateAttackState(elapsedTime);
			break;
		case State::Dead:		// ���S
			UpdateDeadState(elapsedTime);
			break;
		}
	}
	// �ݒu�O
	else
	{
		hologramColor = TrapManager::Instance().GetHologramColor();	// �ݒu�s�F�ݒ�
	}

	UpdateTransform(0, 0);		// �e�g�s��X�V����
	UpdateTransformBody(0, 0);	// �y��s��X�V����

	// �A�j���[�V�����X�V
	gunModel->UpdateAnimation(elapsedTime, "root_x");
	gunModel->UpdateSubAnimation(elapsedTime, "root_x");

	gunModel->UpdateBufferData(transform);		// �e�g�`����X�V����
	bodyModel->UpdateBufferData(bodyTransform);	// �y��`����X�V����

	// �I�u�W�F�N�g�X�V����
	objectManager.Update(elapsedTime * 1.5f);

	// �e�ƓG�̏Փˏ���
	CollisionProjectilesVsEnemies();

#if 0
	hpRenderFlag = Collision::IntersectFanVsSphere(
		Camera::Instance().GetEye(),
		Camera::Instance().GetFront(),
		Camera::Instance().GetFovY(),
		Camera::Instance().GetFarZ(),
		position,
		radius);
#endif
}

// �`�揈��
void Turret::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
	shader->Draw(deviceContext, gunModel.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor, 0.0f, 0.0f, 1.0f });
	shader->Draw(deviceContext, bodyModel.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor, 0.0f, 0.0f, 1.0f });
}
// �c���G�t�F�N�g�`�揈��
void Turret::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, ModelShader* shader)
{
	objectManager.Render(deviceContext.Get(), shader);
}
// �f�o�b�O�v���~�e�B�u�`�揈��
void Turret::DrawDebugPrimitive()
{
	// ���N���X�̃f�o�b�O�v���~�e�B�u�`��
	//Trap::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//�Փ˔���p�̃f�o�b�N�~����`��
	debugRenderer->DrawCylinder({ position.x,6.0f,position.z }, radius, height, DirectX::XMFLOAT4(1, 0, 1, 1));
	
	//�Փ˔���p�̃f�o�b�N�~����`��
	//debugRenderer->DrawCylinder({ position.x,-2.5f,position.z }, notAttackRange, height, DirectX::XMFLOAT4(1, 0, 0, 1));
	
	// �꒣��͈͂��f�o�b�O�~���`��
	//debugRenderer->DrawCylinder({ territoryOrigin.x,-2.5f,territoryOrigin.z }, territoryRange, height, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}
// �f�o�b�O���\��
void Turret::DrawDebugGUI()
{
	// �g�����X�t�H�[��
	if (ImGui::CollapsingHeader("Turret", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::TreeNode("Hologram"))
		{
			ImGui::SliderFloat("scanTimer", &scanTimer, -1.0f, 2.0f);
			ImGui::SliderFloat("glowTimer", &glowTimer, -1.0f, 2.0f);
			ImGui::SliderFloat("hologramTimer", &hologramTimer, -1.0f, 2.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Glitch"))
		{			
			ImGui::SliderFloat("glitchIntensity", &glitchIntensity, 0.0f, 1.0f);
			ImGui::SliderFloat("glitchScale", &glitchScale, 1.0f, 50.0f);
			ImGui::TreePop();
		}
	}
}

// �y��s��X�V����
void Turret::UpdateTransformBody(int axisType, int lengthType)
{
	const DirectX::XMFLOAT4X4 coordinateSystemTransforms[] = 
	{
		{ -1,0,0,0, 0,1, 0,0, 0,0,1,0, 0,0,0,1 },		// 0:RHS Y-UP�@�E��n
		{  1,0,0,0, 0,1, 0,0, 0,0,1,0, 0,0,0,1 },		// 1:LHS Y-UP�@����n
		{ -1,0,0,0, 0,0,-1,0, 0,1,0,0, 0,0,0,1 },		// 2:RHS Z-UP�@�E��n
		{  1,0,0,0, 0,0, 1,0, 0,1,0,0, 0,0,0,1 },		// 3:LHS Z-UP�@����n
	};

	const float scaleFactor[] = { 0.01f, 1.0f };

	DirectX::XMMATRIX C = { DirectX::XMLoadFloat4x4(&coordinateSystemTransforms[axisType]) * DirectX::XMMatrixScaling(scaleFactor[lengthType], scaleFactor[lengthType], scaleFactor[lengthType]) };
	//�X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//��]�s����쐬
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0, 0, angle.z);
	//�ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y - 2.5f, position.z);//scale=150
	//DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y - 5.0f, position.z);//scale=300
	//3�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = C * S * R * T;
	//�v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&bodyTransform, W);
}

// �e�ƓG�̏Փˏ���
void Turret::CollisionProjectilesVsEnemies()
{
	// �S�Ă̒e�ۂƂ��ׂĂ̓G�𑍓�����ŏՓ˔���
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	int projectileCount = objectManager.GetObjectCount();
	for (int i = 0; i < projectileCount; ++i)
	{
		Object* object = objectManager.GetObjectW(i);
		for (int j = 0; j < enemyCount; ++j)
		{
			// �Փˏ���
			Enemy* enemy = enemyManager.GetEnemy(j);
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsCylinder(
				object->GetPosition(),
				object->GetRadius(),
				enemy->GetPosition(),
				enemy->GetRadius(),
				enemy->GetHeight(),
				outPosition))
			{
				// �_���[�W��^����				
				if (enemy->ApplyDamage(attack, 0.0f))
				{
					// �I�u�W�F�N�g��ނ��r�[���Ȃ�
					if (object->GetType() == Type::Beem)
					{
						ParticleSystem::Instance().BoomEffect(object->GetPosition(), 1, int(EffectTexAll::EfTexAll::BlueThader), 3.0f, 0.5f, { NULL, NULL, 2, 1 });
						ParticleSprite* particleSprite = new ParticleSprite(object->GetPosition(), { NULL, NULL, NULL }, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::BlueThader), 200, 1.5f, 0.0f, true, 0.015f, 0.06f, { 0.0f, 0.0f, 1.0f, 1.0f });
						continue;
					}
				}
			}
		}
	}
}

// �ҋ@�X�e�[�g�֑J�ڂƍX�V����
void Turret::TransitionIdleState()
{
	state = State::Idle;				// �ҋ@���
	gunModel->PlayAnimation(-1, false);	// �A�j���[�V�����w��Ȃ�
}
void Turret::UpdateIdleState(float elapsedTime)
{
	// �z���O�������I��������
	if (GetHologramTimer() >= 1.0f)
	{
		// �v���C���[���G
		if (SearchEnemy(territoryRange, notAttackRange))
		{
			// ����������ǐՃX�e�[�g�֑J��
			TransitionAttackState();
		}
	}

	// �̗͂��c���Ă��Ȃ����
	if (health <= 0)
	{
		// ���S�X�e�[�g�֑J��
		TransitionDeadState();
	}
}

// �U���X�e�[�g�֑J�ڂƍX�V����
void Turret::TransitionAttackState()
{
	state = State::Attack;														// �U�����
	gunModel->PlayAnimation(static_cast<int>(TurretAnimation::Attack), true);	// �U���A�j���[�V�����Đ�
}
void Turret::UpdateAttackState(float elapsedTime)
{
	// ���񏈗�
	TurnToTarget(elapsedTime, turnSpeed);

	coolTime--;			// �N�[���^�C������
	// �N�[���^�C�����c���Ă��Ȃ����
	if (coolTime <= 0)
	{	
		ProjectileStraite* projectile = new ProjectileStraite(&objectManager);
		projectile->TurretLaunch(beem, 0.4f, 1.5f, position, targetPosition, angle.y, Type::Beem, (int)EffectTexAll::EfTexAll::BlueThader, 1.0f, 1.0f, 0.0f);
		float volume = 0.3f;
		// �J��������̋����ɉ����ĉ��ʒ���
		{
			float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&Camera::Instance().GetEye()), DirectX::XMLoadFloat3(&projectile->GetPosition()))));
			if (length > 20.0f)
			{
				volume = (20.0f / length) * 0.3f;
			}
			else
			{
				volume = 0.3f;
			}
		}
		// ���ˉ��Đ�
		AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Shot)->Play(false, volume);
		coolTime = 20;	// �N�[���^�C���ݒ�
	}

	// �U���A�j���[�V�������I����Ĕ͈͓��ɓG�����Ȃ����
	if (!SearchEnemy(territoryRange, notAttackRange))
	{
		//�ҋ@�X�e�[�g�֑J��
		TransitionIdleState();
	}

	// �̗͂��c���Ă��Ȃ����
	if (health <= 0)
	{
		// ���S�X�e�[�g�֑J��
		TransitionDeadState();
	}
}

// ���S�X�e�[�g�֑J�ڂƍX�V����
void Turret::TransitionDeadState()
{
	state = State::Dead;				// ���S���
	gunModel->PlayAnimation(-1, true);	// �A�j���[�V�����w��Ȃ�
	glitchIntensity = 1.0f;				// �O���b�`�ݒ�
}
void Turret::UpdateDeadState(float elapsedTime)
{
	brokenTimer -= elapsedTime;	// �j�󎞂̔j���܂ł̎��Ԍ���
	// �j�󎞂̔j���܂ł̎��Ԃ��c���Ă��Ȃ����
	if (brokenTimer <= 0.0f)
	{
		Destroy();	// �j��
	}
}