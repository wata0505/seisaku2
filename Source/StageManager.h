#pragma once

#include <vector>
#include "Stage.h"

class StageManager
{
private:
	StageManager();
	~StageManager() {}

public:
	static StageManager& Instance()
	{
		static StageManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader);
	// �C���X�^���V���O�`�揈��
	void InstaningRender(ID3D11DeviceContext* deviceContext, ModelShader* shader);

	// ���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

	// �X�e�[�W�o�^
	void Register(Stage* stage);
	// �X�e�[�W�S����
	void Clear();

private:
	std::unique_ptr<Model>	obj = nullptr;
	std::vector<Stage*>		stages;
	InstancingConstants		constants;
	int						instangNo = 0;
};