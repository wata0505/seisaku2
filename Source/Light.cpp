#include "Light.h"
#include "LightManager.h"

// �R���X�g���N�^
Light::Light(DirectX::XMFLOAT3 position, float distance, float r, float g, float b)
{
	lightParam.DirLightDirection = { position.x, position.y, position.z, distance };	// �ʒu�E���������ݒ�
	lightParam.DirLightColor = { r, g, b, 1.0f };										// �F�ݒ�
}

// �j��
void Light::Destroy()
{
	LightManager::Instance().Remove(this);
}

// ���C�g���ݒ�
void Light::SetParameter(DirectX::XMFLOAT3 position, float distance, float r, float g, float b)
{
	lightParam.DirLightDirection = { position.x, position.y, position.z, distance };	// �ʒu�E���������ݒ�
	lightParam.DirLightColor = { r, g, b, 1.0f };										// �F�ݒ�
}