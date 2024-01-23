#pragma once

#include "Model.h"
#include "Stage.h"

class StageMain : public Stage
{
public:
	StageMain();
	~StageMain() override {};

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) override;

	// ���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;

	// �s��X�V����
	void UpdateTransform();

private:
	std::unique_ptr<Model>	model = nullptr;		// ���f��
	float					progressTimer = 0.0f;	// �o�ߎ���
};