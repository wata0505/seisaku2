#pragma once
#include <memory>
#include <wrl.h>
#include "Shader.h"
#define MAX_INSTANCES 512




struct ParticleConstants
{
	DirectX::XMFLOAT4X4 world[MAX_INSTANCES]{ {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1} };			// ワールド行列
	DirectX::XMFLOAT4 materialColor[MAX_INSTANCES];	// マテリアルカラ
	DirectX::XMFLOAT4 dissolve[MAX_INSTANCES];
};
class ParticleShader
{
public:
	enum ModelPSType {
		Default,
		Toon,

		MAX
	};
	
public:
	ParticleShader(ID3D11Device* device);
	~ParticleShader(){}

	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc);
	void Draw(ID3D11DeviceContext* dc, const Model* model,ParticleConstants& buffer,int partialCount,int psType,int texType,const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
	void End(ID3D11DeviceContext* dc);

private:
	//static const int MaxBones = 128;

	//static const int MAX_BONES{ 256 };
	// 定数バッファフォーマット
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader[static_cast<int>(ModelPSType::MAX)];
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;


	Microsoft::WRL::ComPtr<ID3D11SamplerState>      shadowMapSamplerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;

	DirectX::XMFLOAT4	materialColor = { 0.5,0.5,0.5,1 };
	std::vector<SkinnedMeshResouurce::constants> buffers;
};