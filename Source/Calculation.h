#pragma once
#include<DirectXMath.h>
class Vector3
{
public:
	//³K»
	static DirectX::XMFLOAT3 Normalize(DirectX::XMFLOAT3 v1);
	//àÏ
	static float Dot(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	//å«³
	static float Lenght(DirectX::XMFLOAT3 v1);
	//ø«Z
	static DirectX::XMFLOAT3 Subset(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	//OÏ
	static DirectX::XMFLOAT3 Cross(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	//|¯Z
	static DirectX::XMFLOAT3 Multip(DirectX::XMFLOAT3 v1, float v2);
	//«µZ
	static DirectX::XMFLOAT3 Add(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	//ÊuÉûüð«·
	static DirectX::XMFLOAT3 PosDir(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2,float Lengle);
	//RDðQDÀWÉ
	static DirectX::XMFLOAT2 C3Dor2D(DirectX::XMFLOAT3 pos);
	//m¦ÅtrueðÔ·
	static bool Probability(int num);
	//t[²ÆÉtrueðÔ·
	static bool FrameInput(int timer,int framIndex = 60);
	
};
class Vector2
{
public:
	// ü`â®
	static DirectX::XMFLOAT2 Normalize(DirectX::XMFLOAT2 v1);
	//àÏ
	static float Dot(DirectX::XMFLOAT2 v1, DirectX::XMFLOAT2 v2);
	//ø«Z
	static DirectX::XMFLOAT2 Subset(DirectX::XMFLOAT2 v1, DirectX::XMFLOAT2 v2);
						   
	//|¯Z				   
	static DirectX::XMFLOAT2 Multip(DirectX::XMFLOAT2 v1, float v2);
	//«µZ					   
	static DirectX::XMFLOAT2 Add(DirectX::XMFLOAT2 v1, DirectX::XMFLOAT2 v2);


};