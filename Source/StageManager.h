#pragma once

#include <vector>
#include "Stage.h"

// �X�e�[�W�}�l�[�W���[
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
	void Render(ID3D11DeviceContext* dc, ModelShader* shader);

	void InstaningRender(ID3D11DeviceContext* dc, ModelShader* shader);

	// �X�e�[�W�o�^
	void Register(Stage* stage);

	// �X�e�[�W�S����
	void clear();

	// ���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

	bool PillarVS(const DirectX::XMFLOAT3& pos, const float radius, const float height, DirectX::XMFLOAT3& outPos);

private:
	static const int MAX_BONES{ 256 };
	std::unique_ptr<Model> obj = nullptr;
	std::vector<Stage*> stages;
	InstancingConstants constants;
	int instangNo = 0;
};