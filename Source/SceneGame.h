#pragma once
#include"Player.h"
#include"Scene.h"
#include "Graphics.h"
#include "CameraController.h"
#include "Framebuffer.h"
#include "SubFramebuffer.h"
#include "FullscreenQuad.h"
#include "RenderContext.h"
#include "Shadowbuffer.h"
#include "StageMain.h"
#include "MetaAI.h"
#include "EffectAll.h"
#include "EffectTexAll.h"
#include "Base.h"

#include <ctime>
using namespace DirectX;
// �Q�[���V�[��
class SceneGame :public Scene
{
public:
	SceneGame() {}
	~SceneGame() override {}

	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render()override;



private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	////�l�X�ȕ`�施�߂�GPU�ɓ`������
	//ID3D11DeviceContext* immediate_context;
	////�L�����o�X�ɕ`��������z�i�E�B���h�E�j�ɂ������
	//IDXGISwapChain* swap_chain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>immediate_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain>swap_chain;
	//�F���������ރL�����o�X
	//ID3D11RenderTargetView* render_target_view;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>render_target_view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>depth_stencil_view;

	//���s�������������ރL�����o�X

	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[3];

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[3];

	
	//�����ւ��p�s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader>replaced_pixel_shader;

	std::unique_ptr<FullscreenQuad> bit_block_transfer;

	//�X�L�����b�V��
	//std::unique_ptr<SkinnedMesh> SkinnedMeshes[8];
	//�I�t�X�N���[���o�t�@
	std::unique_ptr<Framebuffer> framebuffers[2];
	std::unique_ptr<SubFramebuffer> subframebuffers[6];
	std::unique_ptr<Shadowbuffer> shadowbuffer;

	std::unique_ptr<Sprite> sprite_batchs;

	std::unique_ptr<Sprite> sprite_batchs2;

	std::unique_ptr<Sprite> claerSprite;

	std::unique_ptr<Sprite> waveSprite[3];

	std::shared_ptr<Sprite> loodSprite;

	std::unique_ptr<Sprite> renderSprite;

	//�����ւ��p�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> replaced_shader_resource_view;
private:
	void projectImgui();
private:
	std::unique_ptr<Player> player = nullptr;
	std::unique_ptr<Base> base = nullptr;
	//std::shared_ptr <CameraController> cameraController = nullptr;
	std::unique_ptr<Meta> meta = nullptr;
	static const int ShadowMapSize = 1096;
	StageMain* stageMain;
	// ���C�g�̕���
	DirectX::XMFLOAT4 light_direction{ 1.0f, -1.0f, 0.0f, 0.0f };

	float shadowDrawRect = 1.0f; // �V���h�E�}�b�v�ɕ`�悷��͈�
	DirectX::XMFLOAT4X4 lightViewProjection; // ���C�g�r���[�v���W�F�N�V�����s��

	LuminanceExtractionData luminanceExtractionData;

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
	DirectX::XMFLOAT3 objPos[3][3] = { {{ 50,-4,-50 },{60,-4,17},{130,-4,-80}},{{ 115,-4,-50 },{115,-4,-20},{115,-45,-80}},{{ 50,-4,-50 },{60,-4,17},{130,-4,-80}} }; //�I�u�W�F�N�g�ʒu
	float objAngleY[3][3] = { { 0,90,-45 },{ 90,90,90 },{ 0,90,-45 }};//�I�u�W�F�N�g��]
	float objAngleZ[3][3] = { { 0,0,0 },{ 0,0,90 },{ 0,0,0 }};//�I�u�W�F�N�g��]

};