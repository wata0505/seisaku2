#include "shader.h"
#include "SceneTitle.h"
#include "Camera.h"
//#include "StageMain.h"
#include "StageManager.h"
#include "EnemyManager.h"
#include "Input.h"
#include "SwordTrailShader.h"
#include "EffectManager.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneLoading.h"
#include "StageObj.h"
#include "LightManager.h"
#include "SceneTutorial.h"
#include "Audio.h"
#include "AudioAll.h"
void SceneTitle::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	device = graphics.GetDevice();
	effectTexAll = new EffectTexAll();
	
	// エネミー初期化
	// スライム（ステートマシン用）
	//titlebgm = Audio::Instance().LoadAudioSource("resources\\Audio\\titlie.wav");
	

	StageManager& stageManager = StageManager::Instance();
	stageMain = new StageMain;
	stageManager.Register(stageMain);
	DirectX::XMFLOAT2 dir = {0,0};
	DirectX::XMFLOAT3 pos = {0,0,0};
	float angle = 0;
	//for (int i = 0; i < 10; i++)
	//{
	//
	//	angle = DirectX::XMConvertToRadians(36 * i);
	//	dir.x = sinf(angle);
	//	dir.y = cosf(angle);
	//	pos.x = dir.x * 80;
	//	pos.y = -1.5;
	//	pos.z = dir.y * 80;
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
		1000.0f
	);
	//カメラコントローラー初期化
	cameraController = std::make_unique <CameraController>();
	cameraController->SetRangeMax(35);
	cameraController->SetRange(35);
	//ピクセルシェーダーオブジェクト
	create_ps_from_cso(device.Get(), "Shader\\PostEffectPS.cso", pixel_shaders[0].GetAddressOf());
	create_ps_from_cso(device.Get(), "Shader\\BlurPS.cso", pixel_shaders[1].GetAddressOf());
	create_ps_from_cso(device.Get(), "Shader\\EffectPS.cso", pixel_shaders[2].GetAddressOf());
	luminanceExtractionData.threshold = 0.3;
	luminanceExtractionData.intensity = 5;
	bit_block_transfer = std::make_unique<FullscreenQuad>(device.Get());

	spriteBatchs[0] = std::make_unique<Sprite>(L".\\resources\\haikei\\6.png");
	//SkinnedMeshes[0] = std::make_unique<SkinnedMesh>(device.Get(), ".\\resources\\nico.fbx",0);
	framebuffers[0] = std::make_unique<Framebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	//framebuffers[1] = std::make_unique<Framebuffer>(device.Get(), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	subframebuffers[0] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	subframebuffers[1] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	subframebuffers[2] = std::make_unique<SubFramebuffer>(device.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	shadowbuffer = std::make_unique<Shadowbuffer>(device.Get(), ShadowMapSize, ShadowMapSize);
	spriteBatchs[1] = std::make_unique<Sprite>(L".\\resources\\UI\\Change Braver.png");
	spriteBatchs[2] = std::make_unique<Sprite>(L".\\resources\\UI\\gamestart.png");
	spriteBatchs[3] = std::make_unique<Sprite>();
	spriteBatchs[4] = EffectTexAll::Instance().GetSprite(int(EffectTexAll::EfTexAll::Bock));
	spriteBatchs[5] = std::make_unique<Sprite>(L".\\resources\\UI\\start the story.png");
	spriteBatchs[6] = std::make_unique<Sprite>(L".\\resources\\UI\\tutorial.png");
	renderSprite = std::make_unique<Sprite>();

	effectAll = new EffectAll();
	audioAll = new AudioAll();
}
void SceneTitle::Finalize()
{

	StageManager::Instance().clear();
	LightManager::Instance().Clear();

}
void SceneTitle::Update(float elapsedTime)
{
	//titlebgm->Play(true);
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Title)->Play(true);
	GamePad& gamePad = Input::Instance().GetGamePad();
	const GamePadButton anyButton =
		GamePad::BTN_A
		| GamePad::BTN_B
		| GamePad::BTN_X
		| GamePad::BTN_Y
		;
	
	cameraController->SetTarget(position);
	cameraController->TitleUpdate(elapsedTime);
	StageManager::Instance().Update(elapsedTime);

	

	EffectManager::instance().Update(elapsedTime);
	
	if (gamePad.GetButtonDown() & anyButton) {
		cameraController->SetRangeMax(25);
		if (titleMode) {
			switch (gameMode)
			{
			case (int)GamaMode::Default:
				break;
			case (int)GamaMode::Tutorial:

			default:
				break;
			}
		}
		AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Ketei)->Play(false);
		titleMode++;
	}
	if (dissolveDTimer > 1) {
		AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Title)->Stop();
		switch (gameMode) {
		case (int)GamaMode::Default:
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
			break;
		case (int)GamaMode::Tutorial:
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTutorial));
			break;
		}
	}
	timer++;
	if (titleMode > 0) {
		dissolveTimer += elapsedTime;
		if (gamePad.GetButtonDown() & GamePad::BTN_DOWN) {
			gameMode++;
			if (gameMode > (int)GamaMode::Tutorial) gameMode = (int)GamaMode::Default;
		}
		if (gamePad.GetButtonDown() & GamePad::BTN_UP) {
			gameMode--;
			if (gameMode < (int)GamaMode::Default) gameMode = (int)GamaMode::Tutorial;
		}
	}
	if (titleMode > 1) {
		dissolveDTimer += elapsedTime * 0.5;
	}
	haikeiTimer += elapsedTime;
}
void SceneTitle::Render()
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
		DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat4(&shadowlight_direction);
		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
		LightPosition = DirectX::XMVectorScale(LightPosition, light);
		DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(Position - LightPosition,
			Position,
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		// シャドウマップに描画したい範囲の射影行列を生成
		DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f,
			500.0f);
		DirectX::XMStoreFloat4x4(&rc.view, V);
		DirectX::XMStoreFloat4x4(&rc.projection, P);
		DirectX::XMStoreFloat4x4(&lightViewProjection, V * P);
	}
	ModelShader* shader = graphics.GetShader(Graphics::ModelShaderId::ShadowmapCaster);
	shader->Begin(immediate_context, rc);
	StageManager::Instance().Render(immediate_context, shader);
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
	//rc.viewPosition = camera_position;
	rc.deviceContext = immediate_context;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	rc.lightDirection = light_direction;
	////immediate_context->PSSetShaderResources(10, 1, shadowbuffer->SrvShadow.GetAddressOf());
	//デバック関係
	//projectImgui();
	//EnemyManager::Instance().DrawDebugGUI();
	rc.shadowMapData.shadowMap = shadowbuffer->SrvShadow.Get();
	rc.shadowMapData.lightViewProjection = lightViewProjection;

	SpriteShader* shader2 = graphics.GetShader(Graphics::SpriteShaderId::Skybox);

	shader2->Begin(rc);
	spriteBatchs[0]->Render(immediate_context,
		0, 0, 1280, 720,
		0, 0, spriteBatchs[0]->GetTextureWidth(), spriteBatchs[0]->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0,haikeiTimer
	);
	//spriteBatchs[0]->SetShaderResourceView(framebuffers[0]->shaderResourceViews[2], 1280, 720);
	shader2->Draw(rc, spriteBatchs[0].get());
	shader2->End(rc);
	//デフォルトモデルシェーダー
	shader = graphics.GetShader(Graphics::ModelShaderId::ModelShaderDefault);

	shader->Begin(immediate_context, rc);
	StageManager::Instance().Render(immediate_context, shader);
	StageManager::Instance().InstaningRender(immediate_context, shader);
	shader->End(immediate_context);
	graphics.GetDebugRenderer()->Render(immediate_context, rc.view, rc.projection);

	framebuffers[0]->EffectActivate(immediate_context);
	EffectManager::instance().Render(rc.view, rc.projection);
	
	//デフォルトスプライトシェーダー
	shader2 = graphics.GetShader(Graphics::SpriteShaderId::SpriteShaderDefault);
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
	//サブオフスクリーンバッファ（ブルーム）
	subframebuffers[1]->Clear(immediate_context);
	subframebuffers[1]->Activate(immediate_context);
	bit_block_transfer->blit(immediate_context, subframebuffers[0]->shaderResourceViews.GetAddressOf(), 0, 1, pixel_shaders[1].Get());
	subframebuffers[1]->Deactivate(immediate_context);
	ID3D11ShaderResourceView* shaderResourceViews[2] = { framebuffers[0]->shaderResourceViews[5].Get(),subframebuffers[1]->shaderResourceViews.Get() };
	subframebuffers[1]->Clear(immediate_context);
	subframebuffers[1]->Activate(immediate_context);
	//ID3D11ShaderResourceView* shaderResourceViews[2]{ framebuffers[0]->shaderResourceViews[5].Get(),framebuffers[1]->shaderResourceViews[0].Get() };
	//（合成）
	bit_block_transfer->blit(immediate_context, shaderResourceViews, 0, 2, pixel_shaders[0].Get());
	subframebuffers[1]->Deactivate(immediate_context);
	//画面をスプライトにいれる
	spriteBatchs[3]->SetShaderResourceView(subframebuffers[1]->shaderResourceViews.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	
	//２D描画
	shader2 = graphics.GetShader(Graphics::SpriteShaderId::SpriteShaderDefault);
	shader2->Begin(rc);
	spriteBatchs[4]->Render(immediate_context,
		0, 0, 1280, 720,
		0, 0, spriteBatchs[4]->GetTextureWidth(), spriteBatchs[4]->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 0
	);
	shader2->Draw(rc, spriteBatchs[4].get());
	spriteBatchs[3]->Render(immediate_context,
		0, 0, 1280, 720,
		0, 0, spriteBatchs[3]->GetTextureWidth(), spriteBatchs[3]->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0,0,dissolveDTimer
	);
	shader2->Draw(rc, spriteBatchs[3].get());
	spriteBatchs[1]->Render(immediate_context,
		0, 0, 1280, 720,
		0, 0, spriteBatchs[1]->GetTextureWidth(), spriteBatchs[1]->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, dissolveTimer
	);
	spriteBatchs[2]->Render(immediate_context,
		200, 520, 1000, 200,
		0, 0, spriteBatchs[2]->GetTextureWidth(), spriteBatchs[2]->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,dissolveTimer
	);
	
    shader2->Draw(rc, spriteBatchs[1].get());
    if (timer / 20 % 2) {
    	shader2->Draw(rc, spriteBatchs[2].get());
    }
	DirectX::XMFLOAT2 gauge;
	gauge.x = 500 * (1.0 - gameMode * 0.5);
	gauge.y = 100 * (1.0 - gameMode * 0.5);
	spriteBatchs[5]->Render(immediate_context,
		1000 - gauge.x * 0.5, 200 - gauge.y *0.5, gauge.x, gauge.y,
		0, 0, spriteBatchs[5]->GetTextureWidth(), spriteBatchs[5]->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f, 
		dissolveDTimer*3,0,0
	);
	gauge.x = 500 * (0.5 + gameMode * 0.5);
	gauge.y = 100 * (0.5 + gameMode * 0.5);
	spriteBatchs[6]->Render(immediate_context,
		1000 - gauge.x * 0.5, 400 - gauge.y * 0.5, gauge.x, gauge.y,
		0, 0, spriteBatchs[6]->GetTextureWidth(), spriteBatchs[6]->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		dissolveDTimer*3,0,0
	);
	if (titleMode) {
		shader2->Draw(rc, spriteBatchs[5].get());
		shader2->Draw(rc, spriteBatchs[6].get());
	}
	shader2->End(rc);

}


void SceneTitle::projectImgui()
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
		ImGui::SliderFloat("DrawRect", &shadowDrawRect, 1.0f, 1150.0f);
		ImGui::SliderFloat("leght", &light, 1.0f, 1150.0f);
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
		ImGui::Image(framebuffers[0]->shaderResourceViews[0].Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[1].Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[2].Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[3].Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[4].Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::Image(framebuffers[0]->shaderResourceViews[5].Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		//ImGui::Image(framebuffers[1]->shaderResourceViews[0].Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::End();

}