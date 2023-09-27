#pragma once
#include <memory>
#include <wrl.h>
#include "Shader.h"
#include "Sprite.h"
#define MAX_INSTANCES 512
struct InstancingSpriteConstants {
	DirectX::XMFLOAT4 world[MAX_INSTANCES]{ {0,0,0,1} };			// ���[���h�s��
	DirectX::XMFLOAT4 materialColor[MAX_INSTANCES];	// �}�e���A���J
	DirectX::XMFLOAT4 spriteSize[MAX_INSTANCES];
	DirectX::XMFLOAT4 dissolve[MAX_INSTANCES];
	DirectX::XMFLOAT4 timer[MAX_INSTANCES];
	
	
};
class InstancingSpriteShader : public SpriteShader
{
public:
	InstancingSpriteShader(ID3D11Device* device);
	~InstancingSpriteShader() override {}

	void Begin(const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Sprite* sprite) override;
	void End(const RenderContext& rc) override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>        geometryShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;


	Microsoft::WRL::ComPtr<ID3D11Buffer> sceneBuffers;

	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffers;


	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;

	struct SceneConstants
	{
		DirectX::XMFLOAT4X4 View;       // �r���[�ϊ��s��
		DirectX::XMFLOAT4X4 Projection; // �����ϊ��s��
		//DirectX::XMFLOAT2   ParticleSize;  // �p�[�e�B�N���̑傫��

	};
};
