#pragma once
#include "Shader.h"
#include "SwordTrailShader.h"

// �O���錾
class ObjectManager;

// �e��
class Object
{
public:
	Object(ObjectManager* manager);
	virtual ~Object() {}

	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	// �`�揈��
	virtual void Render(ID3D11DeviceContext* dc, ModelShader* shader) = 0;

	// �G�t�F�N�g�`�揈��
	virtual void TrailRender(RenderContext& rc, SwordTrailShader* shader)= 0;

	// �f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

	// �ʒu�擾
	const DirectX::XMFLOAT3& GetPosition()const { return position; }

	// �����擾
	const DirectX::XMFLOAT3& GetDirection()const { return direction; }

	// �X�P�[���擾
	const DirectX::XMFLOAT3& GetScale()const { return scale; }

	// ���a����
	const float GetRadius()const { return radius; }

	//�^�C�v�擾
	const int GetType()const { return type; }

	// ���G����
	const float GetInvincibleTime()const { return invincibleTime; }

	//�_���[�W
	const int GetDamage()const { return damage; }

	void SetDirectionUp(DirectX::XMFLOAT3 up) { directionUp = up; }

	DirectX::XMFLOAT3 GetDirectionUp() { return directionUp; }

	// �j��
	void Destroy();

protected:
	// �s��X�V����
	void UpdateTransform();
protected:
	DirectX::XMFLOAT3  position = { 0,0,0 };
	DirectX::XMFLOAT3  direction = { 0,0,1 };
	DirectX::XMFLOAT3  scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	DirectX::XMFLOAT3 directionUp = { 0,1,0};
	int type = -1;
	float damage = 1;
	float invincibleTime = 1.0;
	float lifeTimer = 3.0f;
	ObjectManager* manager = nullptr;
	float radius = 0.5f;
};