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

void SceneGame::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	device = graphics.GetDevice();

	EnemyManager& enemyManager = EnemyManager::Instance();
	// エネミー初期化
	// ドラゴン（
	//EnemyDragon* dragon = new EnemyDragon();
	//dragon->SetPosition(DirectX::XMFLOAT3(0.0f, 20.0f, 25.0f));
	//dragon->SetTerritory(dragon->GetPosition(), 20.0f);
	//dragon->SetId(0);
	//enemyManager.Register(dragon);
	// バグ
	//for(int i = 0; i < 20; i++) {
	//	EnemyBag* bag = new EnemyBag();
	//	float yaw = DirectX::XMConvertToRadians(rand() % 360);
	//	DirectX::XMFLOAT2 dir;
	//	dir.x = sinf(yaw);
	//	dir.y = cosf(yaw);
	//	int len = rand() % 100 + 5;
	//	bag->SetPosition(DirectX::XMFLOAT3(dir.x * len, 0.0f, dir.y * len));
	//	bag->SetTerritory(bag->GetPosition(), 10.0f);
	//	//bag->SetId(0);
	//	enemyManager.Register(bag);
	//	//if (i % 2 == 0) {
	//	//	bag->SetBarrierFlag(true);
	//	//	ParticleSystem::Instance().BarrierEnemyEffect(bag->GetId(), (int)EffectTexAll::EfTexAll::BarrierEnemy, { 1,0,0,1 });
	//	//}
	//}
	//
	//
	//enemyManager.SetEnemyMaxCount(enemyManager.GetEnemyCount());

	EnemySystem& enemySystem = EnemySystem::Instance();
	enemySystem.Start();
	StageManager& stageManager = StageManager::Instance();
	stageManager.clear();
	stageMain = new StageMain;
	stageManager.Register(stageMain);

	DirectX::XMFLOAT2 dir = { 0,0 };
	DirectX::XMFLOAT3 pos = { 0,0,10 };
	float angle = 0;
	//for (int i = 0; i < 10; i++)
	//{
	//
	//	angle = DirectX::XMConvertToRadians(36 * i);
	//	dir.x = sinf(angle);
	//	dir.y = cosf(angle);
	//	pos.x = dir.x * 70;
	//	pos.y = -1.5;
	//	pos.z = dir.y * 70;
	//	StageObj* stageObj = new StageObj(pos);
	//	//stageObj->SetPosition(pos);
	//	stageManager.Register(stageObj);
	//}
	
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


	sprite_batchs = std::make_unique<Sprite>(L".\\resources\\haikei\\6.png");
	sprite_batchs2 = std::make_unique<Sprite>();
	claerSprite = std::make_unique<Sprite>(L".\\resources\\UI\\clear.png");
	loodSprite = EffectTexAll::Instance().GetSprite(int(EffectTexAll::EfTexAll::Bock));
	renderSprite = std::make_unique<Sprite>();
	//スクリーンバファ生成
	framebuffers[0] = std::make_unique<Framebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
    subframebuffers[0] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH /2, SCREEN_HEIGHT /2);
	subframebuffers[1] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH /4, SCREEN_HEIGHT /4);
	subframebuffers[2] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH /8, SCREEN_HEIGHT /8);
	subframebuffers[3] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	subframebuffers[4] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	subframebuffers[5] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	jitterDriftSubFramebuffer = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	shadowbuffer = std::make_unique<Shadowbuffer>(device.Get(), ShadowMapSize, ShadowMapSize);
	luminanceExtractionData.threshold = 0.3;
	luminanceExtractionData.intensity = 5;
	
	meta = std::make_unique<Meta>(player.get(), &enemyManager);
	UIManager& uiManager = UIManager::Instance();
	float gaugeWidth = 500.0f;
	float gaugeHeight = 50.0f;
	BaseUI* hpWaku    = new BaseUI(L".\\resources\\kaba.png", 10, 595, gaugeWidth * 1.1, gaugeHeight * 1.1);
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

	StageManager::Instance().clear();
	EnemyManager::Instance().Clear();
	ParticleManager::Instance().Clear();
	InstancingSpriteManager::Instance().Clear();
	ParticleSpriteManager::Instance().Clear();
	LightManager::Instance().Clear();
	UIManager::Instance().Clear();
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Bgm)->Stop();
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::BgmBoss)->Stop();
	
}
void SceneGame::Update(float elapsedTime)
{
	player->update(elapsedTime);
	base->Update(elapsedTime);
	StageManager::Instance().Update(elapsedTime);
	EnemySystem::Instance().Update(elapsedTime);
	EnemyManager::Instance().Update(elapsedTime);
	EnemyManager::Instance().DrawDebugPrimitive();
		
	EffectManager::instance().Update(elapsedTime);

	ParticleManager::Instance().Update(elapsedTime);

	InstancingSpriteManager::Instance().Update(elapsedTime);

	ParticleSpriteManager::Instance().Update(elapsedTime);

	LightManager::Instance().Update(elapsedTime);

	UIManager::Instance().Update(elapsedTime);

	
	if (base->GetHP() != 0 && player->GetHealth() > 0) {
		dissolveTimer += elapsedTime;
		if (dissolveTimer > 3) dissolveTimer = 3;
	}
	else
	{
		dissolveTimer -= elapsedTime;
	}
	haikeiTimer += elapsedTime;
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
		LightPosition = DirectX::XMVectorScale(LightPosition, 30.0f);
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
	player->render(immediate_context, shader);
	base->Render(immediate_context,shader);
	StageManager::Instance().Render(immediate_context, shader);
	EnemyManager::Instance().Render(immediate_context, shader);
	StageManager::Instance().InstaningRender(immediate_context,shader);
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
	sprite_batchs->Render(immediate_context,
		0, 0, 1280, 720,
		0, 0, sprite_batchs->GetTextureWidth(), sprite_batchs->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0, haikeiTimer,0
	);
	//sprite_batchs[0]->SetShaderResourceView(framebuffers[0]->shaderResourceViews[2], 1280, 720);
	shader2->Draw(rc, sprite_batchs.get());
	shader2->End(rc);

	//デフォルトシェーダー
	shader = graphics.GetShader(Graphics::ModelShaderId::ModelShaderDefault);
	shader->Begin(immediate_context, rc);
	StageManager::Instance().Render(immediate_context, shader);
	shader->Begin(immediate_context, rc);
	player->render(immediate_context, shader);
	base->Render(immediate_context, shader);
	EnemyManager::Instance().Render(immediate_context, shader);
	StageManager::Instance().InstaningRender(immediate_context, shader);
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
	//player->DrawDebugGUI();
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
	ID3D11ShaderResourceView* shaderResourceViews[2] = { framebuffers[0]->shaderResourceViews[5].Get(),subframebuffers[2]->shaderResourceViews.Get()};
	{
		bit_block_transfer->blit(immediate_context, shaderResourceViews, 0, 2, pixel_shaders[0].Get());
	
	}
	subframebuffers[3]->Deactivate(immediate_context);
	subframebuffers[4]->Clear(immediate_context);
	subframebuffers[4]->Activate(immediate_context);
	bit_block_transfer->blit(immediate_context, subframebuffers[3]->shaderResourceViews.GetAddressOf(), 0, 1);
	if (player->GetQuickflag()) {
		//放射ブラー
		bit_block_transfer->blit(immediate_context, subframebuffers[3]->shaderResourceViews.GetAddressOf(), 0, 1, pixel_shaders[2].Get());
	}
	else
	{
		bit_block_transfer->blit(immediate_context, subframebuffers[3]->shaderResourceViews.GetAddressOf(), 0, 1);
	}

	subframebuffers[4]->Deactivate(immediate_context);

	// ジッタードリフトシェーダー
	jitterDriftData.time += 1.0f / 60.0f;
	jitterDriftSubFramebuffer->Clear(immediate_context);
	jitterDriftSubFramebuffer->Activate(immediate_context);
	bit_block_transfer->JitterDrift(immediate_context, subframebuffers[4]->shaderResourceViews.GetAddressOf(), 13, 1, jitterDriftData, jitterDriftPixelShader.Get());
	jitterDriftSubFramebuffer->Deactivate(immediate_context);

	shader2 = graphics.GetShader(Graphics::SpriteShaderId::SpriteShaderDefault);
	shader2->Begin(rc);
	//sprite_batchs2->SetShaderResourceView(subframebuffers[4]->shaderResourceViews.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	sprite_batchs2->SetShaderResourceView(jitterDriftSubFramebuffer->shaderResourceViews.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	sprite_batchs2->Render(immediate_context,
		0, 0, 1280, 720,
		0, 0, sprite_batchs2->GetTextureWidth(), sprite_batchs2->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0, 0, 0
	);
	shader2->Draw(rc, sprite_batchs2.get());
	//player->Sprite2DRender(immediate_context, rc, shader2);
	UIManager::Instance().Render(rc,shader2);
	base->HpDisplay(rc, shader2);
	claerSprite->Render(immediate_context,
		200, 120, 1000, 200,
		0, 0, claerSprite->GetTextureWidth(), claerSprite->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		dissolveTimer
	);
	loodSprite->Render(immediate_context,
		0, 0, 1280, 720,
		0, 0, loodSprite->GetTextureWidth(), loodSprite->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		dissolveTimer,0,0
	);
	shader2->Draw(rc, loodSprite.get());
	if (base->GetHP() == 0) {
		
		shader2->Draw(rc, claerSprite.get());
		if (dissolveTimer < 0) {
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		}
	}
	shader2->End(rc);
	if (player->GetDidTimer() < NULL) {
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
}
void SceneGame::projectImgui()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_None))
	{
	}
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
		ImGui::Image(subframebuffers[5]->shaderResourceViews.Get(), { 128, 128 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[1].Get(), { 128, 128 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[2].Get(), { 128, 128 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[3].Get(), { 128, 128 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[4].Get(), { 128, 128 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[5].Get(), { 128, 128 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(subframebuffers[0]->shaderResourceViews.Get(), { 128, 128 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(subframebuffers[1]->shaderResourceViews.Get(), { 128, 128 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(subframebuffers[2]->shaderResourceViews.Get(), { 128, 128 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(subframebuffers[4]->shaderResourceViews.Get(), { 128, 128 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
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