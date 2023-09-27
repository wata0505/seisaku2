#include "LightManager.h"
#include"Shader.h"
#include"Graphics.h"

LightManager::LightManager() {
	Graphics& graphics = Graphics::Instance();
	create_vs_from_cso(graphics.GetDevice(), "Shader\\LightVS.cso", VertexShader.ReleaseAndGetAddressOf(),
		nullptr, nullptr, 0);
	create_ps_from_cso(graphics.GetDevice(), "Shader\\LightPS.cso", PixelShader.GetAddressOf());

	create_ps_from_cso(graphics.GetDevice(), "Shader\\CompositPS.cso", CompositPixelShader.GetAddressOf());

	// �R���X�^���g�o�b�t�@����
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
//���C�g�X�V
void LightManager::Update(float elapsedTime){
	//removes.insert = lightParam.at(index);

	for (Light* light : removes)
	{
		// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Light*>::iterator it = std::find(lightParam.begin(), lightParam.end(), light);
		if (it != lightParam.end())
		{
			lightParam.erase(it);
		}

		// �폜
		delete light;
	}
	// �j�����X�g���N���A
	removes.clear();
	//�e�ۂ̔p������
	//delete lightParam;
}
//���C�g�o�^
void LightManager::Register(Light* Light)
{
	lightParam.emplace_back(Light);
}
// ���C�g�폜
void LightManager::Remove(Light* Light)
{
	// �j�����X�g�ɒǉ�
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
//���C�g���o�b�t�@�ɓ���
bool LightManager::ActivateDirLight(ID3D11DeviceContext* immediate_context,int index) {
	DirectX::XMFLOAT4 color = lightParam.at(index)->GetDirLightParam().DirLightColor;
	for (int i = 0; i < colors.size(); i++) {
		if (color.x == colors.at(i).x && color.y == colors.at(i).y && color.z == colors.at(i).z) {
			return false;
		}
	}
	immediate_context->UpdateSubresource(DirLight.Get(), 0, NULL, &lightParam.at(index)->GetDirLightParam(), 0, 0);
	immediate_context->PSSetConstantBuffers(1, 1, DirLight.GetAddressOf());
	//�|�C���g���C�g�̐F��o�^
	colors.push_back(lightParam.at(index)->GetDirLightParam().DirLightColor);
	return true;
}
//���C�g���V�F�[�_�[�ɐݒ�
void LightManager::Activate(ID3D11DeviceContext* immediate_context) {
	immediate_context->VSSetShader(VertexShader.Get(), 0, 0);
	immediate_context->PSSetShader(PixelShader.Get(), 0, 0);
	colors.clear();
}
//���C�g����
void LightManager::CompositActivate(ID3D11DeviceContext* immediate_context) {
	immediate_context->VSSetShader(VertexShader.Get(), 0, 0);
	immediate_context->PSSetShader(CompositPixelShader.Get(), 0, 0);
}