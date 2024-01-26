#pragma once

#include "Scene.h"
#include "Graphics.h"
#include "Framebuffer.h"
#include "SubFramebuffer.h"
#include "Shadowbuffer.h"
#include "FullscreenQuad.h"
#include "Player.h"
#include "Base.h"
#include "CameraController.h"

using namespace DirectX;

class SceneTitle : public Scene
{
public:
	SceneTitle() {}
	~SceneTitle() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

private:
	// テクスチャ関連の更新処理
	void UpdateTextureData(float elapsedTime);

	// ImGui描画処理
	void ImGuiRender();

private:
	// タイトルモード
	enum TitleMode
	{
		TitleStart,	// タイトル開始
		NotSelect,	// 非選択
		Select,		// 選択
		StageSelect,// ステージ選択
	};

	// ゲームモード
	enum class GamaMode
	{
		Game,		// ゲーム
		Tutorial	// チュートリアル
	};

	// ステージモード
	enum class StageMode
	{
		Stage1,		// 地上敵
		Stage2,	    // 空中敵
		Stage3      // 地上敵 + 空中敵
	};

	// ピクセルシェーダー種類
	enum PixelShaderKind
	{
		PostEffect,		// ポストエフェクト
		GaussianBlur,	// ガウシアンブラー
		RadiationBlur,	// 放射ブラー
		JitterDriftPS,	// ジッタードリフト

		PSMax
	};

	// サブフレームバッファ種類
	enum SubFrameBufferKind
	{
		Luminance,		// 輝度抽出
		Bloom,			// ブルーム
		Synthesis,		// 合成
		JitterDriftSFB,	// ジッタードリフト

		SFBMax
	};

	// スプライト種類
	enum SpriteKind
	{
		Title,			// タイトル
		DecisionText,	// 決定
		Back,			// 背景
		GameStartText,	// ゲーム
		TutorialText,	// チュートリアル
		Stage1,			// ステージ1
		Stage2,			// ステージ2
		Stage3,			// ステージ3
        StageTextFrame,	// ステージ枠組
		StageBack,		// ステージ背景
		CyberCircle,	// サイバー円

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
	std::unique_ptr<Base>							tower = nullptr;								// タワー
	std::unique_ptr<CameraController>				cameraController = nullptr;						// カメラコントローラー
	float											shadowDrawRect = 10.0f;							// シャドウマップに描画する範囲
	float											titleDissolveTimer = 0;							// タイトルログ&決定テクスチャのディゾルブ時間
	float											textDissolveTimer = 0;							// ゲーム&チュートリアルテクスチャのディゾルブ時間
	float											skyboxTimer = 0.0f;								// スカイボックス経過時間
	float											punchPosition = 0.0f;							// 殴りによる位置調整
	float											punchScale = 1.0f;								// 殴りによるサイズ調整
	float											lerpScale = 0.0f;								// ゲーム&チュートリアルテクスチャのサイズ調整
	float											punchRotate = 0.0f;								// 殴りによる回転値
	float											waitPunchTimer = 0.0f;							// ディゾルブ入るまでの遅延時間
	float											cosRadian = 0.0f;								// ゲーム&チュートリアルテクスチャを回転させる角度
	float											skyboxColor = 0.0f;								// スカイボックスの色判定
	float											deltaTime = 0.0f;								// Renderで使う処理速度
	float											selectTextureScale[3] = { 0.5f, 0.5f, 0.5f };	// ステージ毎の線形補完サイズ
	float											stageTexturePositionY[3] = { 0.0f, 0.0f, 0.0f };// ステージ毎のテクスチャのY位置
	float											selectFrameTextureScale[3] = { 0.5f, 0.5f, 0.5f };// ステージテクスチャの枠の線形補完サイズ(未使用)
	float											rotateTimer = 0.0f;								// サイバーサークルテクスチャの回転時間
	float											cyberRotate = 0.0f;								// ゲームモード選択中のサイバーサークルテクスチャの線形補完回転
	float											cyberAlpha = 0.0f;								// サイバーサークルテクスチャのアルファ値
	int												progressTimer = 0;								// 経過時間
	int												titleMode = 0;									// タイトルモード
	int												gameMode = 0;									// ゲームモード
	int												stageMode = 0;									// ステージモード
	int                                             stagNo = 0;										// 現在のステージ
	// ジッタードリフト定数バッファ
	struct JitterDriftConstantBuffer
	{
		float jitterStrength = 0.0f;
		float time = 0.0f;
		DirectX::XMFLOAT2 dummy;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> jitterDriftConstantBuffer;	// ジッタードリフト定数バッファ
	JitterDriftData jitterDriftData;
};