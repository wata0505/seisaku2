#pragma once
#if 0
#include"Player.h"
#include"Scene.h"
#include "Graphics.h"
#include "CameraController.h"
#include "Framebuffer.h"
#include "SubFramebuffer.h"
#include "FullscreenQuad.h"
#include "RenderContext.h"
#include "Shadowbuffer.h"
#include "StageMain.h"
#include "MetaAI.h"
#include "EffectAll.h"
#include "EffectTexAll.h"


#include <ctime>
using namespace DirectX;
// ゲームシーン
class SceneTutorial :public Scene
{
public:
	SceneTutorial() {}
	~SceneTutorial() override {}

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
	std::unique_ptr<SubFramebuffer> subframebuffers[5];
	std::unique_ptr<Shadowbuffer> shadowbuffer;

	std::unique_ptr<Sprite> sprite_batchs;

	std::unique_ptr<Sprite> claerSprite;

	std::shared_ptr<Sprite> loodSprite;

	std::unique_ptr<Sprite> renderSprite;

	//差し替え用のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> replaced_shader_resource_view;
private:
	void projectImgui();
	void RenderEnemyGauge(ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection);
private:
	std::shared_ptr<Player> player = nullptr;
	std::shared_ptr <CameraController> cameraController = nullptr;
	std::shared_ptr<Meta> meta = nullptr;
	static const int ShadowMapSize = 1096;
	StageMain* stageMain;
	// ライトの方向
	DirectX::XMFLOAT4 light_direction{ 1.0f, -1.0f, 0.0f, 0.0f };

	float shadowDrawRect = 40.0f; // シャドウマップに描画する範囲
	DirectX::XMFLOAT4X4 lightViewProjection; // ライトビュープロジェクション行列

	LuminanceExtractionData luminanceExtractionData;
	//操作説明
	std::unique_ptr<Sprite> sousasetumei[2];
	std::unique_ptr<Sprite> s;
	bool titleflag = false;
	float dissolveTimer = 0;
	//ヒント最大数
	static const int tipsMax{ 2 };
	//ヒント番号
	int tipsNo = 0;
	//ヒント表示時間
	float tipsTimer = 0;
	//ヒント
	std::unique_ptr<Sprite> tips[tipsMax];
	
	EffectAll* effectAll = nullptr;
	EffectTexAll* effectTexAll = nullptr;
};

#endif

#pragma once
#include"Player.h"
#include"Scene.h"
#include "Graphics.h"
#include "CameraController.h"
#include "Framebuffer.h"
#include "SubFramebuffer.h"
#include "FullscreenQuad.h"
#include "RenderContext.h"
#include "Shadowbuffer.h"
#include "StageMain.h"
#include "MetaAI.h"
#include "EffectAll.h"
#include "EffectTexAll.h"
#include "Base.h"

#include <ctime>
#include <string>
using namespace DirectX;
// ゲームシーン
class SceneTutorial :public Scene
{
public:
	SceneTutorial() {}
	~SceneTutorial() override {}

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
	std::unique_ptr<SubFramebuffer> subframebuffers[6];
	std::unique_ptr<Shadowbuffer> shadowbuffer;

	std::unique_ptr<Sprite> sprite_batchs2;

	std::unique_ptr<Sprite> claerSprite;

	std::unique_ptr<Sprite> waveSprite[3];

	std::shared_ptr<Sprite> loodSprite;

	std::unique_ptr<Sprite> renderSprite;

	//差し替え用のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> replaced_shader_resource_view;
private:
	void projectImgui();
private:
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<Base> base = nullptr;
	//std::shared_ptr <CameraController> cameraController = nullptr;
	std::unique_ptr<Meta> meta = nullptr;
	static const int ShadowMapSize = 1096;
	StageMain* stageMain;
	// ライトの方向
	DirectX::XMFLOAT4 light_direction{ 1.0f, -1.0f, 0.0f, 0.0f };

	float shadowDrawRect = 1.0f; // シャドウマップに描画する範囲
	DirectX::XMFLOAT4X4 lightViewProjection; // ライトビュープロジェクション行列

	LuminanceExtractionData luminanceExtractionData;

	float dissolveTimer = 0;
	float haikeiTimer = 0;

	float waveTimer = 3;

	float gameTimer = 0;

	// ジッタードリフト定数バッファ
	struct JitterDriftConstantBuffer
	{
		float jitterStrength = 0.0f;
		float time = 0.0f;
		DirectX::XMFLOAT2 dummy;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> jitterDriftConstantBuffer;	// ジッタードリフト定数バッファ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> jitterDriftPixelShader;
	std::unique_ptr<SubFramebuffer> jitterDriftSubFramebuffer;
	JitterDriftData jitterDriftData;
	float skyboxColor = 0.0f;
	DirectX::XMFLOAT3 objPos[3][3] = { {{ 50,-4,-50 },{60,-4,17},{140,-4,-80}},{{ 115,-4,-50 },{115,-4,-20},{115,-48,-80}},{{80,-4,-30 },{120,-4,-30},{100,-48,-80}} }; //オブジェクト位置
	float objAngleY[3][3] = { { 0,90,-45 },{ 90,90,90 },{ 10,-10,90 } };//オブジェクト回転
	float objAngleZ[3][3] = { { 0,0,0 },{ 0,0,90 },{ 0,0,90 } };//オブジェクト回転

private:
	//チュートリアル用変数
	enum TutorialState
	{
		STATE_ATTACK,
		STATE_OBJ_SETTING,
		STATE_RLUE_CHECK
	};
	//現在のステート
	int tutorialState = 0;
	//項目(今後増える予定あり)
	static const int checkMAX = 5;
	static const int stateMAX = 3;
	bool isTutorialCheck[checkMAX] = {};
	int checkCount = 0;
	//添え字
	enum CheckList
	{
		CHECK_ATTACK,
		CHECK_OBJ_SELECT,
		CHECK_OBJ_SET_MODE,
		CHECK_OBJ_SET,
		CHECK_RULE
	};
	
	//纏めてアップデート
	void TutorialUpdate(float elapsedTime);
	//各種チェック項目のアップデート
	void TutorialAttackUpdate(float elapsedTime);
	void TutorialObjSettingUpdate(float elapsedTime);
	void TutorialRuleCheckUpdate(float elapsedTime);

	//imgui用
	std::string stateListString[stateMAX] = {
		"STATE_ATTACK",
		"STATE_OBJ_SETTING",
		"STATE_RLUE_CHECK" };

	std::string checkListString[checkMAX] = {
		"CHECK_ATTACK",
		"CHECK_OBJ_SELECT",
		"CHECK_OBJ_SET_MODE",
		"CHECK_OBJ_SET",
		"CHECK_RULE" };
}; 