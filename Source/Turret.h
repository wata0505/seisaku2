#pragma once

#include "Trap.h"
#include "ObjectManager.h"

class Turret : public Trap
{
public:
	Turret();
	~Turret() {}

public:
	// �X�V����
	void Update(float elapsedTime) override;
	// �`�揈��
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) override;
	// �c���G�t�F�N�g�`�揈��
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, ModelShader* shader) override;
	// �f�o�b�O�v���~�e�B�u�`�揈��
	void DrawDebugPrimitive() override;
	// �f�o�b�O���\��
	void DrawDebugGUI() override;

private:
	// �y��s��X�V����
	void UpdateTransformBody(int axisType, int lengthType);

	// �e�ƃG�l�~�[�̏Փˏ���
	void CollisionProjectilesVsEnemies();

	// �ҋ@�X�e�[�g�֑J�ڂƍX�V����
	void TransitionIdleState();
	void UpdateIdleState(float elapsedTime);

	// �U���X�e�[�g�֑J�ڂƍX�V����
	void TransitionAttackState();
	void UpdateAttackState(float elapsedTime);

	// ���S�X�e�[�g�ƍX�V����
	void TransitionDeadState();
	void UpdateDeadState(float elapsedTime);

private:
	// �^���b�g�A�j���[�V����
	enum  TurretAnimation
	{
		Attack,
	};

	// �X�e�[�g
	enum class State
	{
		Idle,
		Attack,
		Dead,
	};

private:
	std::unique_ptr<Model>	gunModel = nullptr;										// �e�g���f��
	std::unique_ptr<Model>	bodyModel = nullptr;									// �y�䃂�f��
	std::shared_ptr<Model>	beem = nullptr;											// �e���f��
	ObjectManager			objectManager;											// �e
	State					state = State::Idle;									// ���
	DirectX::XMFLOAT4X4		bodyTransform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };	// �y��s��
	int						coolTime = 0;											// �N�[���^�C��
};