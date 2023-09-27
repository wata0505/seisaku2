#pragma once

#include "Shader.h"
#include"Collision.h"

//�X�e�[�W
class Stage
{
public:
	Stage() {}
	virtual ~Stage() {}

	//�X�V����
	virtual void Update(float elapsedTime) = 0;

	//�`�揈��
	virtual void Render(ID3D11DeviceContext* dc, ModelShader* shader) = 0;

	// ���C�L���X�g
	virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) = 0;

	// ���C�L���X�g
	virtual bool PillarVS(const DirectX::XMFLOAT3& pos, const float radius, const float height, DirectX::XMFLOAT3& outPos) = 0;

	bool GetInstsncing() { return instsncing; }

	DirectX::XMFLOAT4X4 GetTransform() { return transform; }

protected:
	bool instsncing = false;
	DirectX::XMFLOAT4X4   transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
};