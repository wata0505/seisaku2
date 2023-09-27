#pragma once
#include <memory>
#include <wrl.h>
#include "Shader.h"

class ShadowMapShader : public ModelShader
{
public:
	ShadowMapShader(ID3D11Device* device);
	~ShadowMapShader() override {}

	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc)override;
	void Draw(ID3D11DeviceContext* dc, const Model* model, std::vector<SkinnedMeshResouurce::constants> buffer, const DirectX::XMFLOAT4 uvstatus = { 0,0,0,0 }, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f })override;
	void Draw(ID3D11DeviceContext* dc, const Model* model, InstancingConstants buffer, int max, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
	void End(ID3D11DeviceContext* dc) override;
	

private:
	static const int MaxBones = 128;

	static const int MAX_BONES{ 256 };
	// 定数バッファフォーマット
	struct Constants
	{
		DirectX::XMFLOAT4X4 world;			// ワールド行列
		DirectX::XMFLOAT4 materialColor;	// マテリアルカラー
		DirectX::XMFLOAT4X4 boneTransforms[MAX_BONES]{ {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1} };
	};
	
	struct ShadowConstants
	{
		DirectX::XMFLOAT4X4 shadow_projection;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> shadow_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> instan_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader[Max];
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader[Max];
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;


	DirectX::XMFLOAT4	materialColor = { 1,1,1,1 };
};