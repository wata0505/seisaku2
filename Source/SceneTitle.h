#pragma once

#include "Scene.h"
#include "Graphics.h"
#include "Framebuffer.h"
#include "SubFramebuffer.h"
#include "Shadowbuffer.h"
#include "FullscreenQuad.h"
#include "AudioAll.h"
#include "EffectAll.h"
#include "Player.h"
#include "MetaAI.h"
#include "Base.h"
#include "CameraController.h"

using namespace DirectX;

class SceneTitle : public Scene
{
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
	// ImGui描画処理
	void ImGuiRender();

private:
	// タイトルモード
	enum TitleMode
	{
		TitleStart,	// タイトル開始
		NotSelect,	// 非選択
		Select,		// 選択
	};

	// ゲームモード
	enum class GamaMode
	{
		Game,		// ゲーム
		Tutorial	// チュートリアル
	};

	// ピクセルシェーダー種類
	enum PixelShaderKind
	{
		PostEffect,		// ポストエフェクト
		GaussianBlur,	// ガウシアンブラー
		RadiationBlur,	// 放射ブラー

		PSMax
	};

	// サブフレームバッファ種類
	enum SubFrameBufferKind
	{
		Luminance,		// 輝度抽出
		Bloom,			// ブルーム
		Synthesis,		// 合成

		SFBMax
	};

	// スプライト種類
	enum SpriteKind
	{
		Skybox,			// スカイボックス
		Title,			// タイトル
		DecisionText,	// 決定
		Back,			// 背景
		GameStartText,	// ゲーム
		TutorialText,	// チュートリアル

		SMax
	};

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView;								// レンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;									// サンプラーステート
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader[PixelShaderKind::PSMax];			// ポストエフェクトピクセルシェーダー
	std::unique_ptr<Framebuffer>					framebuffer;									// オフスクリーンバファ	
	std::unique_ptr<SubFramebuffer>					subframebuffers[SubFrameBufferKind::SFBMax];	// ポストエフェクトバッファ
	std::unique_ptr<Shadowbuffer>					shadowbuffer;									// シャドウバッファ
	std::unique_ptr<FullscreenQuad>					bitBlockTransfer;								// ポストエフェクト処理
	std::shared_ptr<Sprite>							renderSprite;									// 画面に出力するテクスチャ
	std::shared_ptr<Sprite>							spriteBatchs[SpriteKind::SMax];					// タイトルでのテクスチャ
	std::unique_ptr<Player>							player = nullptr;								// プレイヤー
	std::unique_ptr<Meta>							meta = nullptr;									// MetaAI
	std::unique_ptr<Base>							tower = nullptr;								// タワー
	std::unique_ptr<CameraController>				cameraController = nullptr;						// カメラコントローラー
	float											shadowDrawRect = 10.0f;							// シャドウマップに描画する範囲
	float											titleDissolveTimer = 0;							// タイトルログ&決定テクスチャのディゾルブ時間
	float											textDissolveTimer = 0;							// ゲーム&チュートリアルテクスチャのディゾルブ時間
	float											skyboxTimer = 0.0f;								// スカイボックス経過時間
	float											dropPositionY = 0.0f;							// 非選択のゲームモードを蹴落とす位置
	float											punchPosition = 0.0f;							// 殴りによる位置調整
	float											punchScale = 1.0f;								// 殴りによるサイズ調整
	float											lerpScale = 0.0f;								// ゲーム&チュートリアルテクスチャのサイズ調整
	float											punchRotate = 0.0f;								// 殴りによる回転値
	float											cosRadian = 0.0f;								// ゲーム&チュートリアルテクスチャを回転させる角度
	int												progressTimer = 0;								// 経過時間
	int												titleMode = 0;									// タイトルモード
	int												gameMode = 0;									// ゲームモード
};