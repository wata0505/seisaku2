#include "SceneTitle.h"
#include "SceneLoading.h"
#include "SceneGame.h"
#include "SceneTutorial.h"
#include "SceneManager.h"
#include "Shader.h"
#include "AudioAll.h"
#include "EffectAll.h"
#include "EffectManager.h"
#include "RenderContext.h"
#include "Camera.h"
#include "StageObj.h"
#include "StageManager.h"
#include "LightManager.h"
#include "Input.h"
#include "Mathf.h"
#include "Misc.h"

// ������
void SceneTitle::Initialize()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11Device* device = graphics.GetDevice();

	// �G�t�F�N�g�e�N�X�`������
	EffectTexAll* effectTexAll = new EffectTexAll();
	// �G�t�F�N�g����
	EffectAll* effectAll = new EffectAll();
	// �I�[�f�B�I����
	AudioAll* audioAll = new AudioAll();

	// �X�e�[�W����
	{
		StageManager& stageManager = StageManager::Instance();
		// �Ⴂ�ʒu�����
		DirectX::XMFLOAT3 position = { 0.0f, -10.0f, 0.0f };
		for (int index = -2; index < 2; index++)
		{
			// ���_���}�C�i�X�ʒu->�v���X�ʒu�ɐ���
			position.x = index * 20.0f;
			StageObj* stageObj = new StageObj(position);
			stageManager.Register(stageObj);
		}

		position.y = -0.275f;
		position.z -= 40.0f;
		tower = std::make_unique<Base>(position);
	}

	// �v���C���[������
	{
		player = std::make_unique<Player>();
		player->SetPosition({});	// �ʒu�����_��
	}

	// �J����������
	{
		Camera& camera = Camera::Instance();
		camera.SetLookAt(DirectX::XMFLOAT3(0, 0, -10), {}, DirectX::XMFLOAT3(0, 1, 0));
		camera.SetPerspectiveFov(DirectX::XMConvertToRadians(45),
			graphics.GetScreenWidth() / graphics.GetScreenHeight(),
			0.1f, 1000.0f);
	}
	// �J�����R���g���[���[������
	{
		cameraController = std::make_unique<CameraController>();
		cameraController->SetRangeMax(5);
		cameraController->SetRange(5);
		// �J�����ʒu�ݒ�
		DirectX::XMFLOAT3 position = player->GetPosition();
		position.x += 2.0f;
		position.y += 1.5f;
		cameraController->SetTarget(position);
	}

	// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	{
		create_ps_from_cso(device, "Shader\\PostEffectPS.cso", pixelShader[PixelShaderKind::PostEffect].GetAddressOf());
		create_ps_from_cso(device, "Shader\\BlurPS.cso", pixelShader[PixelShaderKind::GaussianBlur].GetAddressOf());
		create_ps_from_cso(device, "Shader\\EffectPS.cso", pixelShader[PixelShaderKind::RadiationBlur].GetAddressOf());
		// �W�b�^�[�h���t�g�萔�o�b�t�@
		{
			// �萔�o�b�t�@�I�u�W�F�N�g�쐬
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

		// �`��o�b�t�@
		framebuffer = std::make_unique<Framebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
		subframebuffers[SubFrameBufferKind::Luminance] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
		subframebuffers[SubFrameBufferKind::Bloom] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		subframebuffers[SubFrameBufferKind::Synthesis] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
		subframebuffers[SubFrameBufferKind::JitterDriftSFB] = std::make_unique<SubFramebuffer>(device, SCREEN_WIDTH, SCREEN_HEIGHT);
		// �V���h�E�o�b�t�@
		static const int ShadowMapSize = 2096;
		shadowbuffer = std::make_unique<Shadowbuffer>(device, ShadowMapSize, ShadowMapSize);
		// �e�N�X�`��
		spriteBatchs[SpriteKind::Title] = std::make_unique<Sprite>(L".\\resources\\UI\\Title\\title.png");
		spriteBatchs[SpriteKind::DecisionText] = std::make_unique<Sprite>(L".\\resources\\UI\\Title\\GameStart.png");
		spriteBatchs[SpriteKind::Back] = EffectTexAll::Instance().GetSprite(int(EffectTexAll::EfTexAll::Bock));
		spriteBatchs[SpriteKind::GameStartText] = std::make_unique<Sprite>(L".\\resources\\UI\\Title\\StartTheStory.png");
		spriteBatchs[SpriteKind::TutorialText] = std::make_unique<Sprite>(L".\\resources\\UI\\Title\\Tutorial.png");
		spriteBatchs[SpriteKind::Stage1] = std::make_unique<Sprite>(L".\\resources\\UI\\Title\\Stage1.png");
		spriteBatchs[SpriteKind::Stage2] = std::make_unique<Sprite>(L".\\resources\\UI\\Title\\Stage2.png");
		spriteBatchs[SpriteKind::Stage3] = std::make_unique<Sprite>(L".\\resources\\UI\\Title\\Stage3.png");
		//spriteBatchs[SpriteKind::StageTextFrame] = std::make_unique<Sprite>(L".\\resources\\UI\\stageTextFrame.png");
		spriteBatchs[SpriteKind::StageBack] = std::make_unique<Sprite>(L".\\resources\\UI\\Title\\StageBack.png");
		spriteBatchs[SpriteKind::CyberCircle] = std::make_unique<Sprite>(L".\\resources\\UI\\Title\\CyberCircle.png");
		// �S�Ă���������e�N�X�`��
		renderSprite = std::make_unique<Sprite>();
	}
}
// �I����
void SceneTitle::Finalize()
{
	StageManager::Instance().Clear();
	LightManager::Instance().Clear();
}

// �X�V����
void SceneTitle::Update(float elapsedTime)
{
	// �^�C�g��BGM�Đ�
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Title)->Play(true, 0.3f);
	
	// �v���C���[�X�V����
	player->TitleUpdate(elapsedTime);
	// �^���[�X�V����
	tower->Update(elapsedTime);
	// �J�����R���g���[���[�X�V����
	cameraController->TitleUpdate(elapsedTime);
	// �X�e�[�W�X�V����
	StageManager::Instance().Update(elapsedTime);
		
	{
		// �Q�[���p�b�h���͏��
		GamePad& pad = Input::Instance().GetGamePad();
		const GamePadButton anyButton = GamePad::BTN_A | GamePad::BTN_B | GamePad::BTN_X | GamePad::BTN_Y;
		// �L�[���͂������
		if (pad.GetButtonDown() & anyButton)
		{
			if ((titleMode == TitleMode::Select && gameMode == (int)GamaMode::Game) || (titleMode == TitleMode::NotSelect && gameMode == (int)GamaMode::Tutorial))
			{
				player->SetTitleState(Player::TitleState::TitlePunchStart);
			}
			else if (player->GetTitleState() < Player::TitleState::TitlePunchStart)
			{
				// ���艹��~�ƍĐ�
				AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Ketei)->Stop();
				AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Ketei)->Play(false, 0.3f);
			}

			titleMode++;
		}
		if (titleMode > TitleMode::TitleStart)
		{
			titleDissolveTimer += elapsedTime;
		}

		bool isSelectSE = false;
		// �^�C�g���ŃL�[�����͏�Ԃ���Ȃ����
		if (titleMode > TitleMode::TitleStart && titleMode < TitleMode::Select)
		{
			// �^�C�g�����O&����e�N�X�`���̃f�B�]���u���ԉ��Z
			titleDissolveTimer += elapsedTime;
			if (pad.GetButtonDown() & GamePad::BTN_DOWN)
			{
				isSelectSE = true;
				gameMode++;
				if (gameMode > (int)GamaMode::Tutorial) gameMode = (int)GamaMode::Game;
			}
			else if (pad.GetButtonDown() & GamePad::BTN_UP)
			{
				isSelectSE = true;
				gameMode--;
				if (gameMode < (int)GamaMode::Game) gameMode = (int)GamaMode::Tutorial;
			}
		}
		if (titleMode == TitleMode::Select && gameMode == (int)GamaMode::Game)
		{
			if (pad.GetButtonDown() & GamePad::BTN_DOWN)
			{
				isSelectSE = true;
				stageMode++;
				if (stageMode > (int)StageMode::Stage3) stageMode = (int)StageMode::Stage1;
			}
			else if (pad.GetButtonDown() & GamePad::BTN_UP)
			{
				isSelectSE = true;
				stageMode--;
				if (stageMode < (int)StageMode::Stage1) stageMode = (int)StageMode::Stage3;
			}
		}

		if (isSelectSE)
		{
			AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Slash2)->Stop();
			AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Slash2)->Play(false, 0.3f);
		}
	}
	// �e�N�X�`���֘A�̍X�V����
	UpdateTextureData(elapsedTime);	
}
// �e�N�X�`���֘A�̍X�V����
void SceneTitle::UpdateTextureData(float elapsedTime)
{
	// �v���C���[�̃^�C�g���ł̏�Ԃ�����]�C��Ԉȍ~�Ȃ�
	if (player->GetTitleState() >= Player::TitleState::TitlePunchReverberation)
	{
		static constexpr float value = 3.0f;
		// ����ɂ��T�C�Y�g�傪���l(3.0f)�𒴂�����A�T�C�Y���Œ�
		if (punchScale >= value)
		{
			punchScale = value;
			skyboxColor = 1.0f;
		}
		// ����ɂ��T�C�Y�g�傪���l(3.0f)�𒴂��Ă��Ȃ����
		else
		{
			punchScale += elapsedTime * 12.0f;		// ����ŃT�C�Y���g��
			punchRotate += elapsedTime * 40.0f;		// ����ŉ�]�l���Z
			punchPosition += elapsedTime * 12.0f;	// ����ňʒu����
			// ����ɂ��ʒu���������l(1.0f)�𒴂�����A�ʒu���Œ�
			if (punchPosition >= 1.0f)
			{
				punchPosition = 1.0f;
			}
		}
	}

	bool isPunch = false;
	// �^�C�g�����[�h���Q�[�����[�h��I��������Ȃ�
	if ((titleMode >= TitleMode::StageSelect) || (gameMode == (int)GamaMode::Tutorial && titleMode >= TitleMode::Select))
	{
		isPunch = true;
	}
	
	if (titleMode >= TitleMode::Select)
	{
		cyberAlpha -= elapsedTime * 2.0f;
		if (cyberAlpha <= 0.0f)
		{
			cyberAlpha = 0.f;
		}
	}
	else if (titleMode >= TitleMode::NotSelect)
	{
		cyberAlpha += elapsedTime * 2.0f;
		if (cyberAlpha >= 0.75f)
		{
			cyberAlpha = 0.75f;
		}
	}

	if (isPunch)
	{
		waitPunchTimer += elapsedTime;
	}
	if (waitPunchTimer >= 1.0f)
	{
		textDissolveTimer += elapsedTime * 0.5f;
	}
	// �Q�[��&�`���[�g���A���e�N�X�`���̃f�B�]���u���Ԃ����l(1.0f)�𒴂�����
	if (textDissolveTimer > 1.0f)
	{
		// �^�C�g��BGM��~
		AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Title)->Stop();
		// �Q�[�����[�h�ʏ���
		switch (gameMode)
		{
		case (int)GamaMode::Game:		// �Q�[��			
			stagNo = stageMode;
			SceneManager::Instance().SetStage(stagNo);
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
			break;
		case (int)GamaMode::Tutorial:	// �`���[�g���A��
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTutorial));
			break;
		}
	}
	
	// �o�ߎ��ԉ��Z
	progressTimer++;
	// �T�C�o�[�T�[�N���e�N�X�`����]���ԉ��Z
	rotateTimer += elapsedTime * 6.0f;	
	// �X�J�C�{�b�N�X�o�ߎ��ԉ��Z
	skyboxTimer += elapsedTime;
	skyboxColor -= elapsedTime;
	if (skyboxColor <= 0.0f)
	{
		skyboxColor = 0.0f;
	}
	// �Q�[�����[�h�̑I��؂�ւ��ɂ�鏈��
	{
		// ���`�⊮���x
		static constexpr float speed = 15.0f;
		// �Q�[�����[�h�ɉ����ăT�C�Y����
		lerpScale = Mathf::Lerp(lerpScale, static_cast<float>(gameMode) * 0.5f, elapsedTime * speed);
		// �Q�[�����[�h�ɉ����ĉ�]����
		cosRadian = Mathf::Lerp(cosRadian, DirectX::XM_PI * static_cast<float>(gameMode), elapsedTime * speed);
		// �Q�[�����[�h�ɕύX����]����
		cyberRotate = Mathf::Lerp(cyberRotate, (static_cast<float>(gameMode) * 2.0f - 1.0f) * 45.0f, elapsedTime * 2.0f);
	}
	deltaTime = elapsedTime;
}

// �`�揈��
void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* immediate_context = graphics.GetDeviceContext();
	renderTargetView = graphics.GetRenderTargetView();
	samplerState = graphics.GetSamplerState(2);

	//�h��Ԃ��F�̐ݒ�
	FLOAT color[]{ 0.9f,0.9f,0.2f,1.0f };
	//�����_�[�^�[�Q�b�g�r���[�̃N���A�B�L�����o�X�S�̂��w�肵���F�œh��Ԃ��B
	//�N���A���郌���_�[�^�[�Q�b�g�r���[�ƃN���A����F���w��
	immediate_context->ClearRenderTargetView(renderTargetView.Get(), color);
	//�[�x�X�e���V���r���[�̃N���A�B�L�����o�X�S�̂̉��s�������Z�b�g�B
	//�N���A����[�x�X�e���V���r���[�ƃN���A����[�x�X�e���V���̃t���O�A�[�x�o�b�t�@���N���A����
	immediate_context->ClearDepthStencilView(graphics.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//���ꂩ��`���L�����o�X���w�肷��
	immediate_context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), graphics.GetDepthStencilView());

	//�A�j�\�g���s�b�N��s2
	immediate_context->PSSetSamplers(2, 1, samplerState.GetAddressOf());

	RenderContext rc;
	rc.deviceContext = immediate_context;
	// �V���h�E�o�b�t�@	
	{
		// ���C�g�̕���
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
			// �V���h�E�}�b�v�ɕ`�悵�����͈͂̎ˉe�s��𐶐�
			DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f, 500.0f);
			DirectX::XMStoreFloat4x4(&rc.view, V);
			DirectX::XMStoreFloat4x4(&rc.projection, P);
			DirectX::XMFLOAT4X4 lightViewProjection;
			DirectX::XMStoreFloat4x4(&lightViewProjection, V * P);
			// ���C�g�r���[�v���W�F�N�V�����s��
			rc.shadowMapData.lightViewProjection = lightViewProjection;
			rc.shadowMapData.shadowMap = shadowbuffer->SrvShadow.Get();
		}
		ModelShader* shadowShader = graphics.GetShader(Graphics::ModelShaderId::ShadowmapCaster);
		shadowShader->Begin(immediate_context, rc);
		StageManager::Instance().Render(immediate_context, shadowShader);
		player->render(immediate_context, shadowShader);
		StageManager::Instance().InstaningRender(immediate_context, shadowShader);
		shadowShader->End(immediate_context);
		shadowbuffer->deactivate(immediate_context);
	}

	// �J�������
	{
		Camera& camera = Camera::Instance();
		rc.viewPosition.x = camera.GetEye().x;
		rc.viewPosition.y = camera.GetEye().y;
		rc.viewPosition.z = camera.GetEye().z;
		rc.viewPosition.w = 1.0f;
		rc.view = camera.GetView();
		rc.projection = camera.GetProjection();
	}

	// 2D�p�Ƀ��X�^���C�U�X�e�[�g�̐ݒ��GPU���Ɏw��
	immediate_context->RSSetState(graphics.GetRasterizerState(2));
	// �[�x�e�X�g�F�I���A�[�x���C�g�F�I���̐[�x�X�e���V���X�e�[�g��ݒ肵�Ă���
	immediate_context->OMSetDepthStencilState(graphics.GetDepthStencilState(0), 1);

	// �I�t�X�N���[���o�b�t�@
	{
		framebuffer->Clear(immediate_context);
		framebuffer->Activate(immediate_context);

		// �X�J�C�{�b�N�X�V�F�[�_�[
		{
			SpriteShader* skyboxShader = graphics.GetShader(Graphics::SpriteShaderId::Skybox);
			skyboxShader->Begin(rc);
			spriteBatchs[SpriteKind::Back]->Render(immediate_context, skyboxColor, skyboxTimer);
			skyboxShader->Draw(rc, spriteBatchs[SpriteKind::Back].get());
			skyboxShader->End(rc);
		}

		// �f�t�H���g���f���V�F�[�_�[
		{
			ModelShader* defaulModelShader = graphics.GetShader(Graphics::ModelShaderId::ModelShaderDefault);
			defaulModelShader->Begin(immediate_context, rc);
			StageManager::Instance().InstaningRender(immediate_context, defaulModelShader);			
			player->render(immediate_context, defaulModelShader);
			defaulModelShader->End(immediate_context);
		}
		framebuffer->Deactivate(immediate_context);
	}
	// �|�X�g�G�t�F�N�g�o�b�t�@
	{
		// �P�x���o�`��o�b�t�@
		{
			subframebuffers[SubFrameBufferKind::Luminance]->Clear(immediate_context);
			subframebuffers[SubFrameBufferKind::Luminance]->Activate(immediate_context);
			SpriteShader* luminanceShader = graphics.GetShader(Graphics::SpriteShaderId::LuminanceExtraction);
			luminanceShader->Begin(rc);
			renderSprite->SetShaderResourceView(framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::Color)].Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
			luminanceShader->Draw(rc, renderSprite.get());
			subframebuffers[SubFrameBufferKind::Luminance]->Deactivate(immediate_context);
		}
		// �u���[���`��o�b�t�@
		{
			subframebuffers[SubFrameBufferKind::Bloom]->Clear(immediate_context);
			subframebuffers[SubFrameBufferKind::Bloom]->Activate(immediate_context);
			bitBlockTransfer->blit(immediate_context, subframebuffers[SubFrameBufferKind::Luminance]->shaderResourceViews.GetAddressOf(), 0, 1, pixelShader[PixelShaderKind::GaussianBlur].Get());
			subframebuffers[SubFrameBufferKind::Bloom]->Deactivate(immediate_context);
		}
		// ���f���`��o�b�t�@�ƃu���[���`��o�b�t�@���������A�|�X�g�G�t�F�N�g���|����
		{
			static const int viewCount = 2;
			ID3D11ShaderResourceView* shaderResourceViews[viewCount] = { framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::Synthesis)].Get(), subframebuffers[SubFrameBufferKind::Bloom]->shaderResourceViews.Get() };
			subframebuffers[SubFrameBufferKind::Synthesis]->Clear(immediate_context);
			subframebuffers[SubFrameBufferKind::Synthesis]->Activate(immediate_context);
			bitBlockTransfer->blit(immediate_context, shaderResourceViews, 0, viewCount, pixelShader[PixelShaderKind::PostEffect].Get());
			subframebuffers[SubFrameBufferKind::Synthesis]->Deactivate(immediate_context);
		}
		// �W�b�^�[�h���t�g�V�F�[�_�[�`��o�b�t�@
		{
			// �W�b�^�[�h���t�g�V�F�[�_�[
			jitterDriftData.time += 1.0f / 60.0f;
			jitterDriftData.jitterStrength = skyboxColor / 200.0f;
			subframebuffers[SubFrameBufferKind::JitterDriftSFB]->Clear(immediate_context);
			subframebuffers[SubFrameBufferKind::JitterDriftSFB]->Activate(immediate_context);
			bitBlockTransfer->JitterDrift(immediate_context, subframebuffers[SubFrameBufferKind::Synthesis]->shaderResourceViews.GetAddressOf(), 13, 1, jitterDriftData, pixelShader[PixelShaderKind::JitterDriftPS].Get());
			subframebuffers[SubFrameBufferKind::JitterDriftSFB]->Deactivate(immediate_context);
		}
		// �|�X�g�G�t�F�N�g���|���I�������ʂ��X�v���C�g�ɐݒ�
		renderSprite->SetShaderResourceView(subframebuffers[SubFrameBufferKind::JitterDriftSFB]->shaderResourceViews.Get(), SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	// 2D�`��
	{
		// �f�t�H���g�X�v���C�g�V�F�[�_�[
		SpriteShader* defaultSpriteShader = graphics.GetShader(Graphics::SpriteShaderId::SpriteShaderDefault);
		defaultSpriteShader->Begin(rc);
		// �w�i�e�N�X�`���`���A3D��ԕ`��
		{
			spriteBatchs[SpriteKind::Back]->Render(immediate_context);
			defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::Back].get());
			renderSprite->Render(immediate_context, 0, 0, textDissolveTimer * 3.0f);
			defaultSpriteShader->Draw(rc, renderSprite.get());
		}
		// �X�N���[�����E����
		float screenWidth = graphics.GetScreenWidth();
		float screenHeight = graphics.GetScreenHeight();
		// �����̃X�N���[�����E����
		float divideScreenWidth = screenWidth * 0.5f;
		float divideScreenHeight = screenHeight * 0.5f;
		// �^�C�g�����S�e�N�X�`���`��
		{
			static constexpr float textureScale = 0.4f;
			float dw = static_cast<float>(spriteBatchs[SpriteKind::Title]->GetTextureWidth()) * textureScale;	// �`�敝
			float dh = static_cast<float>(spriteBatchs[SpriteKind::Title]->GetTextureHeight()) * textureScale;	// �`�捂��
			spriteBatchs[SpriteKind::Title]->Render(immediate_context, 0.0f, 0.0f, dw, dh, titleDissolveTimer);
			defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::Title].get());
		}
		// ����(GameStart)�e�N�X�`���`��
		{
			float dx = divideScreenWidth;		// �`��X�ʒu
			float dy = screenHeight * 0.75f;	// �`��Y�ʒu
			static constexpr float textureScale = 0.75f;
			float dw = static_cast<float>(spriteBatchs[SpriteKind::DecisionText]->GetTextureWidth()) * textureScale;	// �`�敝
			float dh = static_cast<float>(spriteBatchs[SpriteKind::DecisionText]->GetTextureHeight()) * textureScale; // �`�捂��
			spriteBatchs[SpriteKind::DecisionText]->Render(immediate_context, dx - (dw * 0.5f), dy - (dh * 0.5f), dw, dh, titleDissolveTimer);

			if (progressTimer / 20 % 2)
			{
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::DecisionText].get());
			}
		}
		// �Q�[��&�`���[�g���A���e�N�X�`���`��
		{
			static constexpr float texturePositionRateX = 0.75f;			// �X�N���[�����Ɋ|���銄��
			static constexpr float texturePositionRateY = 0.35f;			// �X�N���[�����Ɋ|���銄��
			float dx = screenWidth * texturePositionRateX;					// �`��X�ʒu
			float dy = screenHeight * texturePositionRateY;					// �`��Y�ʒu
			float radius = screenHeight * (0.5f - texturePositionRateY);	// �e�N�X�`���Ԃ̊�_����̔��a
			DirectX::XMFLOAT2 textureSize = { 500.0f, 100.0f };				// �`�敝�E����
			
			// �Q�[�����[�h�ʏ��������ϐ�
			float selectScale = punchScale;	// ����ɂ��T�C�Y����
			float decisionScale = 1.0f;		// �I�������Q�[�����[�h�ŃT�C�Y����
			float selectRotate = punchRotate;	// ����ɂ���]�l����
			float decisionRotate = 0.0f;		// �I�������Q�[�����[�h�ŉ�]�l����
			float selectPosition = punchPosition;	// ����ɂ��ʒu����
			DirectX::XMFLOAT2 decisionPosition = {};	// �I�������Q�[�����[�h�ňʒu����
			// �Q�[���e�N�X�`��
			{
				// �Q�[�����[�h�ʏ�������
				switch (gameMode)
				{
				case (int)GamaMode::Game:		// �Q�[��
					decisionScale = selectScale;
					decisionRotate = selectRotate;
					decisionPosition.x = -screenWidth * (texturePositionRateX - 0.5f) * selectPosition;
					decisionPosition.y = screenHeight * (0.5f - texturePositionRateY) * selectPosition;
					break;
				case (int)GamaMode::Tutorial:	// �`���[�g���A��
					decisionScale = 1.0f;
					decisionRotate = 0.0f;
					decisionPosition = {};
					break;
				}
				// �Q�[��(0) : 500,100�A�`���[�g���A��(1) : 250,50
				float width = textureSize.x * (1.0f - lerpScale) * decisionScale;
				float height = textureSize.y * (1.0f - lerpScale) * decisionScale;
				float divideWidth = width * 0.5f;
				float divideHeight = height * 0.5f;
				spriteBatchs[SpriteKind::GameStartText]->RotateRender(immediate_context,
					dx - divideWidth + decisionPosition.x + cosf(cosRadian - DirectX::XM_PIDIV2) * radius, divideScreenHeight - divideHeight + decisionPosition.y + cosf(cosRadian + DirectX::XM_PI) * radius, width, height,
					decisionRotate, textDissolveTimer * 3.0f);
			}
			// �`���[�g���A���e�N�X�`��
			{
				// �Q�[�����[�h�ʏ�������
				switch (gameMode)
				{
				case (int)GamaMode::Game:		// �Q�[��
					decisionScale = 1.0f;
					decisionRotate = 0.0f;
					decisionPosition = {};
					break;
				case (int)GamaMode::Tutorial:	// �`���[�g���A��
					decisionScale = selectScale;
					decisionRotate = selectRotate;
					decisionPosition.x = -screenWidth * (texturePositionRateX - 0.5f) * selectPosition;
					decisionPosition.y = -screenHeight * (texturePositionRateY - 0.5f) * selectPosition;
					break;
				}
				// �Q�[��(0) : 250,50�A�`���[�g���A��(1) : 500,100
				float width = textureSize.x * (0.5f + lerpScale) * decisionScale;
				float height = textureSize.y * (0.5f + lerpScale) * decisionScale;
				float divideWidth = width * 0.5f;
				float divideHeight = height * 0.5f;
				spriteBatchs[SpriteKind::TutorialText]->RotateRender(immediate_context,
					dx - divideWidth + decisionPosition.x + cosf(cosRadian + DirectX::XM_PIDIV2) * radius, divideScreenHeight - divideHeight + decisionPosition.y + cosf(cosRadian) * radius, width, height,
					decisionRotate, textDissolveTimer * 3.0f);
			}
			// �T�C�o�[�T�[�N���e�N�X�`��
			{
				float width = static_cast<float>(spriteBatchs[SpriteKind::CyberCircle]->GetTextureWidth());
				float height = static_cast<float>(spriteBatchs[SpriteKind::CyberCircle]->GetTextureHeight());
				float divideWidth = width * 0.5f;
				float divideHeight = height * 0.5f;
				spriteBatchs[SpriteKind::CyberCircle]->Render(immediate_context,
					dx - divideWidth, divideScreenHeight - divideHeight, width, height,
					0.0f, 0.0f, width, height,
					rotateTimer + cyberRotate, 1.0f, 1.0f, 1.0f, cyberAlpha,
					textDissolveTimer * 3.0f);
			}

			// �^�C�g�����[�h����I����Ԉȍ~�Ȃ�
			if ((titleMode >= TitleMode::NotSelect && titleMode < TitleMode::Select) || (titleMode >= TitleMode::Select && gameMode == (int)GamaMode::Tutorial))
			{
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::CyberCircle].get());
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::GameStartText].get());
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::TutorialText].get());
			}
			if (titleMode >= TitleMode::Select && gameMode == (int)GamaMode::Game)
			{
				float stageScale[3] = { 0.5f, 0.5f, 0.5f };
				float selectScale[3] = { 0.5f, 0.5f, 0.5f };
				float shiftHeight[3] = { 30.0f, 200.0f, 280.0f };
				switch (stageMode)
				{
				case (int)StageMode::Stage1:
					stageScale[0] = 0.7f;
					selectScale[0] = 1.0f;
					break;
				case (int)StageMode::Stage2:
					stageScale[1] = 0.7f;
					selectScale[1] = 1.0f;
					shiftHeight[0] = 0.0f;
					shiftHeight[1] = 110.0f;
					break;
				case (int)StageMode::Stage3:
					stageScale[2] = 0.7f;
					selectScale[2] = 1.0f;
					shiftHeight[0] = 0.0f;
					shiftHeight[1] = 80.0f;
					shiftHeight[2] = 190.0f;
					break;
				}
						
				// �X�e�[�W�e�N�X�`��
				{
					// �f�B�]���u�l
					float dissolve = textDissolveTimer * 5.0f;
					// �e�N�X�`�����E����
					float textureWidth = static_cast<float>(spriteBatchs[SpriteKind::Stage1]->GetTextureWidth());
					float textureHeight = static_cast<float>(spriteBatchs[SpriteKind::Stage1]->GetTextureHeight());
					// �����̃e�N�X�`������
					float textureDivideHeight = textureHeight * 0.5f;
					// �`��X�ʒu
					float positionX = screenWidth * 0.6f;

					for (int index = SpriteKind::Stage1; index <= SpriteKind::Stage3; index++)
					{
						int changeValue = index - SpriteKind::Stage1;
						selectTextureScale[changeValue] = Mathf::Lerp(selectTextureScale[changeValue], selectScale[changeValue], deltaTime * 10.0f);						
						stageTexturePositionY[changeValue] = Mathf::Lerp(stageTexturePositionY[changeValue], screenHeight * 0.4f + shiftHeight[changeValue], deltaTime * 10.0f);
						float positionY = stageTexturePositionY[changeValue];
						float positionW = textureWidth * stageScale[changeValue] * selectTextureScale[changeValue];
						float positionH = textureHeight * stageScale[changeValue] * selectTextureScale[changeValue];						
						float shiftPositionX = positionW * (selectTextureScale[changeValue] - 1.0f);
						float shiftPositionY = positionH * (selectTextureScale[changeValue] - 1.0f);
						float shiftPositionW = positionW * (selectTextureScale[changeValue] - selectScale[changeValue]);
						float shiftPositionH = positionH * (selectTextureScale[changeValue] - selectScale[changeValue]);
						spriteBatchs[SpriteKind::StageBack]->Render(immediate_context,
							positionX - shiftPositionX, positionY - shiftPositionY,
							positionW + shiftPositionW, positionH + shiftPositionH,
							0.0f, 0.0f, textureWidth, textureHeight,
							0.0f, 1.0f, 1.0f, 1.0f, 1.0f, dissolve);
						defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::StageBack].get());
						spriteBatchs[index]->Render(immediate_context,
							//positionX - shiftPositionX, positionY - shiftPositionY - shiftPositionY * 0.5f,
							//positionW + shiftPositionW, positionH + shiftPositionH + shiftPositionY * 0.5f,
							positionX - shiftPositionX, positionY - shiftPositionY,
							positionW + shiftPositionW, positionH + shiftPositionH,
							0.0f, 0.0f, textureWidth, textureHeight,
							0.0f, 1.0f, 1.0f, 1.0f, 1.0f, dissolve);
						defaultSpriteShader->Draw(rc, spriteBatchs[index].get());
#if 0
						selectFrameTextureScale[changeValue] = Mathf::Lerp(selectFrameTextureScale[changeValue], selectScale[changeValue], deltaTime * 8.0f);
						float framePositionW2 = textureWidth * stageScale[changeValue] * selectFrameTextureScale[changeValue];
						float framePositionH2 = textureHeight * stageScale[changeValue] * selectFrameTextureScale[changeValue];
						float frameShiftPositionX = framePositionW2 * (selectFrameTextureScale[changeValue] - 1.0f);
						float frameShiftPositionY = framePositionH2 * (selectFrameTextureScale[changeValue] - 1.0f);
						float framePositionW = textureWidth * stageScale[changeValue];
						float framePositionH = textureHeight * stageScale[changeValue] - textureDivideHeight * stageScale[changeValue];
						spriteBatchs[SpriteKind::StageTextFrame]->Render(immediate_context,
							//positionX + shiftPositionX, positionY - shiftPositionY - shiftPositionY * 0.5f,
							positionX + frameShiftPositionX, positionY - frameShiftPositionY,
							framePositionW, framePositionH,
							0.0f, 0.0f, textureWidth, textureDivideHeight,
							0.0f, 1.0f, 1.0f, 1.0f, 1.0f, dissolve);
						defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::StageTextFrame].get());
						spriteBatchs[SpriteKind::StageTextFrame]->Render(immediate_context,
							//positionX - shiftPositionX, positionY + textureDivideHeight * stageScale[changeValue] + shiftPositionY + shiftPositionY * 0.5f,
							positionX - frameShiftPositionX, positionY + textureDivideHeight * stageScale[changeValue] + frameShiftPositionY,
							framePositionW, framePositionH,
							0.0f, textureDivideHeight, textureWidth, textureDivideHeight,
							0.0f, 1.0f, 1.0f, 1.0f, 1.0f, dissolve);
						defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::StageTextFrame].get());
#endif
					}
				}
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::CyberCircle].get());
				defaultSpriteShader->Draw(rc, spriteBatchs[SpriteKind::GameStartText].get());
			}
		}
		defaultSpriteShader->End(rc);
	}	

	// ImGui�`�揈��
	//ImGuiRender();
}
// ImGui�`�揈��
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
			ImGui::Image(framebuffer->shaderResourceViews[static_cast<int>(Framebuffer::Buffer::Position)].Get(), { 256, 144 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
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
}