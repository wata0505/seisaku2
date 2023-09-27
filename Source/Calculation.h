#pragma once
#include<DirectXMath.h>
class Vector3
{
public:
	//³‹K‰»
	static DirectX::XMFLOAT3 Normalize(DirectX::XMFLOAT3 v1);
	//“àÏ
	static float Dot(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	//‘å‚«‚³
	static float Lenght(DirectX::XMFLOAT3 v1);
	//ˆø‚«Z
	static DirectX::XMFLOAT3 Subset(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	//ŠOÏ
	static DirectX::XMFLOAT3 Cross(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	//Š|‚¯Z
	static DirectX::XMFLOAT3 Multip(DirectX::XMFLOAT3 v1, float v2);
	//‘«‚µZ
	static DirectX::XMFLOAT3 Add(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
	//ˆÊ’u‚É•ûŒü‚ğ‘«‚·
	static DirectX::XMFLOAT3 PosDir(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2,float Lengle);
	//‚RD‚ğ‚QDÀ•W‚É
	static DirectX::XMFLOAT2 C3Dor2D(DirectX::XMFLOAT3 pos);
	//Šm—¦‚Åtrue‚ğ•Ô‚·
	static bool Probability(int num);
	//ƒtƒŒ[ƒ€‚²‚Æ‚Étrue‚ğ•Ô‚·
	static bool FrameInput(int timer,int framIndex = 60);
	
};
class Vector2
{
public:
	// üŒ`•âŠ®
	static DirectX::XMFLOAT2 Normalize(DirectX::XMFLOAT2 v1);
	//“àÏ
	static float Dot(DirectX::XMFLOAT2 v1, DirectX::XMFLOAT2 v2);
	//ˆø‚«Z
	static DirectX::XMFLOAT2 Subset(DirectX::XMFLOAT2 v1, DirectX::XMFLOAT2 v2);
						   
	//Š|‚¯Z				   
	static DirectX::XMFLOAT2 Multip(DirectX::XMFLOAT2 v1, float v2);
	//‘«‚µZ					   
	static DirectX::XMFLOAT2 Add(DirectX::XMFLOAT2 v1, DirectX::XMFLOAT2 v2);


};