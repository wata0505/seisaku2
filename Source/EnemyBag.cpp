#include "EnemyBag.h"
#include "Graphics.h"
#include "Mathf.h"
#include "player.h"
#include "StateDerived.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ParticleSprite.h"
#include "Audio.h"
#include "AudioAll.h"
#include "Calculation.h"
#include "FireBall.h"
#include "ParticleSystem.h"
#include "ParticleSprite.h"
#include"Base.h"
// �R���X�g���N�^
EnemyBag::EnemyBag(bool tutorial)
{
	model = std::make_unique<Model>(".\\resources\\enemy\\enemy.fbx", true);
	model->AppendAnimations(".\\resources\\enemy\\Enemy_taikiandmigration.fbx",0);
	model->AppendAnimations(".\\resources\\enemy\\Enemy_attack1.fbx", 0);
	model->AppendAnimations(".\\resources\\enemy\\Enemy_attack2.fbx", 0);
	model->AppendAnimations(".\\resources\\enemy\\Enemy_attack3.fbx", 0);
	model->AppendAnimations(".\\resources\\enemy\\Enemy_damage.fbx", 0);
	model->AppendAnimations(".\\resources\\enemy\\Enemy_Die.fbx", 0);
	model->ModelSerialize(".\\resources\\enemy\\enemy.fbx");
	//model->ModelCreate(".\\resources\\Slime\\Slime.fbx");
	model->ModelRegister(".\\resources\\enemy\\enemy.fbx");
	// ���f�����傫���̂ŃX�P�[�����O
	maxHealth = 30;
	health = maxHealth;
	radius = 0.5f;
	height = 2.0f;
	activeflag = true;
	
	//StateMachine�𐶐����A�K�w�^�X�e�[�g�}�V���ɑΉ�����悤�ɓo�^�X�e�[�g��ύX���Ă����B
	stateMachine = std::make_unique<StateMachine<EnemyBag>>();
	// �X�e�[�g�}�V����1�w�ڂ̃X�e�[�g��o�^(BattleState�����l�̕��@�Ŋe���ǉ����Ă�������)
	stateMachine->RegisterState(new BagSearchState(this));
	stateMachine->RegisterState(new BagBattleState(this));
	stateMachine->RegisterState(new BagRecieveState(this));
	stateMachine->RegisterState(new BagReDamageState(this));
	// �e�e�X�e�[�g�ɃT�u�X�e�[�g��o�^(WanderState�ȊO��2�w�ڂ̃X�e�[�g�����l�̕��@�Ŋe���ǉ����Ă��������B)
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Search), new BagWanderState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Search), new BagIdleState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Battle), new BagPursuitState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Battle), new BagAttackState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Battle), new BagFireBallState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Battle), new BagStandbyState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Battle), new BagRoarState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Battle), new BagBackStepState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::Recieve), new  BagCalledState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::ReDamage), new BagDamageState(this));
	stateMachine->RegisterSubState(static_cast<int>(EnemyBag::BagState::ReDamage), new BagDieState(this));
	// �X�e�[�g���Z�b�g
	stateMachine->SetState(static_cast<int>(BagState::Search));
	mass = 2.0;
	UpdateTransform(0, 0);

	model->UpdateBufferDara(transform);
	//renderdata = model->GetBufferData();

	se[(int)EnemyBagSE::Walk] = Audio::Instance().LoadAudioSource("resources\\Audio\\wolk2.wav");
	se[(int)EnemyBagSE::Run] = Audio::Instance().LoadAudioSource("resources\\Audio\\run3.wav");
	se[(int)EnemyBagSE::Roar] = Audio::Instance().LoadAudioSource("resources\\Audio\\bag1.wav");
	//se[(int)EnemyBagSE::hit] = Audio::Instance().LoadAudioSource("resources\\Audio\\pannti.wav");
	searchRange = 20.0f;

	attackRange = 2.5f;
	scale.x = scale.y = scale.z = 4.0f;
	territoryRange = 10.0f;
	angle.y = DirectX::XMConvertToRadians(180);
	tutorialflag = tutorial;
	eria = 100;
}

// �f�X�g���N�^
EnemyBag::~EnemyBag()
{
	// delete model;
}
//���[�g���[�V����
void EnemyBag::RootMove() {

	DirectX::XMFLOAT2 dir = ForwardToPlayer();
	velocity.x =  dir.x * (model->GetRootPow() * rootSpeed);
	velocity.z =  dir.y * (model->GetRootPow() * rootSpeed);
}
float EnemyBag::MovePow() {
	
	return (velocity.x * velocity.x) + (velocity.z * velocity.z);
}
void EnemyBag::Update(float elapsedTime)
{
	//�`�攻��
	if (reMoveflag)ReMove();
	if (!activeflag)return;
	UpdateRnderflag();
	float ElapsedTime = elapsedTime;
	if (Player::Instance().GetQuickflag()) {
		ElapsedTime *= 0.5;
	}
	if (health <= 0) {
		if (!model->IsPlayAnimation()) {
			//���S���[�V�������I������f���[�g
			//Destroy();
			activeflag = false;
			renderflag = false;
			//position.y = -2000;
		}
		UpdateTransform((int)Character::AxisType::RHSYUP, (int)Character::LengthType::Cm);
		model->UpdateAnimation(ElapsedTime,"pelvis");
		// ���f���s��X�V
		model->UpdateBufferDara(transform);
		//renderdata = model->GetBufferData();
		return;
	}
	
	//�q�b�g�X�g�b�v
	if (Player::Instance().GetAttackHitflag()) ElapsedTime *= hitStop;
	//�X�e�[�g�}�V���X�V
	stateMachine->Update(elapsedTime);



	// ���͏����X�V
	UpdateVelocity(ElapsedTime);

	// ���G���ԍX�V
	UpdateInvincibleTimer(elapsedTime);

	// �I�u�W�F�N�g�s��X�V
	UpdateTransform((int)Character::AxisType::RHSYUP, (int)Character::LengthType::Cm);

	CollisionFireBallVSPlayer();
	model->UpdateAnimation(ElapsedTime,"pelvis",renderflag);
	objectManager.Update(elapsedTime);
	// ���f���s��X�V
	if (renderflag) {
		model->UpdateBufferDara(transform);
	}
	//���f���`����󂯓n��
	//renderdata = model->GetBufferData();
	//SE�A�v�f�[�g
	SeUpdate(elapsedTime);
	enemyTimer++;
}
void EnemyBag::SeUpdate(float elapsedTime) {
	//if (model->GetCurrentAnimationIndex() == (int)EnemyBagAnimation::RunFWD) {
	//
	//}

}
// ���S�������ɌĂ΂��
void EnemyBag::OnDead()
{
	// ���S
	stateMachine->ChangeState(static_cast<int>(EnemyBag::BagState::ReDamage));
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Down)->Play(false, DOWNSE);
	
}

void EnemyBag::OnDamaged()
{
	if (!barrierFlg || saveDamage >= 4) {
		stateMachine->ChangeState(static_cast<int>(EnemyBag::BagState::ReDamage));
	}
	barrierHP -= (--saveDamage) ;
	if (barrierHP < 0 && barrierFlg) {
		barrierFlg = false;
		ParticleSprite* particleSprite = new ParticleSprite(GetEfPos(), { NULL,NULL,NULL }, ParticleSprite::ParticleTriangle, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Flame), 5000, 1.5, 0.2);
	}
	if(tutorialflag)health = maxHealth;
}
void EnemyBag::FireBallShoat()
{
	nodePosition = SearchNodePos("koshi");
    
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	//DirectX::XMFLOAT3 dir;
	FireBall* projectile = new FireBall(&objectManager,0.05);
	DirectX::XMFLOAT3 dir{};
	dir.x = sinf(angle.y);
	dir.y = 0;
	dir.z = cosf(angle.y);
	//�����A�ʒu
	projectile->Launch(dir, nodePosition, angle.y);
}


void EnemyBag::CollisionFireBallVSPlayer()
{
	int projectileCount = objectManager.GetObjectCount();

	for (int i = 0; i < projectileCount; ++i)
	{

		Object* object = objectManager.GetObjectW(i);
		Player& player = Player::Instance();
		if (!player.GetRenderflag())return;
		//�Փˏ���
		DirectX::XMFLOAT3 outPosition;
		if (Collision::IntersectSphereVsCylinder(
			object->GetPosition(),
			object->GetRadius(),
			player.GetPosition(),
			player.GetRadius(),
			player.GetHeight(),
			outPosition))
		{

			if (player.GetGuardflag() && player.GetInvincibleTimer() < 0) {
				//ParticleSprite* particleSprite = new ParticleSprite(player.GetEfPos(), { NULL,NULL,NULL }, ParticleSprite::ParticleSquare, ParticleSprite::Expansion, (int)EffectTexAll::EfTexAll::BlackThunder, 3, 0.3);
				player.SetCounterflag(true);
			}
			if (player.GetCounterflag())return;
			if (player.GetState() == Player::State::Guard && player.GetInvincibleTimer() < 0) {
				ParticleSprite* particleSprite = new ParticleSprite(nodePosition, { NULL,NULL,NULL }, ParticleSprite::ParticleImpact, ParticleSprite::Expansion, (int)EffectTexAll::EfTexAll::Impact, efMax, efLife);
				player.PlayerKnockBack(this->position, player.GetPosition(), 20, 0, 1);
				player.SetInvincibleTime(object->GetInvincibleTime());
				return;
			}
			// �_���[�W��^����
			if (player.ApplyDamage(object->GetDamage(), object->GetInvincibleTime()))
			{

				player.PlayerKnockBack(this->position, player.GetPosition(), 20, 0, 1);
				//EffectAll::Instance().swordEffect->Play(player.GetPosition(), 0.2);
				//�e�۔j��
				AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Hit)->Play(false, SE);
			}
		}

	}
}
void EnemyBag::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{

	//if (health <= 0) {
	//	return;
	//}
	if(renderflag)shader->Draw(dc, model.get());
}


void EnemyBag::DrawDebugPrimitive()
{
	// ���N���X�̃f�o�b�O�v���~�e�B�u�`��
	Enemy::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �꒣��͈͂��f�o�b�O�~���`��
	debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

	// �^�[�Q�b�g�ʒu���f�o�b�O���`��
	debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	if (attackFlg) {
		debugRenderer->DrawSphere(nodePosition, 0.5, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	// ���G�͈͂��f�o�b�O�~���`��
	debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
}





// �v���C���[�̎�������
void EnemyBag::BattleMove(bool leftflag,float elapsedTime, float speedRate)
{
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	DirectX::XMFLOAT2 dir = ForwardToPlayer();
	// �ړ�����
	if (leftflag) {
		MoveInput(-dir.y, dir.x, moveSpeed * speedRate);
	}
	else
	{
		MoveInput(dir.y, -dir.x, moveSpeed * speedRate);
	}
	
	Turn(elapsedTime, dir.x, dir.y, turnSpeed * speedRate);
	
}

void EnemyBag::FacePlayer(float elapsedTime, float speedRate)
{
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	DirectX::XMFLOAT2 dir = ForwardToBase();
	//�v���C���[�Ɍ���
	Turn(elapsedTime, dir.x, dir.y, turnSpeed * speedRate);

}


void EnemyBag::MoveToTarget(float elapsedTime, float speedRate)
{
	// �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
	float vx = targetPosition.x - position.x;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;
	// �ړ�����
	MoveInput(vx, vz, moveSpeed * speedRate);
	if (!SearchPlayer()) {
		Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
	}
	else
	{
		//const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
		const DirectX::XMFLOAT3& playerPosition = Base::Instance().GetPos();
		float vx = playerPosition.x - position.x;
		float vz = playerPosition.z - position.z;
		float dist = sqrtf(vx * vx + vz * vz);
		vx /= dist;
		vz /= dist;
		Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
	}
}
// �p�j�X�e�[�g�֑J��
bool EnemyBag::SearchPlayer()
{
	// �v���C���[�Ƃ̍��፷���l������3D�ŋ������������
	//const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	const DirectX::XMFLOAT3& playerPosition = Base::Instance().GetPos();
	DirectX::XMFLOAT3 vec = Vector3::Subset(playerPosition, position);
	float dist = Vector3::Lenght(vec);
	//�����Ŏ��E�ɓ����Ă��邩
	if (dist < searchRange)
	{
		float distXZ = sqrtf(vec.x * vec.x + vec.z * vec.z);
		// �P�ʃx�N�g����
		vec.x /= distXZ;
		vec.z /= distXZ;
		// �����x�N�g����
		float frontX = sinf(angle.y);
		float frontZ = cosf(angle.y);
		// 2�̃x�N�g���̓��ϒl�őO�㔻��
		float dot = (frontX * vec.x) + (frontZ * vec.z);
		if (dot > 0.0f)
		{
			return true;
		}
	}
	return false;
}
//�v���C���[�ւ̕������擾����
DirectX::XMFLOAT2 EnemyBag::ForwardToPlayer() {

	const DirectX::XMFLOAT3& playerPosition = Player::Instance().GetPosition();
	float vx = playerPosition.x - position.x;
	float vz = playerPosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;
	return {vx,vz};
}
DirectX::XMFLOAT2 EnemyBag::ForwardToBase() {

	const DirectX::XMFLOAT3& basePosition = Base::Instance().GetPos();
	float vx = basePosition.x - position.x;
	float vz = basePosition.z - position.z;
	float dist = sqrtf(vx * vx + vz * vz);
	vx /= dist;
	vz /= dist;
	return { vx,vz };
}
DirectX::XMFLOAT3 EnemyBag::SearchNodePos(const char* nodeName) {
	Animation::Keyframe::node* leftHandBone = model->FindNode(nodeName);
	XMStoreFloat4x4(&world, XMLoadFloat4x4(&leftHandBone->globalTransform) * XMLoadFloat4x4(&transform));
	DirectX::XMFLOAT3 node;
	// �m�[�h�ʒu�擾
	node.x = world._41;
	node.y = world._42;
	node.z = world._43;
	return node;
}
void EnemyBag::ReMove()
{
	activeflag = true;
	health = 30;
	float yaw = DirectX::XMConvertToRadians(rand() % 360);
	DirectX::XMFLOAT2 dir;
	dir.x = sinf(yaw);
	dir.y = cosf(yaw);
	int len = rand() % 100 + 5;
	position = DirectX::XMFLOAT3(dir.x * len, 0.0f, dir.y * len);
	SetTerritory(position, 10.0f);
	reMoveflag = false;
	stateMachine->SetState(static_cast<int>(BagState::Search));
}

// �f�o�b�O�G�l�~�[���\��
//void EnemyBag::DrawDebugGUI()
//{
//
//	//std::string str = "";
//
//	// TODO 05 �f�o�b�O�\���p�Ɋe�X�e�[�g����str�ɒǉ����Ȃ���
//	//switch (state) {
//	//case State::Wander:
//	//	str = "Wander";
//	//	break;
//	//
//	//}
//	//switch (static_cast<State>(stateMachine->GetStateIndex())) {
//	//case State::Wander:
//	//	str = "Wander";
//	//	break;
//	//case State::Idle:
//	//	str = "Idle";
//	//	break;
//	//case State::Pursuit:
//	//	str = "Pursuit";
//	//	break;
//	//case State::Attack:
//	//	str = "Attack";
//	//	break;
//	//
//	//
//	//}
//	// TODO 03_07 �f�o�b�O������\���̕ύX
//	std::string str = "";
//	std::string subStr = "";
//	// ���݂̃X�e�[�g�ԍ��ɍ��킹�ăf�o�b�O�������str�Ɋi�[
//	switch (static_cast<State>(stateMachine->GetStateIndex())) {
//	case State::Search:
//		str = "Search";
//		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyBag::Search::Wander))
//		{
//			subStr = "Wander";
//		}
//		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyBag::Search::Idle))
//		{
//			subStr = "Idle";
//		}
//		break;
//	case State::Battle:
//		str = "Battle";
//		// Battle�X�e�[�g�̕\����Search���Q�l�ɍl���Ă��������B
//		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyBag::Battle::Pursuit))
//		{
//			subStr = "Pursuit";
//		}
//		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyBag::Battle::Attack))
//		{
//			subStr = "Attack";
//		}
//		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyBag::Battle::Standby))
//		{
//			subStr = "Stanby";
//		}
//		break;
//	case State::Recieve:
//		str = "Recieve";
//		if (stateMachine->GetState()->GetSubStateIndex() == static_cast<int>(EnemyBag::Recieve::Called))
//		{
//			subStr = "Called";
//		}
//		break;
//
//	}
//
//
//
//	//�g�����X�t�H�[��
//	if (ImGui::CollapsingHeader("EnemyBag", ImGuiTreeNodeFlags_DefaultOpen))
//	{
//		// �ʒu
//		ImGui::InputFloat3("Position", &position.x);
//		// ��]
//		DirectX::XMFLOAT3 a;
//		a.x = DirectX::XMConvertToDegrees(angle.x);
//		a.y = DirectX::XMConvertToDegrees(angle.y);
//		a.z = DirectX::XMConvertToDegrees(angle.z);
//		ImGui::InputFloat3("Angle", &a.x);
//		angle.x = DirectX::XMConvertToRadians(a.x);
//		angle.y = DirectX::XMConvertToRadians(a.y);
//		angle.z = DirectX::XMConvertToRadians(a.z);
//		// �X�P�[��
//		ImGui::InputFloat3("Scale", &scale.x);
//
//		ImGui::Text(u8"State�@%s", str.c_str());
//		ImGui::Text(u8"SubState�@%s", subStr.c_str());
//	}
//}
//void EnemyBag::UpdateVerticalMove(float elapsedTime) {
//	position.y = 0;
//}

// TODO 05_05 ���b�Z�[�W����M�����Ƃ��̏�����ǉ�
bool EnemyBag::OnMessage(const Telegram& telegram)
{
	switch (telegram.msg)
	{
		// ���̃G�l�~�[����Ă΂ꂽ��
	case MESSAGE_TYPE::MsgCallHelp:
		if (!SearchPlayer()&& health > 0)
		{// TODO 05_05 �v���C���[�������Ă��Ȃ��Ƃ���1�w�ڃX�e�[�g��Receive�ɕύX����
			stateMachine->ChangeState(static_cast<int>(BagState::Recieve));
		}
		return true;
		// ���^AI����U������^����ꂽ�Ƃ�
	case MESSAGE_TYPE::MsgGiveAttackRight:
		// TODO 05_05 �U���t���O��true�ɐݒ�
		if (Vector3::Probability(5)) {
			attackFireBallFlg = true;
		}
		attackFlg = true;
		return true;
	}
	return false;
}