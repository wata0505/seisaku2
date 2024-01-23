#pragma once

#include "Shader.h"
#include "Collision.h"

class Stage
{
public:
	Stage() {}
	virtual ~Stage() {}

	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	// �`�揈��
	virtual void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) = 0;

	// ���C�L���X�g
	virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) = 0;

public:
	// �s��擾
	const DirectX::XMFLOAT4X4& GetTransform() const { return transform; }

	// �ʒu�ݒ�Ǝ擾
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// �X�P�[���ݒ�Ǝ擾
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// �C���X�^���V���O�擾
	bool GetInstancing() { return instancing; }

protected:
	DirectX::XMFLOAT4X4	transform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };	// ���f���s��
	DirectX::XMFLOAT3	position = {};										// �ʒu
	DirectX::XMFLOAT3	angle = {};											// �p�x
	DirectX::XMFLOAT3	scale = { 1.0f, 1.0f, 1.0f };						// �T�C�Y
	bool				instancing = false;									// �C���X�^���V���O
};