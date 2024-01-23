#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <wrl.h>
#include <vector>

class Light
{
private:
	// ���C�g���
	struct DirLightParam 
	{
		DirectX::XMFLOAT4 DirLightDirection;	// ����
		DirectX::XMFLOAT4 DirLightColor;		// �F
	};

public:
	Light(DirectX::XMFLOAT3 position, float distance, float r, float g, float b);
	~Light() {}
	
	// �j��
	void Destroy();

public:
	// ���C�g���擾
	DirLightParam GetDirLightParam() { return lightParam; }

	// ���C�g���ݒ�
	void SetParameter(DirectX::XMFLOAT3 position, float distance, float r, float g, float b);
	
private:
	DirLightParam lightParam;	// ���C�g���
};
