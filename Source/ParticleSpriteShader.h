#pragma once
#include <memory>
#include <wrl.h>
#include "Shader.h"
#include "ParticleSpriteManager.h"
#define MAX_INSTANCES2 2000
#define INPUT  0
#define OUTPUT 1




class ParticleSpriteShader : public SpriteShader
{
private:
	HRESULT CreateResource();//	リソースの作成(バッファ確保)
	HRESULT ComputeShader(const RenderContext& rc, ParticleSprite* particleSprite);//	コンピュート シェーダを使った演算

	void Update(const RenderContext& rc, ParticleSprite* particleSprite);
public:
	ParticleSpriteShader(ID3D11Device* device);
	~ParticleSpriteShader(){}

	void Begin(const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Sprite* sprite = nullptr) override;
	void End(const RenderContext& rc) override;


private:

	Microsoft::WRL::ComPtr<ID3D11Buffer> gPToCpuBuffer = NULL; // CPUへの書き込み用バッファ リソース
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> gPToCpuUAV = NULL;// アンオーダード アクセス ビュー
	Microsoft::WRL::ComPtr<ID3D11Buffer> gPCPUreadBackBuffer = NULL; // リードバック用バッファ リソース

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader>     computeShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>    geometryShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;


	Microsoft::WRL::ComPtr<ID3D11SamplerState>      shadowMapSamplerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;

	DirectX::XMFLOAT4	materialColor = { 0.5,0.5,0.5,1 };
	std::vector<SkinnedMeshResouurce::constants> buffers;
	//定数バッシュ
	Microsoft::WRL::ComPtr<ID3D11Buffer> sceneBuffers;

	Microsoft::WRL::ComPtr<ID3D11Buffer> colorBuffers;

	Microsoft::WRL::ComPtr<ID3D11Buffer> calculationBuffers;
	//パーティクル形
	std::shared_ptr<Sprite> moveSprite[ParticleSprite::ParticleMax];


	int chainA = 0;//バッファーの切り替え
	int chainB = 1;//バッファーの切り替え
public:
	struct ColorConstants
	{
		DirectX::XMFLOAT4 materialColor;      // カラー

	};
	
	struct SceneConstants
	{
		DirectX::XMFLOAT4X4 view;       // ビュー変換行列
		DirectX::XMFLOAT4X4 projection; // 透視変換行列
		
		
	};
	// 定数バッファフォーマット
	struct CalculationConstants
	{
		DirectX::XMFLOAT3 pattern;	// 柄座標
		float dummy1;
		DirectX::XMFLOAT3 tail;     //　剣先
		float max;                  //パーティクル最大値
		DirectX::XMFLOAT4 uvStatus;
	};

};