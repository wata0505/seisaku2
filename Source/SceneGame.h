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

using namespace DirectX;

class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

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
	float shadowDrawRect = 0.1f; // シャドウマップに描画する範囲

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
	DirectX::XMFLOAT3 objPos[3][3] = { {{ 50,-4,-53 },{60,-4,17},{140,-4,-80}},{{ 115,-4,-50 },{115,-4,-20},{115,-48,-80}},{{80,-4,-30 },{120,-4,-30},{100,-48,-80}} }; //オブジェクト位置
	float objAngleY[3][3] = { { 0,90,-45 },{ 90,90,90 },{ 10,-10,90 }};//オブジェクト回転
	float objAngleZ[3][3] = { { 0,0,0 },{ 0,0,90 },{ 0,0,90 }};//オブジェクト回転
	float gameAfterTimer = 0.0f;
	float gameAfterDissolveTimer = 3.0f;
	float t = 1.0f;
};