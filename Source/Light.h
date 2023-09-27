#pragma once
#pragma once
#include<d3d11.h>
#include<DirectXMath.h>
#include <memory>
#include <wrl.h>
#include<vector>

class Light
{
private:
	//���C�e�B���O�֌W
	struct DirLightParam {
		DirectX::XMFLOAT4 DirLightDirection;	//����
		DirectX::XMFLOAT4 DirLightColor;		//�F
	};
public:
	
	Light(DirectX::XMFLOAT3 pos, float distance, float r, float g, float b);
	~Light();
	//���C�g���X�V
	void SetPos(DirectX::XMFLOAT3 pos, float distance, float r, float g, float b);
	void Destroy();
	DirLightParam GetDirLightParam() { return lightParam; }
private:
	//���C�e�B���O�֌W
	DirLightParam lightParam;
};
