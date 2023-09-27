#pragma once
#include <memory>
#include <d3d11.h>
#include <sstream>
#include <wrl.h>

#include <mutex>
#include <DirectXMath.h>

#include "Shader.h"
#include "DebugRenderer.h"
#include "ImGuiRenderer.h"
#include "DefaultSpriteShader.h"
#include "SwordTrailShader.h"
#include "LuminanceExtractionShader.h"
#include "ParticleShader.h"
#include "ParticleSpriteShader.h"
#include "Calculation.h"
#include "EffectTexAll.h"

CONST LONG SCREEN_WIDTH{ 1280 };
CONST LONG SCREEN_HEIGHT{ 720 };


//サンプラーステート取得
// 0   ポイントフィルタ
// 1   リニアフィルタ
// 2   アニソトロピック
enum  SamplerState
{
	Point,
	Linear,
	Astropic
};
//     深度テスト　　深度書き込み
//  0     ON           ON
//  1     ON           OFF
//  2     OFF          ON
//  3     OFF          OFF
enum  DepthStencilState
{

	ONON,
	ONOFF,
	OFFON,
	OFFOFF
};
//ブレンドステート
// 0   無効
// 1   アルファ
// 2   加算
// 3   乗算
enum  BlendState
{

	Invalid,
	Alpha,
	Add,
	Multy
};
//ラスタライザステート
//      描画　　　  　　　　　　表示
// 0   ソリッド　　　　　　　裏面非表示
// 1   ワイヤーフレーム　　　両面表示
// 2   ソリッド　　　　　　　両面表示
enum RasterizerState
{

	SolidBack,
	WireF,
	Solid
};


class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();

	std::mutex rock;

	// インスタンス取得
	static Graphics& Instance() { 


		return *instance; 
	
	}
	enum  ModelShaderId
	{
		ModelShaderDefault,
		Toon,
		ShadowmapCaster,

		ModelShaderMax

	};

	enum  SpriteShaderId
	{
		SpriteShaderDefault,
		Skybox,
		LuminanceExtraction,
		ParticleSprite,
		InstancingSprite,

		SpriteShaderMAX
	};

	enum  DissolveSpriteId
	{
		Dissolve,
		DissolveCircle,
		DissolveDiagonal,

		DissolveSpriteMAX
	};

	// デバイス取得
	ID3D11Device* GetDevice() const { return device.Get(); }

	// デバイスコンテキスト取得
	ID3D11DeviceContext* GetDeviceContext() const { return immediateContext.Get(); }

	// スワップチェーン取得
	IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }

	// レンダーターゲットビュー取得
	ID3D11RenderTargetView* GetRenderTargetView() const { return renderTargetView.Get(); }

	// デプスステンシルビュー取得
	ID3D11DepthStencilView* GetDepthStencilView() const { return depthStencilView.Get(); }

	//サンプラーステート取得
	
	ID3D11SamplerState* GetSamplerState(int i) const { return samplerStates[i].Get(); }


	//深度テスト、深度書き込み
	
	ID3D11DepthStencilState* GetDepthStencilState(int i) const { return depthStencilStates[i].Get(); }


	//ブレンドステート
	
	ID3D11BlendState* GetBlendState(int i) const { return blendStates[i].Get(); }

	
	//ラスタライザステート
	
	ID3D11RasterizerState* GetRasterizerState(int i) const { return rasterizerStates[i].Get(); }

	
	//定数バッファ
	ID3D11Buffer* GetConstant_buffers(int i) const { return constantBuffers[i].Get(); }

	
	DirectX::XMFLOAT4X4 test;

	// 定数バッファフォーマット
	struct scene_constants
	{
		DirectX::XMFLOAT4X4 view_projection;	// ビュー・プロジェクション変換行列
		DirectX::XMFLOAT4 light_direction;		// ライトの向き
		DirectX::XMFLOAT4 camera_position;  //カメラ位置
	};


	// 定数バッファオブジェクト
	//Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers[8];
	

	// シェーダー取得
	//Shader* GetShader() const { return shader.get(); }

	// スクリーン幅取得
	float GetScreenWidth() const { return screenWidth; }

	// スクリーン高さ取得
	float GetScreenHeight() const { return screenHeight; }

	// デバッグレンダラ取得
	DebugRenderer* GetDebugRenderer() const { return debugRenderer.get(); }

	// ラインレンダラ取得
	//LineRenderer* GetLineRenderer() const { return lineRenderer.get(); }

	// ImGuiレンダラ取得
	ImGuiRenderer* GetImGuiRenderer() const { return imguiRenderer.get(); }

	SwordTrailShader* GetSwordTrailShader() const { return swordtrailShader.get(); }

	ParticleShader* GetParticleShader() const { return particleShader.get(); }

	//InstancingModelShader* GetInstancingModelShader() const { return instancingModelShader.get(); }

	//ミューテックス
	std::mutex& GetMutex() { return mutex; }
	ModelShader* GetShader(ModelShaderId id) const { return shader[static_cast<int>(id)].get(); }
	SpriteShader* GetShader(SpriteShaderId id) const { return spriteShaders[static_cast<int>(id)].get();}

	Sprite* GetDissolveSprite(DissolveSpriteId id) const { return dissolveSprite[static_cast<int>(id)].get(); }

private:
	static Graphics* instance;

	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		immediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>      samplerStates[3];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[4];
	Microsoft::WRL::ComPtr<ID3D11BlendState>        blendStates[4];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>   rasterizerStates[3];
	Microsoft::WRL::ComPtr<ID3D11Buffer>            constantBuffers[8];

	std::unique_ptr<ModelShader>					shader[static_cast<int>(ModelShaderId::ModelShaderMax)];
	std::unique_ptr<SpriteShader>					spriteShaders[static_cast<int>(SpriteShaderId::SpriteShaderMAX)];
	std::unique_ptr<SwordTrailShader>				swordtrailShader;
	std::unique_ptr<ParticleShader>				    particleShader;
	//std::unique_ptr<InstancingModelShader>          instancingModelShader;
	std::unique_ptr<DebugRenderer>					debugRenderer;
	//std::unique_ptr<LineRenderer>					    lineRenderer;
	std::unique_ptr<ImGuiRenderer>					imguiRenderer;
	std::mutex										mutex;
	std::shared_ptr<Sprite> dissolveSprite[static_cast<int>(DissolveSpriteId::DissolveSpriteMAX)];
	float	screenWidth;
	float	screenHeight;
	//std::mutex      mutex;
};