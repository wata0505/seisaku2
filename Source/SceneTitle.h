#pragma once
#include"Scene.h"
#include "Graphics.h"
#include "CameraController.h"
#include "Framebuffer.h"
#include "FullscreenQuad.h"
#include "RenderContext.h"
#include "Shadowbuffer.h"
#include "StageMain.h"
#include "MetaAI.h"
#include "EffectAll.h"
#include "SubFramebuffer.h"
#include "AudioSource.h"
#include"AudioAll.h"
#include <ctime>
using namespace DirectX;
// ゲームシーン
class SceneTitle :public Scene
{
public:
	enum class GamaMode
	{
		Default,
		Tutorial
	};
public:
	SceneTitle() {}
	~SceneTitle() override {}

	// 初期化
	void Initialize()override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;



private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	////様々な描画命令をGPUに伝えるやつ
	//ID3D11DeviceContext* immediate_context;
	////キャンバスに描いた画を額（ウィンドウ）にいれるやつ
	//IDXGISwapChain* swap_chain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>immediate_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain>swap_chain;
	//色を書き込むキャンバス
	//ID3D11RenderTargetView* render_target_view;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>render_target_view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>depth_stencil_view;
	//奥行き情報を書き込むキャンバス

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[3];

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[3];


	//差し替え用ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader>replaced_pixel_shader;

	std::unique_ptr<FullscreenQuad> bit_block_transfer;

	//スキンメッシュ
	//std::unique_ptr<SkinnedMesh> SkinnedMeshes[8];
	//オフスクリーンバファ
	std::unique_ptr<Framebuffer> framebuffers[2];
	std::unique_ptr<SubFramebuffer> subframebuffers[3];
	std::unique_ptr<Shadowbuffer> shadowbuffer;

	std::shared_ptr<Sprite> spriteBatchs[7];

	std::shared_ptr<Sprite> renderSprite;

	//差し替え用のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> replaced_shader_resource_view;
private:
	void projectImgui();
private:
	std::unique_ptr <CameraController> cameraController = nullptr;
	static const int ShadowMapSize = 2096;
	StageMain* stageMain;
	// ライトの方向
	DirectX::XMFLOAT4 light_direction{ 1.0f, -1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 shadowlight_direction{ 1.0f, -1.0f, 0.0f, 0.0f };
	float light = 300;
	float shadowDrawRect = 330.0f; // シャドウマップに描画する範囲
	DirectX::XMFLOAT4X4 lightViewProjection; // ライトビュープロジェクション行列
	DirectX::XMFLOAT3 position = { 0,0,0 };

	//Sprite描画用

	EffectAll* effectAll = nullptr;
	int timer = 0;
	int titleMode = 0;
	int gameMode = 0;
	float dissolveTimer = 0;
	float dissolveDTimer = 0;
	EffectTexAll* effectTexAll = nullptr;
	//std::unique_ptr<AudioSource> titlebgm;
	AudioAll* audioAll = nullptr;
	LuminanceExtractionData luminanceExtractionData;
};