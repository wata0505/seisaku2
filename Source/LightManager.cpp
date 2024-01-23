#include "LightManager.h"
#include "Shader.h"
#include "Graphics.h"

// �R���X�g���N�^
LightManager::LightManager() 
{
	Graphics& graphics = Graphics::Instance();
	create_vs_from_cso(graphics.GetDevice(), "Shader\\LightVS.cso", vertexShader.ReleaseAndGetAddressOf(), nullptr, nullptr, 0);
	create_ps_from_cso(graphics.GetDevice(), "Shader\\LightPS.cso", pixelShader.GetAddressOf());
	create_ps_from_cso(graphics.GetDevice(), "Shader\\CompositPS.cso", compositPixelShader.GetAddressOf());

	// �R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bdLight;
	ZeroMemory(&bdLight, sizeof(D3D11_BUFFER_DESC));
	bdLight.ByteWidth = sizeof(DirLightParam);
	bdLight.Usage = D3D11_USAGE_DEFAULT;
	bdLight.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bdLight.CPUAccessFlags = 0;
	graphics.GetDevice()->CreateBuffer(&bdLight, NULL, lightConstantBuffer.GetAddressOf());
}

// �X�V����
void LightManager::Update(float elapsedTime)
{	
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
}

// ���C�g�o�^
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
// ���C�g�S�폜
void LightManager::Clear()
{
	for (Light* Light : lightParam)
	{
		delete Light;
	}
	lightParam.clear();
}

// ���C�g���o�b�t�@�ɓ���
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
	// �|�C���g���C�g�̐F��o�^
	colors.push_back(lightParam.at(index)->GetDirLightParam().DirLightColor);
	return true;
}
// ���C�g���V�F�[�_�[�ɐݒ�
void LightManager::Activate(ID3D11DeviceContext* deviceContext) 
{
	deviceContext->VSSetShader(vertexShader.Get(), 0, 0);
	deviceContext->PSSetShader(pixelShader.Get(), 0, 0);
	colors.clear();
}
// ���C�g����
void LightManager::CompositActivate(ID3D11DeviceContext* deviceContext) 
{
	deviceContext->VSSetShader(vertexShader.Get(), 0, 0);
	deviceContext->PSSetShader(compositPixelShader.Get(), 0, 0);
}