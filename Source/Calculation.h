#pragma once
#include<DirectXMath.h>
class Vector3
{
public:
	//���K��
	static DirectX::XMFLOAT3 Normalize(DirectX::XMFLOAT3 v1);
	//����
	static float Dot(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	//�傫��
	static float Lenght(DirectX::XMFLOAT3 v1);
	//�����Z
	static DirectX::XMFLOAT3 Subset(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	//�O��
	static DirectX::XMFLOAT3 Cross(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	//�|���Z
	static DirectX::XMFLOAT3 Multip(DirectX::XMFLOAT3 v1, float v2);
	//�����Z
	static DirectX::XMFLOAT3 Add(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	//�ʒu�ɕ����𑫂�
	static DirectX::XMFLOAT3 PosDir(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2,float Lengle);
	//�RD���QD���W��
	static DirectX::XMFLOAT2 C3Dor2D(DirectX::XMFLOAT3 pos);
	//�m����true��Ԃ�
	static bool Probability(int num);
	//�t���[�����Ƃ�true��Ԃ�
	static bool FrameInput(int timer,int framIndex = 60);
	
};
class Vector2
{
public:
	// ���`�⊮
	static DirectX::XMFLOAT2 Normalize(DirectX::XMFLOAT2 v1);
	//����
	static float Dot(DirectX::XMFLOAT2 v1, DirectX::XMFLOAT2 v2);
	//�����Z
	static DirectX::XMFLOAT2 Subset(DirectX::XMFLOAT2 v1, DirectX::XMFLOAT2 v2);
						   
	//�|���Z				   
	static DirectX::XMFLOAT2 Multip(DirectX::XMFLOAT2 v1, float v2);
	//�����Z					   
	static DirectX::XMFLOAT2 Add(DirectX::XMFLOAT2 v1, DirectX::XMFLOAT2 v2);


};