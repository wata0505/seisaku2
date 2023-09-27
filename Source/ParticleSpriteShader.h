#pragma once
#include <memory>
#include <wrl.h>
#include "Shader.h"
#include "ParticleSpriteManager.h"
#define MAX_INSTANCES2 2000
#define INPUT  0
#define OUTPUT 1




class ParticleSpriteShader : public SpriteShader
{
private:
	HRESULT CreateResource();//	���\�[�X�̍쐬(�o�b�t�@�m��)
	HRESULT ComputeShader(const RenderContext& rc, ParticleSprite* particleSprite);//	�R���s���[�g �V�F�[�_���g�������Z

	void Update(const RenderContext& rc, ParticleSprite* particleSprite);
public:
	ParticleSpriteShader(ID3D11Device* device);
	~ParticleSpriteShader(){}

	void Begin(const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Sprite* sprite = nullptr) override;
	void End(const RenderContext& rc) override;


private:

	Microsoft::WRL::ComPtr<ID3D11Buffer> gPToCpuBuffer = NULL; // CPU�ւ̏������ݗp�o�b�t�@ ���\�[�X
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> gPToCpuUAV = NULL;// �A���I�[�_�[�h �A�N�Z�X �r���[
	Microsoft::WRL::ComPtr<ID3D11Buffer> gPCPUreadBackBuffer = NULL; // ���[�h�o�b�N�p�o�b�t�@ ���\�[�X

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader>     computeShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>    geometryShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;


	Microsoft::WRL::ComPtr<ID3D11SamplerState>      shadowMapSamplerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;

	DirectX::XMFLOAT4	materialColor = { 0.5,0.5,0.5,1 };
	std::vector<SkinnedMeshResouurce::constants> buffers;
	//�萔�o�b�V��
	Microsoft::WRL::ComPtr<ID3D11Buffer> sceneBuffers;

	Microsoft::WRL::ComPtr<ID3D11Buffer> colorBuffers;

	Microsoft::WRL::ComPtr<ID3D11Buffer> calculationBuffers;
	//�p�[�e�B�N���`
	std::shared_ptr<Sprite> moveSprite[ParticleSprite::ParticleMax];


	int chainA = 0;//�o�b�t�@�[�̐؂�ւ�
	int chainB = 1;//�o�b�t�@�[�̐؂�ւ�
public:
	struct ColorConstants
	{
		DirectX::XMFLOAT4 materialColor;      // �J���[

	};
	
	struct SceneConstants
	{
		DirectX::XMFLOAT4X4 view;       // �r���[�ϊ��s��
		DirectX::XMFLOAT4X4 projection; // �����ϊ��s��
		
		
	};
	// �萔�o�b�t�@�t�H�[�}�b�g
	struct CalculationConstants
	{
		DirectX::XMFLOAT3 pattern;	// �����W
		float dummy1;
		DirectX::XMFLOAT3 tail;     //�@����
		float max;                  //�p�[�e�B�N���ő�l
		DirectX::XMFLOAT4 uvStatus;
	};

};