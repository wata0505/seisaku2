#pragma once
#include"Scene.h"
#include "Graphics.h"
#include "CameraController.h"
#include "Framebuffer.h"
#include "FullscreenQuad.h"
#include "RenderContext.h"
#include "Shadowbuffer.h"
#include "StageMain.h"
#include "MetaAI.h"
#include "EffectAll.h"
#include "SubFramebuffer.h"
#include "AudioSource.h"
#include"AudioAll.h"
#include <ctime>
using namespace DirectX;
// �Q�[���V�[��
class SceneTitle :public Scene
{
public:
	enum class GamaMode
	{
		Default,
		Tutorial
	};
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
	std::unique_ptr<SubFramebuffer> subframebuffers[3];
	std::unique_ptr<Shadowbuffer> shadowbuffer;

	std::shared_ptr<Sprite> spriteBatchs[7];

	std::shared_ptr<Sprite> renderSprite;

	//�����ւ��p�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> replaced_shader_resource_view;
private:
	void projectImgui();
private:
	std::unique_ptr <CameraController> cameraController = nullptr;
	static const int ShadowMapSize = 2096;
	StageMain* stageMain;
	// ���C�g�̕���
	DirectX::XMFLOAT4 light_direction{ 1.0f, -1.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 shadowlight_direction{ 1.0f, -1.0f, 0.0f, 0.0f };
	float light = 300;
	float shadowDrawRect = 330.0f; // �V���h�E�}�b�v�ɕ`�悷��͈�
	DirectX::XMFLOAT4X4 lightViewProjection; // ���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT3 position = { 0,0,0 };

	//Sprite�`��p

	EffectAll* effectAll = nullptr;
	int timer = 0;
	int titleMode = 0;
	int gameMode = 0;
	float dissolveTimer = 0;
	float dissolveDTimer = 0;
	EffectTexAll* effectTexAll = nullptr;
	//std::unique_ptr<AudioSource> titlebgm;
	AudioAll* audioAll = nullptr;
	LuminanceExtractionData luminanceExtractionData;
};