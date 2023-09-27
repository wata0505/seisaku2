#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
/// <summary>
/// �􉽃v���~�e�B�u
/// </summary>
class GeometricPrimitive
{
public:
    // ���_�t�H�[�}�b�g
    struct vertex
    {
        // �ʒu
        DirectX::XMFLOAT3 position;
        // �@��
        DirectX::XMFLOAT3 normal;
    };
    // �萔�o�b�t�@�p�t�H�[�}�b�g
    struct constants
    {
        // ���[���h�s��
        DirectX::XMFLOAT4X4 world;
        // �}�e���A���J���[
        DirectX::XMFLOAT4 materialColor;
    };

private:
    // ���_�o�b�t�@
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    // �C���f�b�N�X�o�b�t�@
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
  //  GeometricPrimitive(ID3D11Device* device);
    virtual ~GeometricPrimitive() = default;

    /// <summary>
    /// �`�揈��
    /// </summary>
    /// <param name="immediate_context">�f�o�C�X�R���e�L�X�g</param>
    /// <param name="world">���[���h�s��</param>
    /// <param name="materialColor">�}�e���A���J���[</param>
    void render(ID3D11DeviceContext* immediate_context,
        const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& materialColor);

protected:
    GeometricPrimitive(ID3D11Device* device);
    /// <summary>
    /// ���_�o�b�t�@�I�u�W�F�N�g�̍쐬
    /// </summary>
    /// <param name="device">�f�o�C�X</param>
    /// <param name="vertices">���_���</param>
    /// <param name="vertex_count">���_��</param>
    /// <param name="indices">�C���f�b�N�X���</param>
    /// <param name="index_count">�C���f�b�N�X��</param>
    void CreateComBuffer(ID3D11Device* device, vertex* vertices, size_t vertex_count,
        uint32_t* indices, size_t index_count);
};

/// <summary>
///�@�������̐�p�􉽃v���~�e�B�u
/// </summary>
class GeometricCube : public GeometricPrimitive
{
public:
    GeometricCube(ID3D11Device* device);

};

/// <summary>
/// �V�����_�[
/// </summary>
class GeometricCylinder : public GeometricPrimitive
{
public:
    // ��Q�����̓V�����_�[�̉~�����������邩�̐����w��
    GeometricCylinder(ID3D11Device* device, uint32_t slices);
};
class GeometricCapsule : public GeometricPrimitive
{
public:
    GeometricCapsule(
        ID3D11Device* device,
        float mantle_height,
        const DirectX::XMFLOAT3& radius,
        uint32_t slices,
        uint32_t ellipsoid_stacks,
        uint32_t mantle_stacks);
};