#pragma once

#include "Model.h"
#include "Stage.h"

class StageObj : public Stage
{
public:
	StageObj(DirectX::XMFLOAT3 position, float angleY = 0.0f, float angleZ = 0.0f);
	~StageObj() override {};

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) override;

	// ���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;
	
	// �s��X�V����
	void UpdateTransform();

private:
	std::unique_ptr<Model>	obj = nullptr;												// ���f��
	DirectX::XMFLOAT4X4		raycastTransform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };	// ���C�L���X�g�p�s��
	//float					radius = 5.0f;												// ���a
	//bool					isRender = true;											// �`��t���O
};