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
	//ライティング関係
	struct DirLightParam {
		DirectX::XMFLOAT4 DirLightDirection;	//方向
		DirectX::XMFLOAT4 DirLightColor;		//色
	};
public:
	
	Light(DirectX::XMFLOAT3 pos, float distance, float r, float g, float b);
	~Light();
	//ライト情報更新
	void SetPos(DirectX::XMFLOAT3 pos, float distance, float r, float g, float b);
	void Destroy();
	DirLightParam GetDirLightParam() { return lightParam; }
private:
	//ライティング関係
	DirLightParam lightParam;
};
