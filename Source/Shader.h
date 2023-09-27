#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Sprite.h"
#include "Model.h"
#include "SkinnedMesh.h"
#include "RenderContext.h"
#define MAX_STAGE 100
struct InstancingConstants
{
	DirectX::XMFLOAT4X4 world[MAX_STAGE];			// ���[���h�s��
	//DirectX::XMFLOAT4X4 boneTransforms[256];
};
//	�{�[���ő吔
static	constexpr	int	MaxBones = 128;

//	�C���X�^���V���O�ő吔
static	constexpr	int	InstancingMax = 512;

//	���b�V���y�уA�j���[�V�����֌W�̏��
struct BufferData
{
	const SkinnedMeshResouurce::Mesh& mesh;				//	���b�V�����
	DirectX::XMFLOAT4X4	boneTransforms[MaxBones];	//	�{�[���s��
	int	instancingCount = 0;						//	�C���X�^���V���O�`�搔
	DirectX::XMFLOAT4X4	transform[InstancingMax];	//	�p���s��

	BufferData(const SkinnedMeshResouurce::Mesh& mesh) : mesh(mesh) {}
};
struct UvConstants
{
	DirectX::XMFLOAT4 uvStatus;			// uv�A�j���[�V����
};


class ModelShader
{
public:
	enum InstanModel
	{
		Default,
		Instancing,
		Max
	};
	ModelShader() {}
	virtual ~ModelShader() {}

	// �`��J�n
	virtual void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) = 0;

	// �`��
	virtual void Draw(ID3D11DeviceContext* dc, const Model* model, std::vector<SkinnedMeshResouurce::constants> buffer, const DirectX::XMFLOAT4 uvstatus= { 0,0,0,0 }, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f }) = 0;

	virtual void Draw(ID3D11DeviceContext* dc, const Model* model, InstancingConstants buffer, int max, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f })  = 0;

	// �`��I��
	virtual void End(ID3D11DeviceContext* context) = 0;
	// �K�v�ȃo�b�t�@�̐ݒ�
	//virtual void SetBuffers(const RenderContext& rc, const BufferData& bufferData) = 0;
};

class SpriteShader
{
public:
	SpriteShader() {}
	virtual ~SpriteShader() {}

	// �`��J�n
	virtual void Begin(const RenderContext& rc) = 0;

	// �`��
	virtual void Draw(const RenderContext& rc, const Sprite* sprite = nullptr) = 0;

	// �`��I��
	virtual void End(const RenderContext& rc) = 0;
};


HRESULT create_vs_from_cso(
    ID3D11Device* device,
    const char* cso_name,
    ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout,
    D3D11_INPUT_ELEMENT_DESC* input_element_desc,
    UINT num_elements
);
HRESULT create_ps_from_cso(
    ID3D11Device* device,
    const char* cso_name,
    ID3D11PixelShader** pixel_shader
);
HRESULT create_gs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11GeometryShader** geometry_shader);
HRESULT create_cs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11ComputeShader** comp_shader);

           