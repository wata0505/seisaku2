#pragma once

#include "Light.h"

class LightManager
{
private:
	LightManager();
	~LightManager() {}

public:
	// インスタンス取得
	static LightManager& Instance()
	{
		static LightManager instance;
		return instance;
	}
	
	// 更新処理
	void Update(float elapsedTime);

	// ライト登録
	void Register(Light* Light);
	// ライト削除
	void Remove(Light* Light);
	// ライト全削除
	void Clear();

	// ライト情報バッファに入力
	bool ActivateDirLight(ID3D11DeviceContext* deviceContext, int index);
	// ライトをシェーダーに設定
	void Activate(ID3D11DeviceContext* deviceContext);
	// ライト合成
	void CompositActivate(ID3D11DeviceContext* deviceContext);

public:
	// ライト数取得
	const int GetDirLightCount() { return static_cast<int>(lightParam.size()); };

private:
	// ライト情報
	struct DirLightParam
	{
		DirectX::XMFLOAT4 DirLightDirection;	// 方向
		DirectX::XMFLOAT4 DirLightColor;		// 色
	};

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	compositPixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		lightConstantBuffer;
	std::vector<Light*>							lightParam;
	std::vector<Light*>							removes;
	std::vector<DirectX::XMFLOAT4>				colors;
};