#include "Light.h"
#include "LightManager.h"

// コンストラクタ
Light::Light(DirectX::XMFLOAT3 position, float distance, float r, float g, float b)
{
	lightParam.DirLightDirection = { position.x, position.y, position.z, distance };	// 位置・減衰距離設定
	lightParam.DirLightColor = { r, g, b, 1.0f };										// 色設定
}

// 破棄
void Light::Destroy()
{
	LightManager::Instance().Remove(this);
}

// ライト情報設定
void Light::SetParameter(DirectX::XMFLOAT3 position, float distance, float r, float g, float b)
{
	lightParam.DirLightDirection = { position.x, position.y, position.z, distance };	// 位置・減衰距離設定
	lightParam.DirLightColor = { r, g, b, 1.0f };										// 色設定
}