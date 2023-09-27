#pragma once
#include <memory>
#include <d3d11.h>
#include <sstream>
#include <wrl.h>

#include <mutex>
#include <DirectXMath.h>

#include "Shader.h"
#include "DebugRenderer.h"
#include "ImGuiRenderer.h"
#include "DefaultSpriteShader.h"
#include "SwordTrailShader.h"
#include "LuminanceExtractionShader.h"
#include "ParticleShader.h"
#include "ParticleSpriteShader.h"
#include "Calculation.h"
#include "EffectTexAll.h"

CONST LONG SCREEN_WIDTH{ 1280 };
CONST LONG SCREEN_HEIGHT{ 720 };


//�T���v���[�X�e�[�g�擾
// 0   �|�C���g�t�B���^
// 1   ���j�A�t�B���^
// 2   �A�j�\�g���s�b�N
enum  SamplerState
{
	Point,
	Linear,
	Astropic
};
//     �[�x�e�X�g�@�@�[�x��������
//  0     ON           ON
//  1     ON           OFF
//  2     OFF          ON
//  3     OFF          OFF
enum  DepthStencilState
{

	ONON,
	ONOFF,
	OFFON,
	OFFOFF
};
//�u�����h�X�e�[�g
// 0   ����
// 1   �A���t�@
// 2   ���Z
// 3   ��Z
enum  BlendState
{

	Invalid,
	Alpha,
	Add,
	Multy
};
//���X�^���C�U�X�e�[�g
//      �`��@�@�@  �@�@�@�@�@�@�\��
// 0   �\���b�h�@�@�@�@�@�@�@���ʔ�\��
// 1   ���C���[�t���[���@�@�@���ʕ\��
// 2   �\���b�h�@�@�@�@�@�@�@���ʕ\��
enum RasterizerState
{

	SolidBack,
	WireF,
	Solid
};


class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();

	std::mutex rock;

	// �C���X�^���X�擾
	static Graphics& Instance() { 


		return *instance; 
	
	}
	enum  ModelShaderId
	{
		ModelShaderDefault,
		Toon,
		ShadowmapCaster,

		ModelShaderMax

	};

	enum  SpriteShaderId
	{
		SpriteShaderDefault,
		Skybox,
		LuminanceExtraction,
		ParticleSprite,
		InstancingSprite,

		SpriteShaderMAX
	};

	enum  DissolveSpriteId
	{
		Dissolve,
		DissolveCircle,
		DissolveDiagonal,

		DissolveSpriteMAX
	};

	// �f�o�C�X�擾
	ID3D11Device* GetDevice() const { return device.Get(); }

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* GetDeviceContext() const { return immediateContext.Get(); }

	// �X���b�v�`�F�[���擾
	IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }

	// �����_�[�^�[�Q�b�g�r���[�擾
	ID3D11RenderTargetView* GetRenderTargetView() const { return renderTargetView.Get(); }

	// �f�v�X�X�e���V���r���[�擾
	ID3D11DepthStencilView* GetDepthStencilView() const { return depthStencilView.Get(); }

	//�T���v���[�X�e�[�g�擾
	
	ID3D11SamplerState* GetSamplerState(int i) const { return samplerStates[i].Get(); }


	//�[�x�e�X�g�A�[�x��������
	
	ID3D11DepthStencilState* GetDepthStencilState(int i) const { return depthStencilStates[i].Get(); }


	//�u�����h�X�e�[�g
	
	ID3D11BlendState* GetBlendState(int i) const { return blendStates[i].Get(); }

	
	//���X�^���C�U�X�e�[�g
	
	ID3D11RasterizerState* GetRasterizerState(int i) const { return rasterizerStates[i].Get(); }

	
	//�萔�o�b�t�@
	ID3D11Buffer* GetConstant_buffers(int i) const { return constantBuffers[i].Get(); }

	
	DirectX::XMFLOAT4X4 test;

	// �萔�o�b�t�@�t�H�[�}�b�g
	struct scene_constants
	{
		DirectX::XMFLOAT4X4 view_projection;	// �r���[�E�v���W�F�N�V�����ϊ��s��
		DirectX::XMFLOAT4 light_direction;		// ���C�g�̌���
		DirectX::XMFLOAT4 camera_position;  //�J�����ʒu
	};


	// �萔�o�b�t�@�I�u�W�F�N�g
	//Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers[8];
	

	// �V�F�[�_�[�擾
	//Shader* GetShader() const { return shader.get(); }

	// �X�N���[�����擾
	float GetScreenWidth() const { return screenWidth; }

	// �X�N���[�������擾
	float GetScreenHeight() const { return screenHeight; }

	// �f�o�b�O�����_���擾
	DebugRenderer* GetDebugRenderer() const { return debugRenderer.get(); }

	// ���C�������_���擾
	//LineRenderer* GetLineRenderer() const { return lineRenderer.get(); }

	// ImGui�����_���擾
	ImGuiRenderer* GetImGuiRenderer() const { return imguiRenderer.get(); }

	SwordTrailShader* GetSwordTrailShader() const { return swordtrailShader.get(); }

	ParticleShader* GetParticleShader() const { return particleShader.get(); }

	//InstancingModelShader* GetInstancingModelShader() const { return instancingModelShader.get(); }

	//�~���[�e�b�N�X
	std::mutex& GetMutex() { return mutex; }
	ModelShader* GetShader(ModelShaderId id) const { return shader[static_cast<int>(id)].get(); }
	SpriteShader* GetShader(SpriteShaderId id) const { return spriteShaders[static_cast<int>(id)].get();}

	Sprite* GetDissolveSprite(DissolveSpriteId id) const { return dissolveSprite[static_cast<int>(id)].get(); }

private:
	static Graphics* instance;

	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		immediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>      samplerStates[3];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[4];
	Microsoft::WRL::ComPtr<ID3D11BlendState>        blendStates[4];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>   rasterizerStates[3];
	Microsoft::WRL::ComPtr<ID3D11Buffer>            constantBuffers[8];

	std::unique_ptr<ModelShader>					shader[static_cast<int>(ModelShaderId::ModelShaderMax)];
	std::unique_ptr<SpriteShader>					spriteShaders[static_cast<int>(SpriteShaderId::SpriteShaderMAX)];
	std::unique_ptr<SwordTrailShader>				swordtrailShader;
	std::unique_ptr<ParticleShader>				    particleShader;
	//std::unique_ptr<InstancingModelShader>          instancingModelShader;
	std::unique_ptr<DebugRenderer>					debugRenderer;
	//std::unique_ptr<LineRenderer>					    lineRenderer;
	std::unique_ptr<ImGuiRenderer>					imguiRenderer;
	std::mutex										mutex;
	std::shared_ptr<Sprite> dissolveSprite[static_cast<int>(DissolveSpriteId::DissolveSpriteMAX)];
	float	screenWidth;
	float	screenHeight;
	//std::mutex      mutex;
};