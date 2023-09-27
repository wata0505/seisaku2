#include "Light.h"
#include"Shader.h"
#include"Graphics.h"
#include"LightManager.h"
Light::Light(DirectX::XMFLOAT3 pos, float distance, float r, float g, float b) {
	lightParam.DirLightDirection = { pos.x, pos.y, pos.z, distance };
	lightParam.DirLightColor = { r,g,b,1 };
}
Light::~Light() {};


void Light::SetPos(DirectX::XMFLOAT3 pos, float distance, float r, float g, float b) {
	lightParam.DirLightDirection = { pos.x, pos.y, pos.z, distance };
	lightParam.DirLightColor = { r,g,b,1 };
}
void Light::Destroy() {
	LightManager::Instance().Remove(this);
}

