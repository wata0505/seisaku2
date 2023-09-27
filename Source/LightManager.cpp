#include "LightManager.h"
#include"Shader.h"
#include"Graphics.h"

LightManager::LightManager() {
	Graphics& graphics = Graphics::Instance();
	create_vs_from_cso(graphics.GetDevice(), "Shader\\LightVS.cso", VertexShader.ReleaseAndGetAddressOf(),
		nullptr, nullptr, 0);
	create_ps_from_cso(graphics.GetDevice(), "Shader\\LightPS.cso", PixelShader.GetAddressOf());

	create_ps_from_cso(graphics.GetDevice(), "Shader\\CompositPS.cso", CompositPixelShader.GetAddressOf());

	// コンスタントバッファ生成
	D3D11_BUFFER_DESC bdLight;
	ZeroMemory(&bdLight, sizeof(D3D11_BUFFER_DESC));
	bdLight.ByteWidth = sizeof(DirLightParam);
	bdLight.Usage = D3D11_USAGE_DEFAULT;
	bdLight.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bdLight.CPUAccessFlags = 0;
	graphics.GetDevice()->CreateBuffer(&bdLight, NULL, DirLight.GetAddressOf());

}
LightManager::~LightManager() {};

//void LightManager::AddDirLight(DirectX::XMFLOAT3 dir, float r, float g, float b) {
//	//DirLightParam* LightParam;
//	//LightParam->DirLightDirection = { dir.x, dir.y, dir.z, 0 };
//	//LightParam->DirLightColor = { r,g,b,1 };
//	//lightParam.push_back(LightParam);
//	//NumDirLight++;
//}
//void LightManager::AddPointLight(DirectX::XMFLOAT3 pos, float distance, float r, float g, float b) {
//	//DirLightParam* LightParam;
//	//LightParam->DirLightDirection = { pos.x, pos.y, pos.z, distance };
//	//LightParam->DirLightColor = { r,g,b,1 };
//	//lightParam.push_back(LightParam);
//	//NumDirLight++;
//}
//ライト更新
void LightManager::Update(float elapsedTime){
	//removes.insert = lightParam.at(index);

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
	//弾丸の廃棄処理
	//delete lightParam;
}
//ライト登録
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
void LightManager::Clear()
{
	for (Light* Light : lightParam)
	{
		delete Light;
	}
	lightParam.clear();
}
//ライト情報バッファに入力
bool LightManager::ActivateDirLight(ID3D11DeviceContext* immediate_context,int index) {
	DirectX::XMFLOAT4 color = lightParam.at(index)->GetDirLightParam().DirLightColor;
	for (int i = 0; i < colors.size(); i++) {
		if (color.x == colors.at(i).x && color.y == colors.at(i).y && color.z == colors.at(i).z) {
			return false;
		}
	}
	immediate_context->UpdateSubresource(DirLight.Get(), 0, NULL, &lightParam.at(index)->GetDirLightParam(), 0, 0);
	immediate_context->PSSetConstantBuffers(1, 1, DirLight.GetAddressOf());
	//ポイントライトの色を登録
	colors.push_back(lightParam.at(index)->GetDirLightParam().DirLightColor);
	return true;
}
//ライトをシェーダーに設定
void LightManager::Activate(ID3D11DeviceContext* immediate_context) {
	immediate_context->VSSetShader(VertexShader.Get(), 0, 0);
	immediate_context->PSSetShader(PixelShader.Get(), 0, 0);
	colors.clear();
}
//ライト合成
void LightManager::CompositActivate(ID3D11DeviceContext* immediate_context) {
	immediate_context->VSSetShader(VertexShader.Get(), 0, 0);
	immediate_context->PSSetShader(CompositPixelShader.Get(), 0, 0);
}