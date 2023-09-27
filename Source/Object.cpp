#include "Object.h"
#include"ObjectManager.h"
#include"Graphics.h"

// �R���X�g���N�^
Object::Object(ObjectManager* manager)
	:manager(manager)
{
	// �������Ƀ}�l�[�W���[�ɓo�^����
	manager->Register(this);
}

// �f�o�b�O�v���~�e�B�u�`��
void Object::DrawDebugPrimitive()
{
	//DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	//
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

// �j��
void Object::Destroy()
{
	//if (type == 5)boon->Play(false);
	// �}�l�[�W���[���玩������������
	manager->Remove(this);

}

// �s��X�V����
void Object::UpdateTransform()
{
	DirectX::XMVECTOR Front, Up, Right;

	//�O�x�N�g�����Z�o
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);

	//���̏�x�N�g�����Z�o
	Up = DirectX::XMLoadFloat3(&directionUp);
	Up = DirectX::XMVector3Normalize(Up);

	//�E�x�N�g�����Z�o
	Right = DirectX::XMVector3Cross(Up, Front);

	//��x�N�g�����Z�o
	Up = DirectX::XMVector3Cross(Front, Right);

	//�v�Z���ʂ����o��
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	//�Z�o�������x�N�g������s����쐬
	transform._11 = right.x * 0.01*scale.x;
	transform._12 = right.y * 0.01*scale.x;
	transform._13 = right.z * 0.01*scale.x;
	transform._14 = 0.0f;
	transform._21 = up.x * 0.01 * scale.y;
	transform._22 = up.y * 0.01 * scale.y;
	transform._23 = up.z * 0.01 * scale.y;
	transform._24 = 0.0f;
	transform._31 = front.x * 0.01 * scale.z;
	transform._32 = front.y * 0.01 * scale.z;
	transform._33 = front.z * 0.01 * scale.z;
	transform._34 = 0.0f;
	transform._41 = position.x;
	transform._42 = position.y;
	transform._43 = position.z;
	transform._44 = 1.0f;

	//���˕���
	this->direction = front;
}
