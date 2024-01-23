#include "LightManager.h"
#include "Shader.h"
#include "Graphics.h"

// コンストラクタ
LightManager::LightManager() 
{
	Graphics& graphics = Graphics::Instance();
	create_vs_from_cso(graphics.GetDevice(), "Shader\\LightVS.cso", vertexShader.ReleaseAndGetAddressOf(), nullptr, nullptr, 0);
	create_ps_from_cso(graphics.GetDevice(), "Shader\\LightPS.cso", pixelShader.GetAddressOf());
	create_ps_from_cso(graphics.GetDevice(), "Shader\\CompositPS.cso", compositPixelShader.GetAddressOf());

	// コンスタントバッファ生成
	D3D11_BUFFER_DESC bdLight;
	ZeroMemory(&bdLight, sizeof(D3D11_BUFFER_DESC));
	bdLight.ByteWidth = sizeof(DirLightParam);
	bdLight.Usage = D3D11_USAGE_DEFAULT;
	bdLight.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bdLight.CPUAccessFlags = 0;
	graphics.GetDevice()->CreateBuffer(&bdLight, NULL, lightConstantBuffer.GetAddressOf());
}

// 更新処理
void LightManager::Update(float elapsedTime)
{	
	for (Light* light : removes)
	{
		// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Light*>::iterator it = std::find(lightParam.begin(), lightParam.end(), light);
		if (it != lightParam.end())
		{
			lightParam.erase(it);
		}

		// 削除
		delete light;
	}
	// 破棄リストをクリア
	removes.clear();
}

// ライト登録
void LightManager::Register(Light* Light)
{
	lightParam.emplace_back(Light);
}
// ライト削除
void LightManager::Remove(Light* Light)
{
	// 破棄リストに追加
	removes.emplace_back(Light);
}
// ライト全削除
void LightManager::Clear()
{
	for (Light* Light : lightParam)
	{
		delete Light;
	}
	lightParam.clear();
}

// ライト情報バッファに入力
bool LightManager::ActivateDirLight(ID3D11DeviceContext* deviceContext, int index) 
{
	DirectX::XMFLOAT4 color = lightParam.at(index)->GetDirLightParam().DirLightColor;
	for (int i = 0; i < colors.size(); i++) 
	{
		if (color.x == colors.at(i).x && color.y == colors.at(i).y && color.z == colors.at(i).z) 
		{
			return false;
		}
	}
	deviceContext->UpdateSubresource(lightConstantBuffer.Get(), 0, NULL, &lightParam.at(index)->GetDirLightParam(), 0, 0);
	deviceContext->PSSetConstantBuffers(1, 1, lightConstantBuffer.GetAddressOf());
	// ポイントライトの色を登録
	colors.push_back(lightParam.at(index)->GetDirLightParam().DirLightColor);
	return true;
}
// ライトをシェーダーに設定
void LightManager::Activate(ID3D11DeviceContext* deviceContext) 
{
	deviceContext->VSSetShader(vertexShader.Get(), 0, 0);
	deviceContext->PSSetShader(pixelShader.Get(), 0, 0);
	colors.clear();
}
// ライト合成
void LightManager::CompositActivate(ID3D11DeviceContext* deviceContext) 
{
	deviceContext->VSSetShader(vertexShader.Get(), 0, 0);
	deviceContext->PSSetShader(compositPixelShader.Get(), 0, 0);
}