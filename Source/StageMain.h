#pragma once

#include "Model.h"
#include "Stage.h"

//���C���X�e�[�W
class StageMain :public Stage
{
public:
	StageMain();
	~StageMain()override;

	//�X�V����
	void Update(float elapsedTime)override;

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;

	// ���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;
	bool PillarVS(const DirectX::XMFLOAT3& pos, const float radius, const float height, DirectX::XMFLOAT3& outPos)override;
	void UpdateTransform();

	//�ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }
	
	//�ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	
	//�X�P�[���擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }
	
	//�X�P�[���擾
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

private:
	 std::unique_ptr<Model> model = nullptr;
	 std::unique_ptr<Model> obj = nullptr;
	DirectX::XMFLOAT3	  position = { 0,0,0 };
	DirectX::XMFLOAT3	  angle{ 0,0,0 };
	DirectX::XMFLOAT3	  scale = { 1,1,1 };
	float timer = 0.0f;

	
};