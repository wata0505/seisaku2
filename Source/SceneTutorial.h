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
// �Q�[���V�[��
class SceneTutorial :public Scene
{
public:
	SceneTutorial() {}
	~SceneTutorial() override {}

	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render()override;

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>render_target_view;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[3];

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[3];

	std::unique_ptr<FullscreenQuad> bit_block_transfer;

	//�I�t�X�N���[���o�t�@
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
	float shadowDrawRect = 10.0f; // �V���h�E�}�b�v�ɕ`�悷��͈�
	float dissolveTimer = 0;
	float haikeiTimer = 0;

	float waveTimer = 3;

	float gameTimer = 0;

	// �W�b�^�[�h���t�g�萔�o�b�t�@
	struct JitterDriftConstantBuffer
	{
		float jitterStrength = 0.0f;
		float time = 0.0f;
		DirectX::XMFLOAT2 dummy;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> jitterDriftConstantBuffer;	// �W�b�^�[�h���t�g�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11PixelShader> jitterDriftPixelShader;
	std::unique_ptr<SubFramebuffer> jitterDriftSubFramebuffer;
	JitterDriftData jitterDriftData;
	float skyboxColor = 0.0f;
	DirectX::XMFLOAT3 objPos[3][3] = { {{ 50,-4,-50 },{60,-4,17},{140,-4,-80}},{{ 115,-4,-50 },{115,-4,-20},{115,-48,-80}},{{80,-4,-30 },{120,-4,-30},{100,-48,-80}} }; //�I�u�W�F�N�g�ʒu
	float objAngleY[3][3] = { { 0,90,-45 },{ 90,90,90 },{ 10,-10,90 } };//�I�u�W�F�N�g��]
	float objAngleZ[3][3] = { { 0,0,0 },{ 0,0,90 },{ 0,0,90 } };//�I�u�W�F�N�g��]
	float gameAfterTimer = 0.0f;
	float gameAfterDissolveTimer = 3.0f;

private:
	//�`���[�g���A���p�ϐ�
	bool isUpdateStoper = false;

	enum TutorialState
	{
		STATE_ATTACK,
		STATE_OBJ_SETTING,
		STATE_RLUE_CHECK,
		STATE_BREAK_TIME,
		STATE_START,
	};
	//���݂̃X�e�[�g
	int tutorialState = TutorialState::STATE_START;
	//���̃X�e�[�g�֌������^�C�}�[
	float stateTimer = 3.0f;
	float initTimer = 3.0f;
	//����(���㑝����\�肠��)
	static const int checkMAX = 6;
	static const int stateMAX = 4;
	bool isTutorialCheck[checkMAX] = {};
	int checkCount = 0;
	//�����\������ׂ̃^�C�}�[
	float highlightTimer[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//�Z�b�g���[�h�t���O
	bool isSetMode = false;
	//�^���b�g�Z�b�g�����t���O
	int taretSetumeiState = 0;
	int pushCount = 0;

	int startUi = 0;

	//�R���g���[���[��pad���q���ł��邩�ŕ\������摜��ύX
	int keyOrButton = 0;

	//�Y����
	enum CheckList
	{
		CHECK_ATTACK,
		CHECK_JUMP,
		CHECK_OBJ_SELECT,
		CHECK_OBJ_SET_MODE,
		CHECK_OBJ_SET,
		CHECK_RULE
	};

	//�����\������ׂ̓Y����
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

	//�Z�߂ăA�b�v�f�[�g
	void TutorialUpdate(float elapsedTime);
	//�e��`�F�b�N���ڂ̃A�b�v�f�[�g
	void TutorialAttackUpdate(float elapsedTime);
	void TutorialObjSettingUpdate(float elapsedTime);
	void TutorialRuleCheckUpdate(float elapsedTime);
	void TutorialBreakTimeUpdate(float elapsedTime);
	void TutorialStartUpdate(float elapsedTime);
	//UI���`�F���W����֐�
	void ChangeUi();

	//�\��UI
	std::unique_ptr<Sprite> sprTutorialState[2] = {};	//��{����UI�\��
	std::unique_ptr<Sprite> sprUiHighlight[4] = {};		//���ݍs�����s���̋����\��
	std::unique_ptr<Sprite> sprOperationUi[2][2] = {};	//������@UI [�t�F�[�Y��][�{�^�����L�[��]
	std::unique_ptr<Sprite> sprObjectSet[2][2] = {};	//�I�u�W�F�N�g�ݒu�̐���
	std::unique_ptr<Sprite> sprTutorialStart[2] = {};	//�`���[�g���A���X�^�[�g�̈ꖇ�G
	std::unique_ptr<Sprite> sprRule[2] = {};	//���[���̈ꖇ�G

	//�����p�ϐ�
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

	//imgui�p
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