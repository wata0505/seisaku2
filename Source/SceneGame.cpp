#include "shader.h"
#include"SceneGame.h"
#include "Camera.h"
//#include "StageMain.h"
#include "StageManager.h"
#include "EnemyManager.h"
#include "Input.h"
#include "SwordTrailShader.h"
#include "EffectManager.h"
#include "EnemyBag.h"
#include "ParticleManager.h"
#include "ParticleSpriteManager.h"
#include "InstancingSpriteManager.h"
#include "StageObj.h"
#include "LightManager.h"
#include "AudioAll.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "UIManager.h"
#include "LockOnUI.h"
#include "BaseUI.h"
#include "BreakUI.h"
#include "ParticleSystem.h"
#include "EnemySystem.h"

#include "TrapManager.h"
#include "Turret.h"
#include "Misc.h"

void SceneGame::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	device = graphics.GetDevice();

	EnemyManager& enemyManager = EnemyManager::Instance();

	EnemySystem& enemySystem = EnemySystem::Instance();
	enemySystem.Start(SceneManager::Instance().GetStage());
	StageManager& stageManager = StageManager::Instance();
	stageManager.Clear();
	stageMain = new StageMain;
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
	
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 0, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		500.0f
	);
	//カメラコントローラー初期化
	//cameraController = std::make_unique <CameraController>();
	float gaugeWidth = 500.0f;
	float gaugeHeight = 500.0f;
	BaseUI* hpWaku = new BaseUI(L".\\resources\\kaba2.png", 10.0f, 395.0f, gaugeWidth * 0.9f, gaugeHeight * 0.9f);
	DirectX::XMFLOAT3 pos = { 100.0f, -2.5f, -139.975f };
	player = std::make_unique<Player>();
	base = std::make_unique<Base>(pos);
	//ピクセルシェーダーオブジェクト
	create_ps_from_cso(device.Get(), "Shader\\PostEffectPS.cso", pixel_shaders[0].GetAddressOf());
	create_ps_from_cso(device.Get(), "Shader\\BlurPS.cso", pixel_shaders[1].GetAddressOf());
	create_ps_from_cso(device.Get(), "Shader\\EffectPS.cso", pixel_shaders[2].GetAddressOf());


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
	create_ps_from_cso(device.Get(), "Shader\\JitterDriftPS.cso", jitterDriftPixelShader.GetAddressOf());

	bit_block_transfer = std::make_unique<FullscreenQuad>(device.Get());


	sprite_batchs2 = std::make_unique<Sprite>();
	waveSprite[0] = std::make_unique<Sprite>(L".\\resources\\UI\\1Wave.png");
	waveSprite[1] = std::make_unique<Sprite>(L".\\resources\\UI\\2Wave.png");
	waveSprite[2] = std::make_unique<Sprite>(L".\\resources\\UI\\FinalWave.png");
	waveBackSprite = std::make_unique<Sprite>(L".\\resources\\UI\\Title\\StageBack.png");
	pixelSprite = std::make_unique<Sprite>(L".\\resources\\UI\\Pixel.png");
	gameOverSprite = std::make_unique<Sprite>(L".\\resources\\UI\\GameOver.png");
	gameClearSprite = std::make_unique<Sprite>(L".\\resources\\UI\\GameClear.png");
	loodSprite = EffectTexAll::Instance().GetSprite(int(EffectTexAll::EfTexAll::Bock));
	renderSprite = std::make_unique<Sprite>();
	//スクリーンバファ生成
	framebuffers[0] = std::make_unique<Framebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
    subframebuffers[0] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH , SCREEN_HEIGHT);
	subframebuffers[1] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH/2, SCREEN_HEIGHT /2);
	subframebuffers[2] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH /4, SCREEN_HEIGHT /4);
	subframebuffers[3] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	subframebuffers[4] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	subframebuffers[5] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	jitterDriftSubFramebuffer = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	shadowbuffer = std::make_unique<Shadowbuffer>(device.Get(), ShadowMapSize, ShadowMapSize);
	luminanceExtractionData.threshold = 0.03f;
	luminanceExtractionData.intensity = 3.0f;
	
	meta = std::make_unique<Meta>(player.get(), &enemyManager);
	UIManager& uiManager = UIManager::Instance();
	
	//BaseUI* mpWaku    = new BaseUI(L".\\resources\\Mpwaku3.png", 10, 650, gaugeWidth * 1.1, gaugeHeight * 0.5);
	gaugeWidth = 65.0f;
	gaugeHeight = 100.0f;
	//BaseUI* mainSword = new BaseUI(L".\\resources\\UI\\SW.PNG", 100, 300, gaugeWidth, gaugeHeight);
	//BaseUI* axe       = new BaseUI(L".\\resources\\UI\\Axe2.PNG",200, 400, gaugeWidth, gaugeHeight);
	//BaseUI* halberd   = new BaseUI(L".\\resources\\UI\\HB.PNG", 0, 400, gaugeWidth, gaugeHeight);
	//BaseUI* subSword  = new BaseUI(L".\\resources\\UI\\EX.PNG", 100, 500, gaugeWidth, gaugeHeight);
	//BaseUI* zyuuzi = new BaseUI(L".\\resources\\UI\\zyuuzi.PNG", 110, 425,50, 50);
	LockOnUI* lock    = new LockOnUI(L".\\resources\\Lockon.png");
	//BreakUI* breakWepon = new BreakUI(L".\\resources\\UI\\break.png", 100, 300, gaugeWidth, gaugeHeight);
	//BaseUI* lt = new BaseUI(L".\\resources\\UI\\LT.PNG", 0, 200, 200, 50);
	//BaseUI* lb = new BaseUI(L".\\resources\\UI\\LB.PNG", 0, 250, 250, 50);

	//ParticleSprite* particleSprite = new ParticleSprite({NULL,NULL,NULL}, { NULL,NULL,NULL }, ParticleSprite::ParticleSoft, ParticleSprite::Chile, (int)EffectTexAll::EfTexAll::Thunder, 20000,NULL, NULL, false);
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Bgm)->Play(true);


}
void SceneGame::Finalize()
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
void SceneGame::Update(float elapsedTime)
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
	EnemyManager::Instance().DrawDebugPrimitive();
		
	EffectManager::instance().Update(deltaTimer);

	ParticleManager::Instance().Update(deltaTimer);

	InstancingSpriteManager::Instance().Update(deltaTimer);

	ParticleSpriteManager::Instance().Update(deltaTimer);

	LightManager::Instance().Update(deltaTimer);

	UIManager::Instance().Update(deltaTimer);

	TrapManager::Instance().Update(deltaTimer);
	TrapManager::Instance().DrawDebugPrimitive();
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
	
#endif
	if (base->GetHP() != 0 && player->GetHealth() > 0 && EnemySystem::Instance().GetWave() < 3) 
	{
		dissolveTimer += elapsedTime;
		if (dissolveTimer > 3.0f)
		{
			dissolveTimer = 3.0f;
		}

		jitterDriftData.jitterStrength = skyboxColor / 200.0f;		
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
		else
		{
			jitterDriftData.jitterStrength = 0.0f;
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
	gameTimer++;
}
void SceneGame::Render()
{	
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
	render_target_view = graphics.GetRenderTargetView();
	sampler_states[0] = graphics.GetSamplerState(0);
	sampler_states[1] = graphics.GetSamplerState(1);
	sampler_states[2] = graphics.GetSamplerState(2);
	//->AddRef()
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


	//ポイントフィルタをｓ０
	immediate_context->PSSetSamplers(0, 1, sampler_states[0].GetAddressOf());
	//リニアフィルタを s1
	immediate_context->PSSetSamplers(1, 1, sampler_states[1].GetAddressOf());
	//アニソトロピックを　s2
	immediate_context->PSSetSamplers(2, 1, sampler_states[2].GetAddressOf());
	//深度テストをOFF深度書き込みOFF

	RenderContext rc;
	//シャドウバッファ
	shadowbuffer->clear(immediate_context);
	shadowbuffer->activate(immediate_context);
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
		DirectX::XMStoreFloat4x4(&lightViewProjection, V * P);
	}
	ModelShader* shader = graphics.GetShader(Graphics::ModelShaderId::ShadowmapCaster);
	shader->Begin(immediate_context, rc);
	StageManager::Instance().Render(immediate_context, shader);
	StageManager::Instance().InstaningRender(immediate_context,shader);
	if (player->GetLoseDirectingTimer() <= 0.0f || player->GetClearDirectingTimer() > 0.0f)
	{
		player->render(immediate_context, shader);
	}
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
	
	rc.shadowMapData.shadowMap = shadowbuffer->SrvShadow.Get();
	rc.shadowMapData.lightViewProjection = lightViewProjection;

	SpriteShader* shader2 = graphics.GetShader(Graphics::SpriteShaderId::Skybox);

	shader2->Begin(rc);
	loodSprite->Render(immediate_context, skyboxColor, haikeiTimer);
	//sprite_batchs[0]->SetShaderResourceView(framebuffers[0]->shaderResourceViews[2], 1280, 720);
	shader2->Draw(rc, loodSprite.get());
	shader2->End(rc);

	//デフォルトシェーダー
	shader = graphics.GetShader(Graphics::ModelShaderId::ModelShaderDefault);
	
	shader->Begin(immediate_context, rc);
	base->Render(immediate_context, shader);
	StageManager::Instance().Render(immediate_context, shader);
	StageManager::Instance().InstaningRender(immediate_context, shader);
	if (player->GetLoseDirectingTimer() <= 0.0f || player->GetClearDirectingTimer() > 0.0f)
	{
		player->render(immediate_context, shader);
	}
	if (player->GetLoseDirectingTimer() <= 0.0f)
	{
		EnemyManager::Instance().Render(immediate_context, shader);
		TrapManager::Instance().Render(immediate_context, shader);
		TrapManager::Instance().InstancingRender(immediate_context, shader);
	}
	
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
	rc.luminanceExtractionData = luminanceExtractionData;
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
	ID3D11ShaderResourceView* shaderResourceViews[2] = { framebuffers[0]->shaderResourceViews[8].Get(),subframebuffers[2]->shaderResourceViews.Get()};
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
	//else
	//{
	//	bit_block_transfer->blit(immediate_context, subframebuffers[3]->shaderResourceViews.GetAddressOf(), 0, 1);
	//}

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
		UIManager::Instance().Render(rc, shader2);
		TrapManager::Instance().Sprite2DRender(immediate_context, rc, shader2);
		base->HpDisplay(rc, shader2);
		int wave = EnemySystem::Instance().GetWave();
		if (wave < 3)
		{
			float waveCountWidth = static_cast<float>(waveSprite[wave]->GetTextureWidth());
			float waveCountHeight = static_cast<float>(waveSprite[wave]->GetTextureHeight());
			waveSprite[wave]->Render(immediate_context,
				screenWidth * 0.5f - waveCountWidth * 0.5f, screenHeight * 0.1f, waveCountWidth, waveCountHeight,
				0.0f, 0.0f, waveCountWidth, waveCountHeight,
				0.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				waveTimer
			);
			shader2->Draw(rc, waveSprite[wave].get());
			float waveBackWidth = static_cast<float>(waveBackSprite->GetTextureWidth());
			float waveBackHeight = static_cast<float>(waveBackSprite->GetTextureHeight());
			waveBackSprite->Render(immediate_context,
				screenWidth * 0.5f - waveCountWidth * 0.5f, screenHeight * 0.1f, waveCountWidth, waveCountHeight,
				0.0f, 0.0f, waveBackWidth, waveBackHeight,
				0.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				waveTimer
			);
			shader2->Draw(rc, waveBackSprite.get());
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
	shader2->End(rc);
	//if (base->GetHP() == 0 || EnemySystem::Instance().GetWave() < 4) 
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
#if 0
	if (player->GetDidTimer() < NULL) 
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
#endif
}
void SceneGame::projectImgui()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_None))
	{
		ImGui::SliderFloat("Sky", &skyboxColor, 0.0f, 1.0f);
		ImGui::SliderFloat("GameAfterDissolveTimer", &gameAfterDissolveTimer, 0.0f, 3.0f);
	}
	EnemyManager::Instance().DrawDebugGUI();
	TrapManager::Instance().DrawDebugGUI();
	if (ImGui::TreeNode("light"))
	{
		ImGui::SliderFloat("light_dir.x", &light_direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("light_dir.y", &light_direction.y, -1.0f, 1.0f);
		ImGui::SliderFloat("light_dir.z", &light_direction.z, -1.0f, 1.0f);
		ImGui::SliderFloat("light_dir.w", &light_direction.w, -1000.0f, 1000.0f);
		ImGui::TreePop();
	
	}
	if (ImGui::TreeNode("Shadowmap"))
	{
		ImGui::SliderFloat("DrawRect", &shadowDrawRect,1.0f, 150.0f);
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
	if (ImGui::TreeNode("Bloom"))
	{
		ImGui::SliderFloat("threshold", &luminanceExtractionData.threshold, 0.0f, 1.0f);
		ImGui::SliderFloat("intensity", &luminanceExtractionData.intensity, 0.0f, 10.0f);
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
	ImGui::Separator();
	ImGui::End();

}