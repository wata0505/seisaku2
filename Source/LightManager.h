#pragma once

#include "Light.h"

class LightManager
{
private:
	LightManager();
	~LightManager() {}

public:
	// �C���X�^���X�擾
	static LightManager& Instance()
	{
		static LightManager instance;
		return instance;
	}
	
	// �X�V����
	void Update(float elapsedTime);

	// ���C�g�o�^
	void Register(Light* Light);
	// ���C�g�폜
	void Remove(Light* Light);
	// ���C�g�S�폜
	void Clear();

	// ���C�g���o�b�t�@�ɓ���
	bool ActivateDirLight(ID3D11DeviceContext* deviceContext, int index);
	// ���C�g���V�F�[�_�[�ɐݒ�
	void Activate(ID3D11DeviceContext* deviceContext);
	// ���C�g����
	void CompositActivate(ID3D11DeviceContext* deviceContext);

public:
	// ���C�g���擾
	const int GetDirLightCount() { return static_cast<int>(lightParam.size()); };

private:
	// ���C�g���
	struct DirLightParam
	{
		DirectX::XMFLOAT4 DirLightDirection;	// ����
		DirectX::XMFLOAT4 DirLightColor;		// �F
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