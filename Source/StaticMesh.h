#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
#include <string>
#include <vector>

/// <summary>
/// �X�^�e�B�b�N���b�V��
/// </summary>
class StaticMesh
{
public:
    /// <summary>
    /// ���_�t�H�[�}�b�g
    /// </summary>
    struct vertex
    {
        DirectX::XMFLOAT3 position;	// ���W
        DirectX::XMFLOAT3 normal;	// �@��
        DirectX::XMFLOAT2 texcoord; //�e�N�X�`�����W
    };

    /// <summary>
    /// �萔�o�b�t�@�p�t�H�[�}�b�g
    /// </summary>
    struct constants
    {
        DirectX::XMFLOAT4X4 world;			// ���[���h�s��
        DirectX::XMFLOAT4 materialColor;	// �}�e���A���J���[
    };

   
    
    ///<summary>
    ///�}�e���A������DrawIndex()�ɕK�v�ȏ�񕔕����T�u�Z�b�g��
    ///</summary>
    struct subset
    {
        std::wstring usemtl;       //�}�e���A����
        uint32_t indexStart{ 0 }; //�C���f�b�N�X�̊J�n�ʒu
        uint32_t indexCount{ 0 }; //�C���f�b�N�X�̐��i���_���j
    };
    std::vector<subset>subsets;
   

   // //�e�N�X�`���t�@�C����
   // std::wstring texture_filename;
   // //�e�N�X�`�����
   // Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;

    //�}�e���A��
    struct material
    {
        //�}�e���A����
        std::wstring name;
        DirectX::XMFLOAT4 Ka{ 0.2f,0.2f,0.2f,1.0f };
        DirectX::XMFLOAT4 Kd{ 0.8f,0.8f,0.8f,1.0f };
        DirectX::XMFLOAT4 Ks{ 1.0f,1.0f,1.0f,1.0f };
        ////�e�N�X�`���t�@�C����
        //std::wstring texture_filename;
        ////�e�N�X�`���̏��
        //Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
        //�e�N�X�`���t�@�C����
        std::wstring textureFilenames[2];
        //�e�N�X�`���̏��
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_views[2];
    };
    //�ǂݍ��񂾃}�e���A��
    std::vector<material>materials;

    DirectX::XMFLOAT3 boundingBox[2]{
        { D3D11_FLOAT32_MAX, D3D11_FLOAT32_MAX, D3D11_FLOAT32_MAX },
        { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX }
    };

private:
    // ���_�o�b�t�@�I�u�W�F�N�g
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    // �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

    // ���_�V�F�[�_�[�I�u�W�F�N�g
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    // �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    // ���̓��C�A�E�g�I�u�W�F�N�g
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    // �萔�o�b�t�@�I�u�W�F�N�g
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

public:
    StaticMesh(ID3D11Device* device, const wchar_t* obj_filename, bool flipping_v_coordinates);
    virtual ~StaticMesh() = default;


    /// <summary>
    /// �`�揈��
    /// </summary>
    /// <param name="immediate_context">�f�o�C�X�R���e�L�X�g</param>
    /// <param name="world">�e�̃��[���h�s��</param>
    /// <param name="materialColor">�}�e���A���J���[</param>
    /// /// <param name="alternative_pixel_shader">�ؑ֗p�s�N�Z���V�F�[�_�[</param>
    void Render(ID3D11DeviceContext* immediate_context,
        const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& materialColor,
        ID3D11PixelShader* alternative_pixel_shader = nullptr);

protected:
    /// <summary>
    /// ���_�o�b�t�@�I�u�W�F�N�g�̍쐬
    /// </summary>
    /// <param name="device"></param>
    /// <param name="vertices">���_���</param>
    /// <param name="vertex_count">���_��</param>
    /// <param name="indices">�C���f�b�N�X���</param>
    /// <param name="index_count">�C���f�b�N�X��</param>
    void CreateComBuffers(ID3D11Device* device, vertex* vertices, size_t vertex_count,
        uint32_t* indices, size_t index_count);
};