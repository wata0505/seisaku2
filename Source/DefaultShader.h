#pragma once

#include <memory>
#include <wrl.h>
#include "Shader.h"

class DefaultShader : public ModelShader
{
public:
	DefaultShader(ID3D11Device* device);
	~DefaultShader() override {}

	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc)override;
	void Draw(ID3D11DeviceContext* dc, Model* model, const DirectX::XMFLOAT4 uvstatus = { 0,0,0,0 }, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f })override;
	void Draw(ID3D11DeviceContext* dc, const Model* model, InstancingConstants buffer, int max, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
	void End(ID3D11DeviceContext* dc) override;	

private:
	static const int MaxBones = 128;

	static const int MAX_BONES{ 256 };
	// �萔�o�b�t�@�t�H�[�}�b�g
	struct constants
	{
		DirectX::XMFLOAT4X4 world;			// ���[���h�s��
		DirectX::XMFLOAT4 materialColor;	// �}�e���A���J���[
		DirectX::XMFLOAT4X4 boneTransforms[MAX_BONES]{ {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1} };
	};
	struct scene_constants
	{
		DirectX::XMFLOAT4X4 viewProjection;	// �r���[�E�v���W�F�N�V�����ϊ��s��
		DirectX::XMFLOAT4 lightDirection;		// ���C�g�̌���
		DirectX::XMFLOAT4 cameraPosition;  //�J�����ʒu
	};
	struct ShadowConstants
	{
		DirectX::XMFLOAT4X4 shadowProjection;
	};
	// �T�u�Z�b�g�p�萔�o�b�t�@
	struct SubsetConstantBuffer
	{
		float adjustMetalness = 0.0f;	// �����x
		float adjustSmoothness = 0.0f;	// �e��
		float glitchIntensity = 1.0f;	// ���x
		float glitchScale = 50.0f;		// �U�ꕝ
		float timer = 0.0f;				// �X�V����
		float scanBorder = 10.0f;		// �`��͈�
		float glowBorder = 10.0f;		// �`��͈�
		float hologramBorder = 10.0f;	// �`��͈�
		DirectX::XMFLOAT3 hologramColor = { 1.0f, 0.0f, 0.0f }; // �z���O�����F
		float maxHeight;				// �ō��_
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> shadowBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> sceneBuffers;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subsetConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> uvBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> instanBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader[Max];
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader[Max];
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	
	Microsoft::WRL::ComPtr<ID3D11SamplerState>      shadowMapSamplerState;

	DirectX::XMFLOAT4	materialColor = {1,1,1,1};
};