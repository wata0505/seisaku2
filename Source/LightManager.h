#pragma once
#include<d3d11.h>
#include<DirectXMath.h>
#include <memory>
#include <wrl.h>
#include<vector>
#include<set>
#include"Light.h"
class LightManager
{
private:
	LightManager();
	~LightManager();
public:
	static LightManager& Instance()
	{
		static LightManager instance;
		return instance;
	}

	bool ActivateDirLight(ID3D11DeviceContext* immediate_context,int index);
	void Activate(ID3D11DeviceContext* immediate_context);
	void CompositActivate(ID3D11DeviceContext* immediate_context);
	
	const int GetNumDirLight() { return static_cast<int>(lightParam.size()); };

	void Update(float elapsedTime);

	void Register(Light* Light);
	
	// ライト削除
	void Remove(Light* Light);
	
	void Clear();
private:
	//ライティング関係
	struct DirLightParam {
		DirectX::XMFLOAT4 DirLightDirection;	//方向
		DirectX::XMFLOAT4 DirLightColor;		//色
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> DirLight;

	int NumDirLight = 0;
	std::vector <Light*> lightParam;
	std::vector<Light*> removes;
	std::vector<DirectX::XMFLOAT4> colors;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> CompositPixelShader;
};

