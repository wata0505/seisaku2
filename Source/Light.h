#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <wrl.h>
#include <vector>

class Light
{
private:
	// ライト情報
	struct DirLightParam 
	{
		DirectX::XMFLOAT4 DirLightDirection;	// 方向
		DirectX::XMFLOAT4 DirLightColor;		// 色
	};

public:
	Light(DirectX::XMFLOAT3 position, float distance, float r, float g, float b);
	~Light() {}
	
	// 破棄
	void Destroy();

public:
	// ライト情報取得
	DirLightParam GetDirLightParam() { return lightParam; }

	// ライト情報設定
	void SetParameter(DirectX::XMFLOAT3 position, float distance, float r, float g, float b);
	
private:
	DirLightParam lightParam;	// ライト情報
};
