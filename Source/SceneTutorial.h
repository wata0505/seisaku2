#pragma once

#include "Scene.h"
#include "Graphics.h"
#include "Framebuffer.h"
#include "SubFramebuffer.h"
#include "Shadowbuffer.h"
#include "FullscreenQuad.h"
#include "Player.h"
#include "Base.h"
#include "MetaAI.h"
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
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>render_target_view;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[3];

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[3];

	std::unique_ptr<FullscreenQuad> bit_block_transfer;

	//オフスクリーンバファ
	std::unique_ptr<Framebuffer> framebuffers[2];
	std::unique_ptr<SubFramebuffer> subframebuffers[6];
	std::unique_ptr<Shadowbuffer> shadowbuffer;

	std::unique_ptr<Sprite> sprite_batchs2;
	std::unique_ptr<Sprite> waveSprite[3];
	std::unique_ptr<Sprite> waveBackSprite;
	std::unique_ptr<Sprite> pixelSprite;
	std::unique_ptr<Sprite> gameOverSprite;
	std::unique_ptr<Sprite> gameClearSprite;
	std::unique_ptr<Sprite> font = {};
	std::shared_ptr<Sprite> loodSprite;
	std::unique_ptr<Sprite> renderSprite;

private:
	void Text(SpriteShader* shader, ID3D11DeviceContext* dc, std::string s, float x, float y, float w, float h, float r, float g, float b, float a, float dissolve);
	void projectImgui();
private:
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<Base> base = nullptr;
	std::unique_ptr<Meta> meta = nullptr;
	float shadowDrawRect = 10.0f; // シャドウマップに描画する範囲
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
	float gameAfterTimer = 0.0f;
	float gameAfterDissolveTimer = 3.0f;

private:
	//チュートリアル用変数
	bool isUpdateStoper = false;

	enum TutorialState
	{
		STATE_ATTACK,
		STATE_OBJ_SETTING,
		STATE_RLUE_CHECK,
		STATE_BREAK_TIME,
		STATE_START,
	};
	//現在のステート
	int tutorialState = TutorialState::STATE_START;
	//次のステートへ向かうタイマー
	float stateTimer = 3.0f;
	float initTimer = 3.0f;
	//項目(今後増える予定あり)
	static const int checkMAX = 6;
	static const int stateMAX = 4;
	bool isTutorialCheck[checkMAX] = {};
	int checkCount = 0;
	//強調表示する為のタイマー
	float highlightTimer[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//セットモードフラグ
	bool isSetMode = false;
	//タレットセット説明フラグ
	int taretSetumeiState = 0;
	int pushCount = 0;

	int startUi = 0;

	//コントローラーかpadを繋いでいるかで表示する画像を変更
	int keyOrButton = 0;

	//添え字
	enum CheckList
	{
		CHECK_ATTACK,
		CHECK_JUMP,
		CHECK_OBJ_SELECT,
		CHECK_OBJ_SET_MODE,
		CHECK_OBJ_SET,
		CHECK_RULE
	};

	//強調表示する為の添え字
	enum Tutorial01
	{
		MOVE,
		CAMERA,
		ATTACK,
		JUMP
	};
	enum Tutorial02
	{
		OBJ_SELECT,
		OBJ_SET_MODE,
		OBJ_SET,
	};

	enum Operations
	{
		KEYBOARD = 0,
		GAMEPAD
	};

	//纏めてアップデート
	void TutorialUpdate(float elapsedTime);
	//各種チェック項目のアップデート
	void TutorialAttackUpdate(float elapsedTime);
	void TutorialObjSettingUpdate(float elapsedTime);
	void TutorialRuleCheckUpdate(float elapsedTime);
	void TutorialBreakTimeUpdate(float elapsedTime);
	void TutorialStartUpdate(float elapsedTime);
	//UIをチェンジする関数
	void ChangeUi();

	//表示UI
	std::unique_ptr<Sprite> sprTutorialState[2] = {};	//基本操作UI表示
	std::unique_ptr<Sprite> sprUiHighlight[4] = {};		//現在行った行動の強調表示
	std::unique_ptr<Sprite> sprOperationUi[2][2] = {};	//操作方法UI [フェーズ数][ボタンかキーか]
	std::unique_ptr<Sprite> sprObjectSet[2][2] = {};	//オブジェクト設置の説明
	std::unique_ptr<Sprite> sprTutorialStart[2] = {};	//チュートリアルスタートの一枚絵
	std::unique_ptr<Sprite> sprRule[2] = {};	//ルールの一枚絵

	//調性用変数
	bool isShowFlag[2] = { false, false };
	bool isHighlightFlag[4] = { false, false, false, false };
	DirectX::XMFLOAT2 pos = { 0, 220 };
	DirectX::XMFLOAT2 size = { 500, 276 };
	float scale = 0.8f;
	DirectX::XMFLOAT2 texSize = { 500, 276 };

	DirectX::XMFLOAT2 pos2 = { 500, 50 };
	DirectX::XMFLOAT2 size2 = { /*400, 600*/0, 20 };
	float scale2 = 0.8f;
	DirectX::XMFLOAT2 texSize2 = { 400, 600 };

	//imgui用
	std::string stateListString[stateMAX] = {
		"STATE_ATTACK",
		"STATE_OBJ_SETTING",
		"STATE_RLUE_CHECK"
		"STATE_BREAK_TIME" };

	std::string checkListString[checkMAX] = {
		"CHECK_ATTACK",
		"CHECK_JUMP",
		"CHECK_OBJ_SELECT",
		"CHECK_OBJ_SET_MODE",
		"CHECK_OBJ_SET",
		"CHECK_RULE" };
};