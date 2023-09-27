#pragma once


#include <memory>
#include <wrl.h>
#include "Shader.h"

class ToonShader : public ModelShader
{
public:
	ToonShader(ID3D11Device* device);
	~ToonShader() override {}

	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc)override;
	void Draw(ID3D11DeviceContext* dc, const Model* model, std::vector<SkinnedMeshResouurce::constants> buffer, const DirectX::XMFLOAT4 uvstatus = { 0,0,0,0 }, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f })override;
	void Draw(ID3D11DeviceContext* dc, const Model* model, InstancingConstants buffer, int max, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }) {};
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
		DirectX::XMFLOAT4X4 shadowProjection; //�e�̃v���W�F�N�V�����s��
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> shadowBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> sceneBuffers;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> uvBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;


	Microsoft::WRL::ComPtr<ID3D11SamplerState>      shadowMapSamplerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;

	DirectX::XMFLOAT4	materialColor = { 0.5,0.5,0.5,1 };
	std::vector<SkinnedMeshResouurce::constants> buffers;

	float uvTimer = 0;
};