#include "shader.h"
#include "SceneTitle.h"
#include "Camera.h"
#include "StageObj.h"
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
#include "EnemySystem.h"
#include "EnemyBag.h"
#include "Mathf.h"
#include "RenderContext.h"

// 初期化
void SceneTitle::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11Device* device = graphics.GetDevice();

	// エフェクトテクスチャ生成
	EffectTexAll* effectTexAll = new EffectTexAll();
	// エフェクト生成
	EffectAll* effectAll = new EffectAll();
	// オーディオ生成
	AudioAll* audioAll = new AudioAll();

	// エネミー初期化
#if 0
	EnemyManager& enemyManager = EnemyManager::Instance();
	for(int i = 0; i < 1; i++) 
	{
		EnemyBag* bag = new EnemyBag();
		//bag->SetPosition({ -i * 3.0f, 0.0f, -50.0f });
		bag->SetPosition({ -i * 3.0f, 2.0f, -5.0f });
		bag->SetTerritory(bag->GetPosition(), 10.0f);
		//bag->SetId(0);
		enemyManager.Register(bag);
	}
	meta = std::make_unique<Meta>(player.get(), &enemyManager);
#endif
	// ステージ生成
	{
		StageManager& stageManager = StageManager::Instance();
		// 低い位置を基準に
		DirectX::XMFLOAT3 position = { 0.0f, -10.0f, 0.0f };
		for (int index = -2; index < 2; index++)
		{
			// 原点よりマイナス位置->プラス位置に生成
			position.x = index * 20.0f;
			StageObj* stageObj = new StageObj(position);
			stageManager.Register(stageObj);
		}

		position.y = -0.275f;
		position.z -= 40.0f;
		tower = std::make_unique<Base>(position);
	}

	// プレイヤー初期化
	{
		player = std::make_unique<Player>();
		player->SetPosition({});	// 位置を原点に
	}

	// カメラ初期化
	{
		Camera& camera = Camera::Instance();
		camera.SetLookAt(DirectX::XMFLOAT3(0, 0, -10), {}, DirectX::XMFLOAT3(0, 1, 0));
		camera.SetPerspectiveFov(DirectX::XMConvertToRadians(45),
			graphics.GetScreenWidth() / graphics.GetScreenHeight(),
			0.1f, 1000.0f);
	}
	// カメラコントローラー初期化
	{
		cameraController = std::make_unique<CameraController>();
		cameraController->SetRangeMax(5);
		cameraController->SetRange(5);
		// カメラ位置設定
		DirectX::XMFLOAT3 position = player->GetPosition();
		position.x += 2.0f;
		position.y += 1.5f;
		cameraController->SetTarget(position);
	}

	// ピクセルシェーダーオブジェクト
	{
		create_ps_from_cso(device, "Shader\\PostEffectPS.cso", pixelShader[PixelShaderKind::PostEffect].GetAddressOf());
		create_ps_from_cso(device, "Shader\\BlurPS.cso", pixelShader[PixelShaderKind::GaussianBlur].GetAddressOf());
		create_ps_from_cso(device, "Shader\\EffectPS.cso", pixelShader[PixelShaderKind::RadiationBlur].GetAddressOf());
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
		create_ps_from_cso(device, "Shader\\JitterDriftPS.cso", pixelShader[PixelShaderKind::JitterDriftPS].GetAddressOf());
		bitBlockTransfer = std::make_unique<FullscreenQuad>(device);

		// 描画バッファ
		framebuffer = std::make_unique<Framebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
		subframebuffers[SubFrameBufferKind::Luminance] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
		subframebuffers[SubFrameBufferKind::Bloom] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		subframebuffers[SubFrameBufferKind::Synthesis] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
		subframebuffers[SubFrameBufferKind::JitterDriftSFB] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
		// シャドウバッファ
		static const int ShadowMapSize = 2096;
		shadowbuffer = std::make_unique<Shadowbuffer>(device, ShadowMapSize, ShadowMapSize);
		// テクスチャ
		spriteBatchs[SpriteKind::Skybox] = std::make_unique<Sprite>(L".\\resources\\haikei\\6.png");
		spriteBatchs[SpriteKind::Title] = std::make_unique<Sprite>(L".\\resources\\UI\\Title\\title.png");
		spriteBatchs[SpriteKind::DecisionText] = std::make_unique<Sprite>(L".\\resources\\UI\\gamestart.png");
		spriteBatchs[SpriteKind::Back] = EffectTexAll::Instance().GetSprite(int(EffectTexAll::EfTexAll::Bock));
		spriteBatchs[SpriteKind::GameStartText] = std::make_unique<Sprite>(L".\\resources\\UI\\start the story.png");
		spriteBatchs[SpriteKind::TutorialText] = std::make_unique<Sprite>(L".\\resources\\UI\\tutorial.png");
		spriteBatchs[SpriteKind::Stage1] = std::make_unique<Sprite>(L".\\resources\\UI\\stage1.png");
		spriteBatchs[SpriteKind::Stage2] = std::make_unique<Sprite>(L".\\resources\\UI\\stage2.png");
		spriteBatchs[SpriteKind::Stage3] = std::make_unique<Sprite>(L".\\resources\\UI\\stage3.png");
		// 全てを合成するテクスチャ
		renderSprite = std::make_unique<Sprite>();
	}
}
// 終了化
void SceneTitle::Finalize()
{
	StageManager::Instance().clear();
	EnemyManager::Instance().Clear();
	LightManager::Instance().Clear();
}

// 更新処理
void SceneTitle::Update(float elapsedTime)
{
	// タイトルBGM再生
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Title)->Play(true);
	
	// プレイヤー更新処理
	player->TitleUpdate(elapsedTime);
	// タワー更新処理
	tower->Update(elapsedTime);
	// エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime);
	// カメラコントローラー更新処理
	cameraController->TitleUpdate(elapsedTime);
	// ステージ更新処理
	StageManager::Instance().Update(elapsedTime);
	// ゲームパッド入力情報
	GamePad& pad = Input::Instance().GetGamePad();
	
	{
		
		const GamePadButton anyButton = GamePad::BTN_A | GamePad::BTN_B | GamePad::BTN_X | GamePad::BTN_Y;
		// キー入力があれば
		if (pad.GetButtonDown() & anyButton)
		{
			// 決定音再生
			AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Ketei)->Play(false);
#if 0
			if (titleMode == TitleMode::TitleStart)
			{
				player->SetTitleState(Player::TitleState::TitleSelect);
			}
			else if (player->GetTitleState() == Player::TitleState::TitleSelect && titleMode >= TitleMode::Select)
			{
				player->SetTitleState(Player::TitleState::TitlePunchStart);
			}
#else
			if (player->GetTitleState() == Player::TitleState::TitleSelect && titleMode >= TitleMode::Select)
			{
				player->SetTitleState(Player::TitleState::TitlePunchStart);
			}
#endif
			titleMode++;
		}
		if (titleMode > TitleMode::TitleStart)titleDissolveTimer += elapsedTime;

		// タイトルでキー未入力状態じゃなければ
		if (titleMode > TitleMode::TitleStart && titleMode < TitleMode::Select)
		{
			// タイトルログ&決定テクスチャのディゾルブ時間加算
			titleDissolveTimer += elapsedTime;
			// プレイヤーのタイトルでの状態が選択中なら
			if (player->GetTitleState() == Player::TitleState::TitleSelect)
			{
				if (pad.GetButtonDown() & GamePad::BTN_DOWN)
				{
					gameMode++;
					if (gameMode > (int)GamaMode::Tutorial) gameMode = (int)GamaMode::Game;
				}
				else if (pad.GetButtonDown() & GamePad::BTN_UP)
				{
					gameMode--;
					if (gameMode < (int)GamaMode::Game) gameMode = (int)GamaMode::Tutorial;
				}
			}
		}
		if (titleMode == TitleMode::Select) {
			if (pad.GetButtonDown() & GamePad::BTN_DOWN)
			{
				stageMode++;
				if (stageMode > (int)StageMode::Stage3) stageMode = (int)StageMode::Stage1;
			}
			else if (pad.GetButtonDown() & GamePad::BTN_UP)
			{
				stageMode--;
				if (stageMode < (int)StageMode::Stage1) stageMode = (int)StageMode::Stage3;
			}

		}
	}

	// プレイヤーのタイトルでの状態が殴り余韻状態以降なら
	if (player->GetTitleState() >= Player::TitleState::TitlePunchReverberation)
	{
		static constexpr float value = 3.0f;
		// 殴りによるサイズ拡大が一定値(3.0f)を超えたら、サイズを固定
		if (punchScale >= value)
		{
			punchScale = value;
		}
		// 殴りによるサイズ拡大が一定値(3.0f)を超えていなければ
		else
		{
			punchScale += elapsedTime * 12.0f;		// 殴りでサイズを拡大
			punchRotate += elapsedTime * 40.0f;		// 殴りで回転値加算
			punchPosition += elapsedTime * 12.0f;	// 殴りで位置調整
			// 殴りによる位置調整が一定値(1.0f)を超えたら、位置を固定
			if (punchPosition >= 1.0f)
			{
				punchPosition = 1.0f;
			}
		}
	}
#if 0
	else
	{
		punchScale = 1.0f;
		punchRotate = 0.0f;
		punchPosition = 0.0f;
	}
#endif
	// 線形補完位置
	float lerpPosition = 0.0f;
	// プレイヤーのタイトルでの状態が蹴落としてる余韻状態なら、線形補完位置を画面外に設定
	if (player->GetTitleState() == Player::TitleState::TitleKickReverberation)
	{
		lerpPosition = SCREEN_HEIGHT * 1.25f;
	}

	// タイトルモードがゲームモードを選択した後なら
	if (titleMode >= TitleMode::StageSelect)
	{
		textDissolveTimer += elapsedTime * 0.5f;
	}
	if(gameMode == (int)GamaMode::Tutorial && titleMode >= TitleMode::Select)textDissolveTimer += elapsedTime * 0.5f;
	// ゲーム&チュートリアルテクスチャのディゾルブ時間が一定値(1.0f)を超えたら
	if (textDissolveTimer > 1.0f) 
	{
		// タイトルBGM停止
		AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Title)->Stop();
		// ゲームモード別処理
		switch (gameMode) 
		{
		case (int)GamaMode::Game:		// ゲーム

			
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
			stagNo = stageMode;
			break;
		case (int)GamaMode::Tutorial:	// チュートリアル
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTutorial));
			break;
		}
	}	
	SceneManager::Instance().SetStage(stagNo);
	// 経過時間加算
	progressTimer++;
	// スカイボックス経過時間加算
	skyboxTimer += elapsedTime;
	skyboxColor -= elapsedTime;
	if (skyboxColor <= 0.0f)
	{
		skyboxColor = 0.0f;
	}
	// ゲームモードの選択切り替えによる処理
	{
		// 線形補完速度
		static constexpr float speed = 15.0f;
		// ゲームモードに応じてサイズ調整
		lerpScale = Mathf::Lerp(lerpScale, static_cast<float>(gameMode) * 0.5f, elapsedTime * speed);
		// ゲームモードに応じて回転調整
		cosRadian = Mathf::Lerp(cosRadian, DirectX::XM_PI * gameMode, elapsedTime * speed);
		// 非選択のゲームモードを蹴落とす処理
		dropPositionY = Mathf::Lerp(dropPositionY, lerpPosition, elapsedTime * speed);
	}
}

// 描画処理
void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
	renderTargetView = graphics.GetRenderTargetView();
	samplerState = graphics.GetSamplerState(2);

	//塗りつぶす色の設定
	FLOAT color[]{ 0.9f,0.9f,0.2f,1.0f };
	//レンダーターゲットビューのクリア。キャンバス全体を指定した色で塗りつぶす。
	//クリアするレンダーターゲットビューとクリアする色を指定
	immediate_context->ClearRenderTargetView(renderTargetView.Get(), color);
	//深度ステンシルビューのクリア。キャンバス全体の奥行情報をリセット。
	//クリアする深度ステンシルビューとクリアする深度ステンシルのフラグ、深度バッファをクリアする
	immediate_context->ClearDepthStencilView(graphics.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//これから描くキャンバスを指定する
	immediate_context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), graphics.GetDepthStencilView());

	//アニソトロピックをs2
	immediate_context->PSSetSamplers(2, 1, samplerState.GetAddressOf());

	RenderContext rc;
	rc.deviceContext = immediate_context;
#if 1
	// シャドウバッファ	
	{
		// ライトの方向
		DirectX::XMFLOAT4 lightDirection = { 0.5f, -1.0f, -0.5f, 0.0f };
		rc.lightDirection = lightDirection;
		shadowbuffer->clear(immediate_context);
		shadowbuffer->activate(immediate_context);
		{
			DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat4(&lightDirection);
			DirectX::XMVECTOR Position = {};
			float lightLength = 300.0f;
			LightPosition = DirectX::XMVectorScale(LightPosition, lightLength);
			DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(Position - LightPosition, Position, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			// シャドウマップに描画したい範囲の射影行列を生成
			DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f, 500.0f);
			DirectX::XMStoreFloat4x4(&rc.view, V);
			DirectX::XMStoreFloat4x4(&rc.projection, P);
			DirectX::XMFLOAT4X4 lightViewProjection;
			DirectX::XMStoreFloat4x4(&lightViewProjection, V * P);
			// ライトビュープロジェクション行列
			rc.shadowMapData.lightViewProjection = lightViewProjection;
			rc.shadowMapData.shadowMap = shadowbuffer->SrvShadow.Get();
		}
		ModelShader* shadowShader = graphics.GetShader(Graphics::ModelShaderId::ShadowmapCaster);
		shadowShader->Begin(immediate_context, rc);
		StageManager::Instance().Render(immediate_context, shadowShader);
		EnemyManager::Instance().Render(immediate_context, shadowShader);
		player->render(immediate_context, shadowShader);
		//tower->Render(immediate_context, shadowShader);
		StageManager::Instance().InstaningRender(immediate_context, shadowShader);
		shadowShader->End(immediate_context);
		shadowbuffer->deactivate(immediate_context);
	}
#endif
	// カメラ情報
	{
		Camera& camera = Camera::Instance();
		rc.viewPosition.x = camera.GetEye().x;
		rc.viewPosition.y = camera.GetEye().y;
		rc.viewPosition.z = camera.GetEye().z;
		rc.viewPosition.w = 1.0f;
		rc.view = camera.GetView();
		rc.projection = camera.GetProjection();
	}

	// 2D用にラスタライザステートの設定をGPU側に指定
	immediate_context->RSSetState(graphics.GetRasterizerState(2));
	// 深度テスト：オン、深度ライト：オンの深度ステンシルステートを設定しておく
	immediate_context->OMSetDepthStencilState(graphics.GetDepthStencilState(0), 1);

	// オフスクリーンバッファ
	{
		framebuffer->Clear(immediate_context);
		framebuffer->Activate(immediate_context);

		// スカイボックスシェーダー
		{
			SpriteShader* skyboxShader = graphics.GetShader(Graphics::SpriteShaderId::Skybox);
			skyboxShader->Begin(rc);
			spriteBatchs[SpriteKind::Skybox]->Render(immediate_context, skyboxColor, skyboxTimer);
			skyboxShader->Draw(rc, spriteBatchs[SpriteKind::Skybox].get());
			skyboxShader->End(rc);
		}

		// デフォルトモデルシェーダー
		{
			ModelShader* defaulModelShader = graphics.GetShader(Graphics::ModelShaderId::ModelShaderDefault);
			defaulModelShader->Begin(immediate_context, rc);
			StageManager::Instance().InstaningRender(immediate_context, defaulModelShader);
			//tower->Render(immediate_context, defaulModelShader);
			EnemyManager::Instance().Render(immediate_context, defaulModelShader);
			player->render(immediate_context, defaulModelShader);
			defaulModelShader->End(immediate_context);
		}
		framebuffer->Deactivate(immediate_context);
	}
	// ポストエフェクトバッファ
	{
		// 輝度抽出描画バッファ
		{
			subframebuffers[SubFrameBufferKind::Luminance]->Clear(immediate_context);
			subframebuffers[SubFrameBufferKind::Luminance]->Activate(immediate_context);
			SpriteShader* luminanceShader = graphics.GetShader(Graphics::SpriteShaderId::LuminanceExtraction);
			luminanceShader->Begin(rc);
			renderSprite->SetShaderResourceView(framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::Color)].Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
			luminanceShader->Draw(rc, renderSprite.get());
			subframebuffers[SubFrameBufferKind::Luminance]->Deactivate(immediate_context);
		}
		// ブルーム描画バッファ
		{
			subframebuffers[SubFrameBufferKind::Bloom]->Clear(immediate_context);
			subframebuffers[SubFrameBufferKind::Bloom]->Activate(immediate_context);
			bitBlockTransfer->blit(immediate_context, subframebuffers[SubFrameBufferKind::Luminance]->shaderResourceViews.GetAddressOf(), 0, 1, pixelShader[PixelShaderKind::GaussianBlur].Get());
			subframebuffers[SubFrameBufferKind::Bloom]->Deactivate(immediate_context);
		}
		// モデル描画バッファとブルーム描画バッファを合成し、ポストエフェクトを掛ける
		{
			static const int viewCount = 2;
			ID3D11ShaderResourceView* shaderResourceViews[viewCount] = { framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::Synthesis)].Get(), subframebuffers[SubFrameBufferKind::Bloom]->shaderResourceViews.Get() };
			subframebuffers[SubFrameBufferKind::Synthesis]->Clear(immediate_context);
			subframebuffers[SubFrameBufferKind::Synthesis]->Activate(immediate_context);
			bitBlockTransfer->blit(immediate_context, shaderResourceViews, 0, viewCount, pixelShader[PixelShaderKind::PostEffect].Get());
			subframebuffers[SubFrameBufferKind::Synthesis]->Deactivate(immediate_context);
		}
		// ジッタードリフトシェーダー描画バッファ
		{
			// ジッタードリフトシェーダー
			jitterDriftData.time += 1.0f / 60.0f;
			jitterDriftData.jitterStrength = skyboxColor / 200.0f;
			subframebuffers[SubFrameBufferKind::JitterDriftSFB]->Clear(immediate_context);
			subframebuffers[SubFrameBufferKind::JitterDriftSFB]->Activate(immediate_context);
			bitBlockTransfer->JitterDrift(immediate_context, subframebuffers[SubFrameBufferKind::Synthesis]->shaderResourceViews.GetAddressOf(), 13, 1, jitterDriftData, pixelShader[PixelShaderKind::JitterDriftPS].Get());
			subframebuffers[SubFrameBufferKind::JitterDriftSFB]->Deactivate(immediate_context);
		}
		// ポストエフェクトを掛け終わった画面をスプライトに設定
		renderSprite->SetShaderResourceView(subframebuffers[SubFrameBufferKind::JitterDriftSFB]->shaderResourceViews.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	// 2D描画
	{
		// デフォルトスプライトシェーダー
		SpriteShader* defaultSpriteShader = graphics.GetShader(Graphics::SpriteShaderId::SpriteShaderDefault);
		defaultSpriteShader->Begin(rc);
		// 背景テクスチャ描画後、3D空間描画
		{
			spriteBatchs[SpriteKind::Back]->Render(immediate_context);
			defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::Back].get());
			renderSprite->Render(immediate_context, 0, 0, textDissolveTimer * 3.0f);
			defaultSpriteShader->Draw(rc, renderSprite.get());
		}
		// スクリーン幅・高さ
		float screenWidth = graphics.GetScreenWidth();
		float screenHeight = graphics.GetScreenHeight();
		// 半分のスクリーン幅・高さ
		float divideScreenWidth = screenWidth * 0.5f;
		float divideScreenHeight = screenHeight * 0.5f;
		// タイトルロゴテクスチャ描画
		{
			static constexpr float textureScale = 0.4f;
			float dw = static_cast<float>(spriteBatchs[SpriteKind::Title]->GetTextureWidth()) * textureScale;	// 描画幅
			float dh = static_cast<float>(spriteBatchs[SpriteKind::Title]->GetTextureHeight()) * textureScale;	// 描画高さ
			spriteBatchs[SpriteKind::Title]->Render(immediate_context, 0.0f, 0.0f, dw, dh, titleDissolveTimer);
			defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::Title].get());
		}
		// 決定(GameStart)テクスチャ描画
		{
			float dx = divideScreenWidth;		// 描画X位置
			float dy = screenHeight * 0.75f;	// 描画Y位置
			static constexpr float textureScale = 0.75f;
			float dw = static_cast<float>(spriteBatchs[SpriteKind::DecisionText]->GetTextureWidth()) * textureScale;	// 描画幅
			float dh = static_cast<float>(spriteBatchs[SpriteKind::DecisionText]->GetTextureHeight()) * textureScale; // 描画高さ
			spriteBatchs[SpriteKind::DecisionText]->Render(immediate_context, dx - (dw * 0.5f), dy - (dh * 0.5f), dw, dh, titleDissolveTimer);

			if (progressTimer / 20 % 2)
			{
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::DecisionText].get());
			}
		}
		// ゲーム&チュートリアルテクスチャ描画
		{
			static constexpr float texturePositionRateX = 0.75f;			// スクリーン幅に掛ける割合
			static constexpr float texturePositionRateY = 0.35f;			// スクリーン幅に掛ける割合
			float dx = screenWidth * texturePositionRateX;					// 描画X位置
			float dy = screenHeight * texturePositionRateY;					// 描画Y位置
			float radius = screenHeight * (0.5f - texturePositionRateY);	// テクスチャ間の基準点からの半径
			DirectX::XMFLOAT2 textureSize = { 500.0f, 100.0f };				// 描画幅・高さ
			
			// ゲームモード別処理分け変数
			float selectScale = punchScale;	// 殴りによるサイズ調整
			float decisionScale = 1.0f;		// 選択したゲームモードでサイズ調整
			float selectRotate = punchRotate;	// 殴りによる回転値調整
			float decisionRotate = 0.0f;		// 選択したゲームモードで回転値調整
			float selectPosition = punchPosition;	// 殴りによる位置調整
			DirectX::XMFLOAT2 decisionPosition = {};	// 選択したゲームモードで位置調整
			float dropPosition = 0.0f;	// 非選択のゲームモードを蹴落とす位置
			// ゲームテクスチャ
			{
				// ゲームモード別処理分け
				switch (gameMode)
				{
				case (int)GamaMode::Game:		// ゲーム
					decisionScale = selectScale;
					decisionRotate = selectRotate;
					decisionPosition.x = -screenWidth * (texturePositionRateX - 0.5f) * selectPosition;
					decisionPosition.y = screenHeight * (0.5f - texturePositionRateY) * selectPosition;
					dropPosition = 0.0f;
					break;
				case (int)GamaMode::Tutorial:	// チュートリアル
					decisionScale = 1.0f;
					decisionRotate = 0.0f;
					decisionPosition = {};
					dropPosition = dropPositionY;
					break;
				}
				// ゲーム(0) : 500,100、チュートリアル(1) : 250,50
				float width = textureSize.x * (1.0f - lerpScale) * decisionScale;
				float height = textureSize.y * (1.0f - lerpScale) * decisionScale;
				float divideWidth = width * 0.5f;
				float divideHeight = height * 0.5f;
				spriteBatchs[SpriteKind::GameStartText]->RotateRender(immediate_context,
					dx - divideWidth + decisionPosition.x + cosf(cosRadian - DirectX::XM_PIDIV2) * radius, divideScreenHeight - divideHeight + dropPosition + decisionPosition.y + cosf(cosRadian + DirectX::XM_PI) * radius, width, height,
					decisionRotate, textDissolveTimer * 3.0f);
			}
			// チュートリアルテクスチャ
			{
				// ゲームモード別処理分け
				switch (gameMode)
				{
				case (int)GamaMode::Game:		// ゲーム
					decisionScale = 1.0f;
					decisionRotate = 0.0f;
					decisionPosition = {};
					dropPosition = dropPositionY;
					break;
				case (int)GamaMode::Tutorial:	// チュートリアル
					decisionScale = selectScale;
					decisionRotate = selectRotate;
					decisionPosition.x = -screenWidth * (texturePositionRateX - 0.5f) * selectPosition;
					decisionPosition.y = -screenHeight * (texturePositionRateY - 0.5f) * selectPosition;
					dropPosition = 0.0f;
					break;
				}
				// ゲーム(0) : 250,50、チュートリアル(1) : 500,100
				float width = textureSize.x * (0.5f + lerpScale) * decisionScale;
				float height = textureSize.y * (0.5f + lerpScale) * decisionScale;
				float divideWidth = width * 0.5f;
				float divideHeight = height * 0.5f;
				spriteBatchs[SpriteKind::TutorialText]->RotateRender(immediate_context,
					dx - divideWidth + decisionPosition.x + cosf(cosRadian + DirectX::XM_PIDIV2) * radius, divideScreenHeight - divideHeight + dropPosition + decisionPosition.y + cosf(cosRadian) * radius, width, height,
					decisionRotate, textDissolveTimer * 3.0f);
			}

			// タイトルモードが非選択状態以降なら
			if (titleMode >= TitleMode::NotSelect && titleMode < TitleMode::Select)
			{
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::GameStartText].get());
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::TutorialText].get());
			}
			if (titleMode >= TitleMode::Select && gameMode == (int)GamaMode::Game)
			{
				float stageScale[3] = { 0.5,0.5,0.5 };
				switch (stageMode)
				{
				case (int)StageMode::Stage1:
					stageScale[0] = 0.7f;
					break;
				case (int)StageMode::Stage2:
					stageScale[1] = 0.7f;
					break;
				case (int)StageMode::Stage3:
					stageScale[2] = 0.7f;
					break;
				}
				spriteBatchs[SpriteKind::Stage1]->Render(immediate_context,
					800.0f, 300.0f, static_cast<float>(spriteBatchs[SpriteKind::Stage1]->GetTextureWidth()) * stageScale[0], static_cast<float>(spriteBatchs[SpriteKind::Stage1]->GetTextureHeight() * stageScale[0]),
					0.0f, 0.0f, static_cast<float>(spriteBatchs[SpriteKind::Stage1]->GetTextureWidth()), static_cast<float>(spriteBatchs[SpriteKind::Stage1]->GetTextureHeight()),
					0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					textDissolveTimer * 5
				);
				spriteBatchs[SpriteKind::Stage2]->Render(immediate_context,
					800.0f, 400.0f, static_cast<float>(spriteBatchs[SpriteKind::Stage2]->GetTextureWidth()) * stageScale[1], static_cast<float>(spriteBatchs[SpriteKind::Stage1]->GetTextureHeight()) * stageScale[1],
					0.0f, 0.0f, static_cast<float>(spriteBatchs[SpriteKind::Stage2]->GetTextureWidth()), static_cast<float>(spriteBatchs[SpriteKind::Stage1]->GetTextureHeight()),
					0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					textDissolveTimer * 5
				);
				spriteBatchs[SpriteKind::Stage3]->Render(immediate_context,
					800.0f, 500.0f, static_cast<float>(spriteBatchs[SpriteKind::Stage3]->GetTextureWidth()) * stageScale[2], static_cast<float>(spriteBatchs[SpriteKind::Stage1]->GetTextureHeight()) * stageScale[2],
					0.0f, 0.0f, static_cast<float>(spriteBatchs[SpriteKind::Stage3]->GetTextureWidth()), static_cast<float>(spriteBatchs[SpriteKind::Stage1]->GetTextureHeight()),
					0.0f,
					1.0f, 1.0f, 1.0f, 1.0f,
					textDissolveTimer * 5
				);
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::Stage1].get());
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::Stage2].get());
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::Stage3].get());
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::GameStartText].get());
			}
		}
		defaultSpriteShader->End(rc);
	}	

	// ImGui描画処理
	ImGuiRender();
}
// ImGui描画処理
void SceneTitle::ImGuiRender()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_None))
	{
		if (ImGui::Button(u8"Damage"))
		{
			skyboxColor = 1.0f;
		}
#if 0
		if (ImGui::TreeNode("JitterDrift"))
		{
			ImGui::SliderFloat("JitterStrength", &jitterDriftData.jitterStrength, 0.0f, 1.0f);
			ImGui::TreePop();
		}
		ImGui::SliderFloat("skyboxColor", &skyboxColor, 0.0f, 1.0f);
#endif
		ImGui::SliderFloat("punchScale", &punchScale, 1.0f, 3.0f);
		ImGui::SliderFloat("lerpScale", &lerpScale, 1.0f, 3.0f);
		ImGui::SliderFloat("punchPosition", &punchPosition, 0.0f, 1.0f);
		if (ImGui::TreeNode("Shadowmap"))
		{
			ImGui::SliderFloat("DrawRect", &shadowDrawRect, 1.0f, 1150.0f);
			ImGui::Text("texture");
			ImGui::Image(shadowbuffer->SrvShadow.Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("MRT"))
		{
			ImGui::Text("texture");
			ImGui::Image(renderSprite->GetShaderResourceView().Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::Color)].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::Depth)].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::Nomal)].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::Pos)].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::MetalSmoothness)].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::AmbientOcclusion)].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::Emission)].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::Light)].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::Synthesis)].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(subframebuffers[SubFrameBufferKind::Luminance]->shaderResourceViews.Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(subframebuffers[SubFrameBufferKind::Bloom]->shaderResourceViews.Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(subframebuffers[SubFrameBufferKind::Synthesis]->shaderResourceViews.Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::Image(subframebuffers[SubFrameBufferKind::JitterDriftSFB]->shaderResourceViews.Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
			ImGui::TreePop();
		}
		ImGui::End();
	}	

	player->DrawDebugGUI();
	EnemyManager::Instance().DrawDebugGUI();
}