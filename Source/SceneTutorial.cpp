#include "SceneTutorial.h"
#include "SceneLoading.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "Shader.h"
#include "AudioAll.h"
#include "EffectManager.h"
#include "ParticleSystem.h"
#include "ParticleManager.h"
#include "ParticleSpriteManager.h"
#include "InstancingSpriteManager.h"
#include "Camera.h"
#include "StageMain.h"
#include "StageObj.h"
#include "StageManager.h"
#include "EnemySystem.h"
#include "EnemyManager.h"
#include "TrapManager.h"
#include "LightManager.h"
#include "LockOnUI.h"
#include "UIManager.h"
#include "Input.h"
#include "Misc.h"

void SceneTutorial::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11Device* device = graphics.GetDevice();

	EnemyManager& enemyManager = EnemyManager::Instance();

	EnemySystem& enemySystem = EnemySystem::Instance();
	enemySystem.Start(SceneManager::Instance().GetStage());
	StageManager& stageManager = StageManager::Instance();
	stageManager.Clear();
	StageMain* stageMain = new StageMain();
	stageManager.Register(stageMain);

	DirectX::XMFLOAT2 dir = { 0,0 };
	//DirectX::XMFLOAT3 objPos[3] = { { 50,-4,-50 },{60,-4,17},{130,-4,-80} };
	//float objAngle[3] = { 0,90,-45 };
	int s = SceneManager::Instance().GetStage();
	//DirectX::XMFLOAT3 objPos[3][3] = { {{ 50,-4,-50 },{60,-4,17},{140,-4,-80}},{{ 115,-4,-50 },{115,-4,-20},{115,-45,-80}},{{0,-4,-70 },{0,-4,-70},{115,-45,-80}} }; //オブジェクト位置//
	for (int i = 0; i < 3; i++) {
		StageObj* stageObj = new StageObj(objPos[s][i], objAngleY[s][i], objAngleZ[s][i]);
		stageManager.Register(stageObj);
	}

	// カメラ初期化
	{
		Camera& camera = Camera::Instance();
		camera.SetLookAt(DirectX::XMFLOAT3(0, 0, -10), {}, DirectX::XMFLOAT3(0, 1, 0));
		camera.SetPerspectiveFov(DirectX::XMConvertToRadians(45),
			graphics.GetScreenWidth() / graphics.GetScreenHeight(),
			0.1f, 500.0f);
	}
	//カメラコントローラー初期化
	//cameraController = std::make_unique <CameraController>();
	float gaugeWidth = 500.0f;
	float gaugeHeight = 500.0f;
	BaseUI* hpWaku = new BaseUI(L".\\resources\\kaba2.png", 10.0f, 395.0f, gaugeWidth * 0.9f, gaugeHeight * 0.9f);
	DirectX::XMFLOAT3 pos = { 100.0f, -2.5f, -139.975f };
	player = std::make_unique<Player>();
	base = std::make_unique<Base>(pos);
	//ピクセルシェーダーオブジェクト
	create_ps_from_cso(device, "Shader\\PostEffectPS.cso", pixel_shaders[0].GetAddressOf());
	create_ps_from_cso(device, "Shader\\BlurPS.cso", pixel_shaders[1].GetAddressOf());
	create_ps_from_cso(device, "Shader\\EffectPS.cso", pixel_shaders[2].GetAddressOf());


	// ジッタードリフト定数バッファ
	{
		// 定数バッファオブジェクト作成
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth = sizeof(JitterDriftConstantBuffer);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, jitterDriftConstantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	create_ps_from_cso(device, "Shader\\JitterDriftPS.cso", jitterDriftPixelShader.GetAddressOf());

	bit_block_transfer = std::make_unique<FullscreenQuad>(device);

	sprite_batchs2 = std::make_unique<Sprite>();
	waveSprite[0] = std::make_unique<Sprite>(L".\\resources\\UI\\1Wave.png");
	waveSprite[1] = std::make_unique<Sprite>(L".\\resources\\UI\\2Wave.png");
	waveSprite[2] = std::make_unique<Sprite>(L".\\resources\\UI\\FinalWave.png");
	waveBackSprite = std::make_unique<Sprite>(L".\\resources\\UI\\Title\\StageBack.png");
	pixelSprite = std::make_unique<Sprite>(L".\\resources\\UI\\Pixel.png");
	gameOverSprite = std::make_unique<Sprite>(L".\\resources\\UI\\GameOver.png");
	gameClearSprite = std::make_unique<Sprite>(L".\\resources\\UI\\GameClear.png");
	font = std::make_unique<Sprite>(L".\\resources\\Font\\Number.png");
	loodSprite = EffectTexAll::Instance().GetSprite(int(EffectTexAll::EfTexAll::Bock));
	renderSprite = std::make_unique<Sprite>();
	//スクリーンバファ生成
	framebuffers[0] = std::make_unique<Framebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
	subframebuffers[0] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
	subframebuffers[1] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	subframebuffers[2] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
	subframebuffers[3] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
	subframebuffers[4] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
	subframebuffers[5] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
	jitterDriftSubFramebuffer = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
	// シャドウバッファ
	static const int ShadowMapSize = 2096;
	shadowbuffer = std::make_unique<Shadowbuffer>(device, ShadowMapSize, ShadowMapSize);

	meta = std::make_unique<Meta>(player.get(), &enemyManager);
	UIManager& uiManager = UIManager::Instance();
	LockOnUI* lock = new LockOnUI(L".\\resources\\Lockon.png");

	//チュートリアルUI関連
	sprTutorialState[0] = std::make_unique<Sprite>(L".\\resources\\UI\\Tutorial\\Tutorial_state1.png");
	sprTutorialState[1] = std::make_unique<Sprite>(L".\\resources\\UI\\Tutorial\\titorial_state2.png");
	for (int i = 0; i < 4; i++)
	{
		sprUiHighlight[i] = std::make_unique<Sprite>(L".\\resources\\UI\\Tutorial\\highlight.png");
	}
	//操作方法関連
	//フェーズ1
	sprOperationUi[TutorialState::STATE_ATTACK][Operations::KEYBOARD] = std::make_unique<Sprite>(L".\\resources\\UI\\Tutorial\\phase1Keys.png");
	sprOperationUi[TutorialState::STATE_ATTACK][Operations::GAMEPAD] = std::make_unique<Sprite>(L".\\resources\\UI\\Tutorial\\phase1Buttons.png");
	//フェーズ2
	sprOperationUi[TutorialState::STATE_OBJ_SETTING][Operations::KEYBOARD] = std::make_unique<Sprite>(L".\\resources\\UI\\Tutorial\\phase2Keys.png");
	sprOperationUi[TutorialState::STATE_OBJ_SETTING][Operations::GAMEPAD] = std::make_unique<Sprite>(L".\\resources\\UI\\Tutorial\\phase2Buttons.png");
}
void SceneTutorial::Finalize()
{

	StageManager::Instance().Clear();
	EnemyManager::Instance().Clear();
	ParticleManager::Instance().Clear();
	InstancingSpriteManager::Instance().Clear();
	ParticleSpriteManager::Instance().Clear();
	LightManager::Instance().Clear();
	UIManager::Instance().Clear();
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Bgm)->Stop();
	TrapManager::Instance().Clear();

}
void SceneTutorial::Update(float elapsedTime)
{
	// ゲームBGM再生
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Bgm)->Play(true, 0.3f);
	float deltaTimer = elapsedTime;
	if (gameAfterTimer >= 1.0f)
	{
		deltaTimer = 0.0f;
	}
	if (EnemySystem::Instance().GetWave() < 3)
	{
		player->update(elapsedTime);
	}
	else
	{
		player->ClearUpdate(elapsedTime);
	}
	base->Update(deltaTimer);
	StageManager::Instance().Update(deltaTimer);
	EnemySystem::Instance().Update(deltaTimer);
	EnemyManager::Instance().Update(deltaTimer);

	EffectManager::instance().Update(deltaTimer);

	ParticleManager::Instance().Update(deltaTimer);

	InstancingSpriteManager::Instance().Update(deltaTimer);

	ParticleSpriteManager::Instance().Update(deltaTimer);

	LightManager::Instance().Update(deltaTimer);

	UIManager::Instance().Update(deltaTimer);

	TrapManager::Instance().Update(deltaTimer);
#if 0
	if (Base::Instance().GetJitterStrength() > 0) {
		jitterDriftData.jitterStrength = Base::Instance().GetJitterStrength();
	}
	jitterDriftData.jitterStrength -= elapsedTime;
	if (jitterDriftData.jitterStrength < 0)jitterDriftData.jitterStrength = 0;
#else
	if (Base::Instance().GetSkyboxColor() > 0.0f)
	{
		skyboxColor = Base::Instance().GetSkyboxColor();
	}
	skyboxColor -= elapsedTime;
	if (skyboxColor <= 0.0f)
	{
		skyboxColor = 0.0f;
	}
	jitterDriftData.jitterStrength = skyboxColor / 200.0f;
#endif
	if (base->GetHP() != 0 && player->GetHealth() > 0 && EnemySystem::Instance().GetWave() < 3)
	{
		dissolveTimer += elapsedTime;
		if (dissolveTimer > 3.0f)
		{
			dissolveTimer = 3.0f;
		}
	}
	else
	{
		float waitTimer = 2.0f;
		if (EnemySystem::Instance().GetWave() == 3)
		{
			waitTimer = 3.5f;
		}
		if (player->GetLoseDirectingTimer() >= waitTimer)
		{
			gameAfterTimer += elapsedTime;
			if (gameAfterTimer >= 1.0f)
			{
				gameAfterTimer = 1.0f;
				gameAfterDissolveTimer -= elapsedTime * 3.0f;
				if (gameAfterDissolveTimer < 0.0f)
				{
					gameAfterDissolveTimer = 0.0f;
				}
			}
			if (player->GetLoseDirectingTimer() >= waitTimer + 1.5f)
			{
				if (EnemySystem::Instance().GetWave() < 3)
				{
					jitterDriftData.jitterStrength += elapsedTime * 0.1f;
					if (jitterDriftData.jitterStrength >= 0.05f)
					{
						if (jitterDriftData.jitterStrength >= 0.1f)
						{
							jitterDriftData.jitterStrength = 0.1f;
						}
						dissolveTimer -= elapsedTime;
					}
				}
				else
				{
					dissolveTimer -= elapsedTime;
				}
			}
		}
	}

	haikeiTimer += deltaTimer;
	if (EnemySystem::Instance().GetWaveTimer() > 3 && EnemySystem::Instance().GetWaveTimer() < 8)
	{
		waveTimer -= elapsedTime;
		if (waveTimer < 0.0f)
		{
			waveTimer = 0.0f;
		}
		else if (waveTimer > 0.0f && waveTimer <= 1.0f)
		{
			AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Text)->Play(false, 0.3f);
		}
	}
	else
	{
		waveTimer += elapsedTime;
		if (waveTimer > 3.0f)
		{
			waveTimer = 3.0f;
		}
		else if (waveTimer > 0.0f && waveTimer <= 1.0f)
		{
			AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Text)->Play(false, 0.3f);
		}
	}

	//コントローラー接続状態でUIを変更する
	ChangeUi();
	//チュートリアル呼び出し
	TutorialUpdate(elapsedTime);

	gameTimer++;
}
void SceneTutorial::Render()
{

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
	render_target_view = graphics.GetRenderTargetView();
	sampler_states[0] = graphics.GetSamplerState(0);
	sampler_states[1] = graphics.GetSamplerState(1);
	sampler_states[2] = graphics.GetSamplerState(2);

	//塗りつぶす色の設定
	FLOAT color[]{ 0.9f,0.9f,0.2f,1.0f };
	//レンダーターゲットビューのクリア。キャンバス全体を指定した色で塗りつぶす。
	//クリアするレンダーターゲットビューとクリアする色を指定
	immediate_context->ClearRenderTargetView(render_target_view.Get(), color);
	//深度ステンシルビューのクリア。キャンバス全体の奥行情報をリセット。
	//クリアする深度ステンシルビューとクリアする深度ステンシルのフラグ、深度バッファをクリアする
	immediate_context->ClearDepthStencilView(graphics.GetDepthStencilView(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//これから描くキャンバスを指定する
	immediate_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), graphics.GetDepthStencilView());

	immediate_context->PSSetSamplers(0, 1, sampler_states[0].GetAddressOf());
	immediate_context->PSSetSamplers(1, 1, sampler_states[1].GetAddressOf());
	immediate_context->PSSetSamplers(2, 1, sampler_states[2].GetAddressOf());

	RenderContext rc;
	//シャドウバッファ
	shadowbuffer->clear(immediate_context);
	shadowbuffer->activate(immediate_context);
	// ライトの方向
	DirectX::XMFLOAT4 light_direction = { 1.0f, -1.0f, 0.0f, 0.0f };
	{
		DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat4(&light_direction);
		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&player->GetPosition());
		LightPosition = DirectX::XMVectorScale(LightPosition, 60.0f);
		DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(Position - LightPosition,
			Position,
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		// シャドウマップに描画したい範囲の射影行列を生成
		DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f,
			100.0f);
		DirectX::XMStoreFloat4x4(&rc.view, V);
		DirectX::XMStoreFloat4x4(&rc.projection, P);
		DirectX::XMFLOAT4X4 lightViewProjection;
		DirectX::XMStoreFloat4x4(&lightViewProjection, V * P);
		rc.shadowMapData.lightViewProjection = lightViewProjection;
		rc.shadowMapData.shadowMap = shadowbuffer->SrvShadow.Get();
	}
	ModelShader* shader = graphics.GetShader(Graphics::ModelShaderId::ShadowmapCaster);
	shader->Begin(immediate_context, rc);
	StageManager::Instance().Render(immediate_context, shader);
	player->render(immediate_context, shader);
	//base->Render(immediate_context,shader);

	//EnemyManager::Instance().Render(immediate_context, shader);
	StageManager::Instance().InstaningRender(immediate_context, shader);
	shader->End(immediate_context);
	shadowbuffer->deactivate(immediate_context);

	immediate_context->OMSetDepthStencilState(graphics.GetDepthStencilState(3), 0);

	//２D用にラスタライザステートの設定をGPU側に指定
	immediate_context->RSSetState(graphics.GetRasterizerState(2));

	//オフスクリーンバッファ
	framebuffers[0]->Clear(immediate_context);
	framebuffers[0]->Activate(immediate_context);

	//深度テスト：オン、深度ライト：オンの深度ステンシルステートを設定しておく
	immediate_context->OMSetDepthStencilState(graphics.GetDepthStencilState(0), 1);

	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.deviceContext = immediate_context;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	rc.lightDirection = light_direction;

	SpriteShader* shader2 = graphics.GetShader(Graphics::SpriteShaderId::Skybox);

	shader2->Begin(rc);
	loodSprite->Render(immediate_context,
		0.0f, 0.0f, 1280, 720.0f,
		0.0f, 0.0f, static_cast<float>(loodSprite->GetTextureWidth()), static_cast<float>(loodSprite->GetTextureHeight()),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		skyboxColor, haikeiTimer, 0.0f
	);
	//sprite_batchs[0]->SetShaderResourceView(framebuffers[0]->shaderResourceViews[2], 1280, 720);
	shader2->Draw(rc, loodSprite.get());
	shader2->End(rc);

	//デフォルトシェーダー
	shader = graphics.GetShader(Graphics::ModelShaderId::ModelShaderDefault);

	shader->Begin(immediate_context, rc);
	base->Render(immediate_context, shader);
	StageManager::Instance().Render(immediate_context, shader);
	player->render(immediate_context, shader);
	EnemyManager::Instance().Render(immediate_context, shader);
	StageManager::Instance().InstaningRender(immediate_context, shader);
	TrapManager::Instance().Render(immediate_context, shader);

	shader->End(immediate_context);
	//レンダーターゲット切り替え
	immediate_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), graphics.GetDepthStencilView());
	//基本リソースデータ挿入
	//subframebuffers[5]->Clear(immediate_context);
	//subframebuffers[5]->Activate(immediate_context);
	//bit_block_transfer->blit(immediate_context, framebuffers[0]->shaderResourceViews[0].GetAddressOf(), 0, 1, nullptr);
	//subframebuffers[5]->Deactivate(immediate_context);
	//immediate_context->PSSetShaderResources(8, 1, framebuffers[0]->shaderResourceViews[2].GetAddressOf());
	////0622 Slot2にGBufferのPosition
	//immediate_context->PSSetShaderResources(9, 1, framebuffers[0]->shaderResourceViews[3].GetAddressOf());
	//immediate_context->PSSetShaderResources(3, 1, subframebuffers[5]->shaderResourceViews.GetAddressOf());
	//immediate_context->PSSetShaderResources(11, 1, framebuffers[0]->shaderResourceViews[1].GetAddressOf());
	framebuffers[0]->RenderActivate(immediate_context);
	//デバック関係
	projectImgui();
	player->DrawDebugGUI();
	//EnemyManager::Instance().DrawDebugGUI();
	//graphics.GetDebugRenderer()->Render(immediate_context, rc.view, rc.projection);

	//モデルパーティクル
	ParticleShader* shader4 = graphics.GetParticleShader();
	shader4->Begin(immediate_context, rc);

	ParticleManager::Instance().Render(immediate_context, shader4);
	shader4->End(immediate_context);
	//トーンシェーダー
	shader = graphics.GetShader(Graphics::ModelShaderId::Toon);
	shader->Begin(immediate_context, rc);
	player->Afterimagerender(immediate_context, shader);
	EnemyManager::Instance().Afterimagerender(immediate_context, shader);
	TrapManager::Instance().Afterimagerender(immediate_context, shader);
	shader->End(immediate_context);
	//エフェクトバッファ
	framebuffers[0]->EffectActivate(immediate_context);
	EffectManager::instance().Render(rc.view, rc.projection);

	//スプライトデフォルト
	shader2 = graphics.GetShader(Graphics::SpriteShaderId::SpriteShaderDefault);
	//剣の軌跡
	SwordTrailShader* shader3 = graphics.GetSwordTrailShader();
	rc.deviceContext->PSSetSamplers(2, 1, sampler_states[2].GetAddressOf());
	rc.deviceContext->PSSetSamplers(1, 1, sampler_states[1].GetAddressOf());
	shader3->Begin(rc);
	player->SwordTrailrender(rc, shader3);
	shader3->End(rc);
	//スプライトパーティクルGPU
	shader2 = graphics.GetShader(Graphics::SpriteShaderId::ParticleSprite);
	rc.deviceContext = immediate_context;
	shader2->Begin(rc);
	shader2->Draw(rc);
	shader2->End(rc);
	//インスタンシングスプライト
	shader2 = graphics.GetShader(Graphics::SpriteShaderId::InstancingSprite);
	rc.deviceContext = immediate_context;
	shader2->Begin(rc);
	shader2->Draw(rc);
	shader2->End(rc);


	framebuffers[0]->Deactivate(immediate_context);
	//サブオフスクリーンバッファ（輝度抜き出し）
	subframebuffers[0]->Clear(immediate_context);
	subframebuffers[0]->Activate(immediate_context);
	shader2 = graphics.GetShader(Graphics::SpriteShaderId::LuminanceExtraction);
	shader2->Begin(rc);
	renderSprite->SetShaderResourceView(framebuffers[0]->shaderResourceViews[0].Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	shader2->Draw(rc, renderSprite.get());
	subframebuffers[0]->Deactivate(immediate_context);
	//サブオフスクリーンバッファ（圧縮テクスチャ）
	subframebuffers[1]->Clear(immediate_context);
	subframebuffers[1]->Activate(immediate_context);
	//bit_block_transfer->blit(immediate_context, subframebuffers[0]->shaderResourceViews.GetAddressOf(), 0, 1, pixel_shaders[1].Get());
	bit_block_transfer->blit(immediate_context, subframebuffers[0]->shaderResourceViews.GetAddressOf(), 0, 1, nullptr);
	subframebuffers[1]->Deactivate(immediate_context);
	//サブオフスクリーンバッファ（ブルーム）
	subframebuffers[2]->Clear(immediate_context);
	subframebuffers[2]->Activate(immediate_context);
	bit_block_transfer->blit(immediate_context, subframebuffers[1]->shaderResourceViews.GetAddressOf(), 0, 1, pixel_shaders[1].Get());
	subframebuffers[2]->Deactivate(immediate_context);
	////サブオフスクリーンバッファ（合成）
	subframebuffers[3]->Clear(immediate_context);
	subframebuffers[3]->Activate(immediate_context);
	ID3D11ShaderResourceView* shaderResourceViews[2] = { framebuffers[0]->shaderResourceViews[8].Get(),subframebuffers[2]->shaderResourceViews.Get() };
	{
		bit_block_transfer->blit(immediate_context, shaderResourceViews, 0, 2, pixel_shaders[0].Get());
	}
	subframebuffers[3]->Deactivate(immediate_context);
	subframebuffers[4]->Clear(immediate_context);
	subframebuffers[4]->Activate(immediate_context);
	bit_block_transfer->blit(immediate_context, subframebuffers[3]->shaderResourceViews.GetAddressOf(), 0, 1);
	if (player->GetQuickflag()) 
	{
		//放射ブラー
		bit_block_transfer->blit(immediate_context, subframebuffers[3]->shaderResourceViews.GetAddressOf(), 0, 1, pixel_shaders[2].Get());
	}

	subframebuffers[4]->Deactivate(immediate_context);

	// ジッタードリフトシェーダー
	jitterDriftData.time += 1.0f / 60.0f;
	jitterDriftSubFramebuffer->Clear(immediate_context);
	jitterDriftSubFramebuffer->Activate(immediate_context);
	bit_block_transfer->JitterDrift(immediate_context, subframebuffers[4]->shaderResourceViews.GetAddressOf(), 13, 1, jitterDriftData, jitterDriftPixelShader.Get());
	jitterDriftSubFramebuffer->Deactivate(immediate_context);

	float screenWidth = static_cast<float>(graphics.GetScreenWidth());
	float screenHeight = static_cast<float>(graphics.GetScreenHeight());
	shader2 = graphics.GetShader(Graphics::SpriteShaderId::SpriteShaderDefault);
	shader2->Begin(rc);
	//sprite_batchs2->SetShaderResourceView(subframebuffers[4]->shaderResourceViews.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	sprite_batchs2->SetShaderResourceView(jitterDriftSubFramebuffer->shaderResourceViews.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	sprite_batchs2->Render(immediate_context,
		0.0f, 0.0f, 1280.0f, 720.0f,
		0.0f, 0.0f, static_cast<float>(sprite_batchs2->GetTextureWidth()), static_cast<float>(sprite_batchs2->GetTextureHeight()),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f
	);
	shader2->Draw(rc, sprite_batchs2.get());
	if (player->GetLoseDirectingTimer() <= 0.0f)
	{
		//player->Sprite2DRender(immediate_context, rc, shader2);
		TrapManager::Instance().Sprite2DRender(immediate_context, rc, shader2);
		UIManager::Instance().Render(rc, shader2);
		base->HpDisplay(rc, shader2);
		int wave = EnemySystem::Instance().GetWave();
		if (wave < 3)
		{
			float waveCountWidth = static_cast<float>(waveSprite[wave]->GetTextureWidth());
			float waveCountHeight = static_cast<float>(waveSprite[wave]->GetTextureHeight());
			float waveBackWidth = static_cast<float>(waveBackSprite->GetTextureWidth());
			float waveBackHeight = static_cast<float>(waveBackSprite->GetTextureHeight());
			waveBackSprite->Render(immediate_context,
				screenWidth * 0.5f - waveCountWidth * 0.5f, screenHeight * 0.075f, waveCountWidth, waveCountHeight,
				0.0f, 0.0f, waveBackWidth, waveBackHeight,
				0.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				waveTimer
			);
			shader2->Draw(rc, waveBackSprite.get());
			waveSprite[wave]->Render(immediate_context,
				screenWidth * 0.5f - waveCountWidth * 0.5f, screenHeight * 0.075f, waveCountWidth, waveCountHeight,
				0.0f, 0.0f, waveCountWidth, waveCountHeight,
				0.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				waveTimer
			);
			shader2->Draw(rc, waveSprite[wave].get());

			if (EnemySystem::Instance().GetWaveTimer() > 8)
			{
				waveBackSprite->Render(immediate_context,
					screenWidth * 0.5f - waveCountWidth * 0.1f, screenHeight * 0.025f, waveCountWidth * 0.3f, waveCountHeight,
					0.0f, 0.0f, waveBackWidth, waveBackHeight,
					0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					3.0f - waveTimer
				);
				shader2->Draw(rc, waveBackSprite.get());
				static constexpr float size = 75.0f;
				int waveTime = static_cast<int>(EnemySystem::Instance().GetWaveMaxTimer(EnemySystem::Instance().GetWave()) - EnemySystem::Instance().GetWaveTimer());
				float shiftPosition = 0.5f;
				if (waveTime < 10)
				{
					shiftPosition = 0.25f;
				}
				Text(shader2, immediate_context, std::to_string(waveTime),
					screenWidth * 0.5f - size * shiftPosition, screenHeight * 0.05f,
					size, size,
					1.0f, 0.0f, 0.0f, 1.0f,
					3.0f - waveTimer);
			}
		}
	}
	// ゲーム終了後テクスチャ
	{
		pixelSprite->Render(immediate_context,
			0.0f, 0.0f, screenWidth, screenHeight,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f,
			0.0f, 0.0f, 0.0f, gameAfterTimer * 0.5f,
			0.0f, 0.0f, 0.0f
		);
		shader2->Draw(rc, pixelSprite.get());
		float width = static_cast<float>(gameOverSprite->GetTextureWidth());
		float height = static_cast<float>(gameOverSprite->GetTextureHeight());
		if (gameAfterDissolveTimer < 3.0f)
		{
			if (base->GetHP() == 0 || player->GetHealth() <= 0 || EnemySystem::Instance().GetWave() < 3)
			{
				gameOverSprite->Render(immediate_context,
					screenWidth * 0.5f - width, screenHeight * 0.5f - height, width * 2.0f, height * 2.0f,
					0.0f, 0.0f, width, height,
					0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					gameAfterDissolveTimer, 0.0f, 0.0f
				);
				shader2->Draw(rc, gameOverSprite.get());
			}
			else
			{
				gameClearSprite->Render(immediate_context,
					screenWidth * 0.5f - width, screenHeight * 0.5f - height, width * 2.0f, height * 2.0f,
					0.0f, 0.0f, width, height,
					0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					gameAfterDissolveTimer, 0.0f, 0.0f
				);
				shader2->Draw(rc, gameClearSprite.get());
			}
		}
	}
	loodSprite->Render(immediate_context,
		0.0f, 0.0f, 1280.0f, 720.0f,
		0.0f, 0.0f, static_cast<float>(loodSprite->GetTextureWidth()), static_cast<float>(loodSprite->GetTextureHeight()),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		dissolveTimer, 0.0f, 0.0f
	);
	shader2->Draw(rc, loodSprite.get());
	if (base->GetHP() == 0 || player->GetHealth() <= 0 || EnemySystem::Instance().GetWave() == 3)
	{
		//shader2->Draw(rc, claerSprite.get());
		if (dissolveTimer < 0)
		{
			// ゲームBGM停止
			AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Bgm)->Stop();
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		}
	}

	//チュートリアルUI
	{
		//基本操作
		//フェーズ1
		if (isShowFlag[0])
		{
			sprTutorialState[0]->Render(immediate_context,
				pos.x, pos.y,						//座標
				size.x * scale, size.y * scale,		//画像サイズ
				0, 0, texSize.x, texSize.y,			//テクスチャ座標、サイズ
				0,									//角度
				1, 1, 1, 1);						//agba
			shader2->Draw(rc, sprTutorialState[0].get());

			//キー、ボタン表示
			sprOperationUi[0][keyOrButton]->Render(immediate_context,
				pos.x, pos.y,						//座標
				size.x * scale, size.y * scale,		//画像サイズ
				0, 0, texSize.x, texSize.y,			//テクスチャ座標、サイズ
				0,									//角度
				1, 1, 1, 1);						//agba
			shader2->Draw(rc, sprOperationUi[0][keyOrButton].get());
		}
		//フェーズ2
		if (isShowFlag[1])
		{
			//設置操作
			sprTutorialState[1]->Render(immediate_context,
				pos.x, pos.y,						//座標
				size.x * scale, size.y * scale,		//画像サイズ
				0, 0, texSize.x, texSize.y,			//テクスチャ座標、サイズ
				0,									//角度
				1, 1, 1, 1);						//agba
			shader2->Draw(rc, sprTutorialState[1].get());

			//キー、ボタン表示
			sprOperationUi[1][keyOrButton]->Render(immediate_context,
				pos.x, pos.y,						//座標
				size.x * scale, size.y * scale,		//画像サイズ
				0, 0, texSize.x, texSize.y,			//テクスチャ座標、サイズ
				0,									//角度
				1, 1, 1, 1);						//agba
			shader2->Draw(rc, sprOperationUi[1][keyOrButton].get());
		}

		//強調表示
		for (int i = 0; i < 4; i++)
		{
			if (isHighlightFlag[i])
			{
				sprUiHighlight[i]->Render(immediate_context,
					pos.x, pos.y + (10 * scale) + (i * (64 * scale)),		//座標
					500 * scale, 64 * scale,					//画像サイズ
					0, 0, 720, 64,								//テクスチャ座標、サイズ
					0,											//角度
					1, 1, 1, 1);								//agba
				shader2->Draw(rc, sprUiHighlight[i].get());
			}
		}
	}

	shader2->End(rc);
	if (player->GetDidTimer() < NULL) {
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
}
void SceneTutorial::Text(SpriteShader* shader, ID3D11DeviceContext* dc, std::string s, float x, float y, float w, float h, float r, float g, float b, float a, float dissolve)
{
	RenderContext rc;
	rc.deviceContext = dc;

	//現在の文字位置（相対位置）
	float carriage = 0;

	//文字数分だけrender()を呼び出す
	for (const char c : s)
	{
		//文字を表示。アスキーコードの位置にある文字位置を切り抜いて表示
		font->Render(dc,
			x + carriage / 2, y, w, h,
			72.0f * (c & 0x0F), 0.0f, 72.0f, 72.0f,
			0.0f,
			r, g, b, a,
			dissolve);
		shader->Draw(rc, font.get());
		//文字位置を幅分ずらす
		carriage += w;
	}
}
void SceneTutorial::projectImgui()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_None))
	{
	}
	TrapManager::Instance().DrawDebugGUI();
	if (ImGui::TreeNode("Shadowmap"))
	{
		ImGui::SliderFloat("DrawRect", &shadowDrawRect, 1.0f, 150.0f);
		//ImGui::SliderFloat("DrawRect2", &shadowDrawRect2, 0.0f, 48.0f);
		//ImGui::ColorEdit3("Color", &shadowColor.x);
		//ImGui::SliderFloat("Bias", &shadowBias, 0.0f, 0.1f);
		ImGui::Text("texture");
		ImGui::Image(shadowbuffer->SrvShadow.Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });

		//ImGui::Image(framebuffers[0]->shaderResourceViews[0].Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });

		ImGui::TreePop();

	}
	if (ImGui::TreeNode("MRT"))
	{
		ImGui::Text("texture");
		ImGui::Image(framebuffers[0]->shaderResourceViews[0].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[1].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[2].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[3].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[4].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[5].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[6].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[7].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[8].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(subframebuffers[0]->shaderResourceViews.Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(subframebuffers[1]->shaderResourceViews.Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(subframebuffers[2]->shaderResourceViews.Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(subframebuffers[3]->shaderResourceViews.Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(subframebuffers[4]->shaderResourceViews.Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("JitterDrift"))
	{
		ImGui::SliderFloat("JitterStrength", &jitterDriftData.jitterStrength, 0.0f, 1.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("enemy"))
	{
		if (ImGui::Button(u8"kill"))
		{
			int max = EnemyManager::Instance().GetEnemyCount();
			for (int i = 0; i < max; i++) {
				if (EnemyManager::Instance().GetEnemy(i)->GetActiveflag()) {
					EnemyManager::Instance().GetEnemy(i)->Destroy();

				}

			}

		}
	}
	//チュートリアル
	if (ImGui::TreeNode("Tutorial"))
	{
		//現在のステート
		ImGui::Text("Now State →");
		ImGui::SameLine();
		ImGui::Text(stateListString[tutorialState].c_str());

		//タイマー
		ImGui::InputFloat("changeTimer", &stateTimer);

		//チェックリスト
		for (int i = 0; i < checkMAX; i++)
		{
			ImGui::Checkbox(checkListString[i].c_str(), &isTutorialCheck[i]);
		}

		/*if (isShowFlag[0])
		{
			if (ImGui::Button(u8"表示切替"))
			{
				isShowFlag[0] = false;
			}
		}
		if (!isShowFlag[0])
		{
			if (ImGui::Button(u8"表示切替"))
			{
				isShowFlag[0] = true;
			}
		}
		if (ImGui::Button(u8"リセット"))
		{
			pos.x = 0;
			pos.y = 220;
			size.x = 500;
			size.y = 276;
			scale = 0.8f;
		}
		*/

		ImGui::SliderFloat("posX", &pos.x, 0, 1280);
		ImGui::SliderFloat("posY", &pos.y, 0, 720);
		ImGui::SliderFloat("sizeX", &size.x, 0, 1280);
		ImGui::SliderFloat("sizeY", &size.y, 0, 720);
		ImGui::SliderFloat("scale", &scale, 0, 2.0f);

	}

	ImGui::Separator();
	ImGui::End();

}


//チュートリアル関連
void SceneTutorial::TutorialUpdate(float elapsedTime)
{
	switch (tutorialState)
	{
	case TutorialState::STATE_ATTACK:
		TutorialAttackUpdate(elapsedTime);
		break;

	case TutorialState::STATE_OBJ_SETTING:
		TutorialObjSettingUpdate(elapsedTime);
		break;

	case TutorialState::STATE_RLUE_CHECK:
		TutorialRuleCheckUpdate(elapsedTime);
		break;
	}
}

void SceneTutorial::TutorialAttackUpdate(float elapsedTime)
{
	//移動を検知
	{
		DirectX::XMFLOAT3 velocity = player->GetVelocity();
		if (velocity.x != 0 || velocity.z != 0)
		{
			//強調表示
			isHighlightFlag[Tutorial01::MOVE] = true;
		}
		else
		{
			//強調表示
			isHighlightFlag[Tutorial01::MOVE] = false;
		}
	}

	//カメラ操作を検知
	{
		GamePad& gamePad = Input::Instance().GetGamePad();
		float ax = gamePad.GetAxisRX();
		float ay = gamePad.GetAxisRY();
		if (ax != 0 || ay != 0)
		{
			//強調表示
			isHighlightFlag[Tutorial01::CAMERA] = true;
		}
		else
		{
			//強調表示
			isHighlightFlag[Tutorial01::CAMERA] = false;
		}
	}

	//攻撃を検知
	{
		if (player->InputAttack())
		{
			//フラグをオンにする
			isTutorialCheck[CheckList::CHECK_ATTACK] = true;
			//強調表示
			highlightTimer[Tutorial01::ATTACK] = 0.5f;
			//isHighlightFlag[Tutorial01::ATTACK] = true;
		}
		highlightTimer[Tutorial01::ATTACK] -= elapsedTime;
		if (highlightTimer[Tutorial01::ATTACK] > 0)
		{
			isHighlightFlag[Tutorial01::ATTACK] = true;
		}
		else
		{
			//強調表示
			isHighlightFlag[Tutorial01::ATTACK] = false;
		}
	}

	//ジャンプを検知
	{
		if (player->jumpCount |= 0)
		{
			//フラグをオンにする
			isTutorialCheck[CheckList::CHECK_JUMP] = true;
			//強調表示
			isHighlightFlag[Tutorial01::JUMP] = true;
		}
		else
		{
			//強調表示
			isHighlightFlag[Tutorial01::JUMP] = false;
		}
	}

	//チェックできた回数をカウント
	checkCount = isTutorialCheck[CheckList::CHECK_ATTACK] + isTutorialCheck[CheckList::CHECK_JUMP];

	//全ての検知が完了したら
	if (checkCount == 2)
	{
		stateTimer -= elapsedTime;
	}
	//一定時間後にステートを更新
	if (stateTimer < 0)
	{
		//ステートを更新
		tutorialState = TutorialState::STATE_OBJ_SETTING;
		//表示するUIを変更
		isShowFlag[0] = !isShowFlag[0];
		isShowFlag[1] = !isShowFlag[1];
		//初期化
		stateTimer = initTimer;
		for (int i = 0; i < 4; i++)
		{
			isHighlightFlag[i] = false;
			highlightTimer[i] = -1.0f;
		}
		//トラップ一度削除
		TrapManager::Instance().Clear();
	}
}

void SceneTutorial::TutorialObjSettingUpdate(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	//オブジェクト選択を検知
	{
		if (gamePad.GetButtonDown() & GamePad::BTN_LEFT)
		{
			//フラグをオンにする
			isTutorialCheck[CheckList::CHECK_OBJ_SELECT] = true;
			highlightTimer[Tutorial02::OBJ_SELECT] = 0.5f;
		}
		if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)
		{
			//フラグをオンにする
			isTutorialCheck[CheckList::CHECK_OBJ_SELECT] = true;
			highlightTimer[Tutorial02::OBJ_SELECT] = 0.5f;
		}
		//一定時間ハイライト
		highlightTimer[Tutorial02::OBJ_SELECT] -= elapsedTime;
		if (highlightTimer[Tutorial02::OBJ_SELECT] > 0)
		{
			isHighlightFlag[Tutorial02::OBJ_SELECT] = true;
		}
		else
		{
			//強調表示
			isHighlightFlag[Tutorial02::OBJ_SELECT] = false;
		}
	}

	//セットモード検知
	{
		if (TrapManager::Instance().GetBuidFlag())
		{
			//フラグをオンにする
			isTutorialCheck[CheckList::CHECK_OBJ_SET_MODE] = true;
			highlightTimer[Tutorial02::OBJ_SET_MODE] = 0.5f;
			isSetMode = true;
		}
		else
		{
			isSetMode = false;
		}

		//一定時間ハイライト
		highlightTimer[Tutorial02::OBJ_SET_MODE] -= elapsedTime;
		if (highlightTimer[Tutorial02::OBJ_SET_MODE] > 0)
		{
			isHighlightFlag[Tutorial02::OBJ_SET_MODE] = true;
		}
		else
		{
			//強調表示
			isHighlightFlag[Tutorial02::OBJ_SET_MODE] = false;
		}
	}

	//設置を検知
	{
		if (isSetMode)
		{
			if (gamePad.GetButtonDown() & GamePad::BTN_UP)
			{
				if (TrapManager::Instance().GetCanSetFlag())
				{
					//フラグをオンにする
					isTutorialCheck[CheckList::CHECK_OBJ_SET] = true;
					highlightTimer[Tutorial02::OBJ_SET] = 0.5f;
				}
			}
		}
		//一定時間ハイライト
		highlightTimer[Tutorial02::OBJ_SET] -= elapsedTime;
		if (highlightTimer[Tutorial02::OBJ_SET] > 0)
		{
			isHighlightFlag[Tutorial02::OBJ_SET] = true;
		}
		else
		{
			//強調表示
			isHighlightFlag[Tutorial02::OBJ_SET] = false;
		}
	}

	//チェックした数をカウント
	checkCount = isTutorialCheck[CheckList::CHECK_OBJ_SELECT] +
		isTutorialCheck[CheckList::CHECK_OBJ_SET_MODE] +
		isTutorialCheck[CheckList::CHECK_OBJ_SET];

	//全て検知をしたら次のステートへ遷移
	if (checkCount == 3)
	{
		stateTimer -= elapsedTime;
	}

	//一定時間後にステートを更新
	if (stateTimer < 0)
	{
		//ステートを更新
		tutorialState = TutorialState::STATE_RLUE_CHECK;

		//表示するUIを変更
		isShowFlag[0] = false;
		isShowFlag[1] = false;
		//初期化
		stateTimer = 5.0f;
		for (int i = 0; i < 4; i++)
		{
			isHighlightFlag[i] = false;
			highlightTimer[i] = -1.0f;
		}
		//トラップ一度削除
		TrapManager::Instance().Clear();
	}
}
void SceneTutorial::TutorialRuleCheckUpdate(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	//タイトルへ戻る
	if (gamePad.GetButtonDown() & GamePad::BTN_START)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
	if (gamePad.GetButtonDown() & GamePad::BTN_X)
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
}

//UI変更処理
void SceneTutorial::ChangeUi()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetControllerState() == true)
	{
		keyOrButton = Operations::GAMEPAD;
	}
	else
	{
		keyOrButton = Operations::KEYBOARD;
	}
}