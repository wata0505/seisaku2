#pragma once
#include<DirectXMath.h>
#include "shader.h"
#include "Model.h"
#include "Trap.h"

class Turret:public Trap
{
public:
	Turret();
	~Turret();
public:
	//�X�V����
	void Update(float elapsed_time)override;
	// �`�揈��
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;
	
	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive()override;


	
private:
	//�s��X�V����
	void UpdateTransform2(int axisType, int lengthType);

	// �ҋ@�X�e�[�g�֑J��
	void TransitionIdleState();

	// �ҋ@�X�e�[�g�X�V����
	void UpdateIdleState(float elapsedTime);

	// �U���X�e�[�g�֑J��
	void TransitionAttackState();

	// �U���X�e�[�g�X�V����
	void UpdateAttackState(float elapsedTime);

private:
	enum  TurretAnimation
	{
		Attack,
	};
	// �X�e�[�g
	enum class State
	{
		Idle,
		Attack,
	};

	State state = State::Idle;

	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<Model> model2 = nullptr;
	//�`����i�[
	std::vector<SkinnedMeshResouurce::constants> renderdata;
	std::vector<SkinnedMeshResouurce::constants> renderdata2;

	DirectX::XMFLOAT4X4		transform2 = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

};