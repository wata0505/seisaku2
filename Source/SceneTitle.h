#pragma once

#include "Scene.h"
#include "Graphics.h"
#include "Framebuffer.h"
#include "SubFramebuffer.h"
#include "Shadowbuffer.h"
#include "FullscreenQuad.h"
#include "Player.h"
#include "Base.h"
#include "CameraController.h"

using namespace DirectX;

class SceneTitle : public Scene
{
public:
	SceneTitle() {}
	~SceneTitle() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

private:
	// �e�N�X�`���֘A�̍X�V����
	void UpdateTextureData(float elapsedTime);

	// ImGui�`�揈��
	void ImGuiRender();

private:
	// �^�C�g�����[�h
	enum TitleMode
	{
		TitleStart,	// �^�C�g���J�n
		NotSelect,	// ��I��
		Select,		// �I��
		StageSelect,// �X�e�[�W�I��
	};

	// �Q�[�����[�h
	enum class GamaMode
	{
		Game,		// �Q�[��
		Tutorial	// �`���[�g���A��
	};

	// �X�e�[�W���[�h
	enum class StageMode
	{
		Stage1,		// �n��G
		Stage2,	    // �󒆓G
		Stage3      // �n��G + �󒆓G
	};

	// �s�N�Z���V�F�[�_�[���
	enum PixelShaderKind
	{
		PostEffect,		// �|�X�g�G�t�F�N�g
		GaussianBlur,	// �K�E�V�A���u���[
		RadiationBlur,	// ���˃u���[
		JitterDriftPS,	// �W�b�^�[�h���t�g

		PSMax
	};

	// �T�u�t���[���o�b�t�@���
	enum SubFrameBufferKind
	{
		Luminance,		// �P�x���o
		Bloom,			// �u���[��
		Synthesis,		// ����
		JitterDriftSFB,	// �W�b�^�[�h���t�g

		SFBMax
	};

	// �X�v���C�g���
	enum SpriteKind
	{
		Title,			// �^�C�g��
		DecisionText,	// ����
		Back,			// �w�i
		GameStartText,	// �Q�[��
		TutorialText,	// �`���[�g���A��
		Stage1,			// �X�e�[�W1
		Stage2,			// �X�e�[�W2
		Stage3,			// �X�e�[�W3
        StageTextFrame,	// �X�e�[�W�g�g
		StageBack,		// �X�e�[�W�w�i
		CyberCircle,	// �T�C�o�[�~

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
	std::unique_ptr<Base>							tower = nullptr;								// �^���[
	std::unique_ptr<CameraController>				cameraController = nullptr;						// �J�����R���g���[���[
	float											shadowDrawRect = 10.0f;							// �V���h�E�}�b�v�ɕ`�悷��͈�
	float											titleDissolveTimer = 0;							// �^�C�g�����O&����e�N�X�`���̃f�B�]���u����
	float											textDissolveTimer = 0;							// �Q�[��&�`���[�g���A���e�N�X�`���̃f�B�]���u����
	float											skyboxTimer = 0.0f;								// �X�J�C�{�b�N�X�o�ߎ���
	float											punchPosition = 0.0f;							// ����ɂ��ʒu����
	float											punchScale = 1.0f;								// ����ɂ��T�C�Y����
	float											lerpScale = 0.0f;								// �Q�[��&�`���[�g���A���e�N�X�`���̃T�C�Y����
	float											punchRotate = 0.0f;								// ����ɂ���]�l
	float											waitPunchTimer = 0.0f;							// �f�B�]���u����܂ł̒x������
	float											cosRadian = 0.0f;								// �Q�[��&�`���[�g���A���e�N�X�`������]������p�x
	float											skyboxColor = 0.0f;								// �X�J�C�{�b�N�X�̐F����
	float											deltaTime = 0.0f;								// Render�Ŏg���������x
	float											selectTextureScale[3] = { 0.5f, 0.5f, 0.5f };	// �X�e�[�W���̐��`�⊮�T�C�Y
	float											stageTexturePositionY[3] = { 0.0f, 0.0f, 0.0f };// �X�e�[�W���̃e�N�X�`����Y�ʒu
	float											selectFrameTextureScale[3] = { 0.5f, 0.5f, 0.5f };// �X�e�[�W�e�N�X�`���̘g�̐��`�⊮�T�C�Y(���g�p)
	float											rotateTimer = 0.0f;								// �T�C�o�[�T�[�N���e�N�X�`���̉�]����
	float											cyberRotate = 0.0f;								// �Q�[�����[�h�I�𒆂̃T�C�o�[�T�[�N���e�N�X�`���̐��`�⊮��]
	float											cyberAlpha = 0.0f;								// �T�C�o�[�T�[�N���e�N�X�`���̃A���t�@�l
	int												progressTimer = 0;								// �o�ߎ���
	int												titleMode = 0;									// �^�C�g�����[�h
	int												gameMode = 0;									// �Q�[�����[�h
	int												stageMode = 0;									// �X�e�[�W���[�h
	int                                             stagNo = 0;										// ���݂̃X�e�[�W
	// �W�b�^�[�h���t�g�萔�o�b�t�@
	struct JitterDriftConstantBuffer
	{
		float jitterStrength = 0.0f;
		float time = 0.0f;
		DirectX::XMFLOAT2 dummy;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> jitterDriftConstantBuffer;	// �W�b�^�[�h���t�g�萔�o�b�t�@
	JitterDriftData jitterDriftData;
};