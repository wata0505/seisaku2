#pragma once

#include "Scene.h"
#include "Graphics.h"
#include "Framebuffer.h"
#include "SubFramebuffer.h"
#include "Shadowbuffer.h"
#include "FullscreenQuad.h"
#include "AudioAll.h"
#include "EffectAll.h"
#include "Player.h"
#include "MetaAI.h"
#include "Base.h"
#include "CameraController.h"

using namespace DirectX;

class SceneTitle : public Scene
{
public:
	SceneTitle() {}
	~SceneTitle() override {}

	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render()override;

private:
	// ImGui�`�揈��
	void ImGuiRender();

private:
	// �^�C�g�����[�h
	enum TitleMode
	{
		TitleStart,	// �^�C�g���J�n
		NotSelect,	// ��I��
		Select,		// �I��
	};

	// �Q�[�����[�h
	enum class GamaMode
	{
		Game,		// �Q�[��
		Tutorial	// �`���[�g���A��
	};

	// �s�N�Z���V�F�[�_�[���
	enum PixelShaderKind
	{
		PostEffect,		// �|�X�g�G�t�F�N�g
		GaussianBlur,	// �K�E�V�A���u���[
		RadiationBlur,	// ���˃u���[

		PSMax
	};

	// �T�u�t���[���o�b�t�@���
	enum SubFrameBufferKind
	{
		Luminance,		// �P�x���o
		Bloom,			// �u���[��
		Synthesis,		// ����

		SFBMax
	};

	// �X�v���C�g���
	enum SpriteKind
	{
		Skybox,			// �X�J�C�{�b�N�X
		Title,			// �^�C�g��
		DecisionText,	// ����
		Back,			// �w�i
		GameStartText,	// �Q�[��
		TutorialText,	// �`���[�g���A��

		SMax
	};

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView;								// �����_�[�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;									// �T���v���[�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader[PixelShaderKind::PSMax];			// �|�X�g�G�t�F�N�g�s�N�Z���V�F�[�_�[
	std::unique_ptr<Framebuffer>					framebuffer;									// �I�t�X�N���[���o�t�@	
	std::unique_ptr<SubFramebuffer>					subframebuffers[SubFrameBufferKind::SFBMax];	// �|�X�g�G�t�F�N�g�o�b�t�@
	std::unique_ptr<Shadowbuffer>					shadowbuffer;									// �V���h�E�o�b�t�@
	std::unique_ptr<FullscreenQuad>					bitBlockTransfer;								// �|�X�g�G�t�F�N�g����
	std::shared_ptr<Sprite>							renderSprite;									// ��ʂɏo�͂���e�N�X�`��
	std::shared_ptr<Sprite>							spriteBatchs[SpriteKind::SMax];					// �^�C�g���ł̃e�N�X�`��
	std::unique_ptr<Player>							player = nullptr;								// �v���C���[
	std::unique_ptr<Meta>							meta = nullptr;									// MetaAI
	std::unique_ptr<Base>							tower = nullptr;								// �^���[
	std::unique_ptr<CameraController>				cameraController = nullptr;						// �J�����R���g���[���[
	float											shadowDrawRect = 10.0f;							// �V���h�E�}�b�v�ɕ`�悷��͈�
	float											titleDissolveTimer = 0;							// �^�C�g�����O&����e�N�X�`���̃f�B�]���u����
	float											textDissolveTimer = 0;							// �Q�[��&�`���[�g���A���e�N�X�`���̃f�B�]���u����
	float											skyboxTimer = 0.0f;								// �X�J�C�{�b�N�X�o�ߎ���
	float											dropPositionY = 0.0f;							// ��I���̃Q�[�����[�h���R���Ƃ��ʒu
	float											punchPosition = 0.0f;							// ����ɂ��ʒu����
	float											punchScale = 1.0f;								// ����ɂ��T�C�Y����
	float											lerpScale = 0.0f;								// �Q�[��&�`���[�g���A���e�N�X�`���̃T�C�Y����
	float											punchRotate = 0.0f;								// ����ɂ���]�l
	float											cosRadian = 0.0f;								// �Q�[��&�`���[�g���A���e�N�X�`������]������p�x
	int												progressTimer = 0;								// �o�ߎ���
	int												titleMode = 0;									// �^�C�g�����[�h
	int												gameMode = 0;									// �Q�[�����[�h
};