#include "StaticMesh.h"
#include "Shader.h"
#include "misc.h"
#include <vector>
#include <fstream>
#include <filesystem>
#include "texture.h"


using namespace DirectX;

StaticMesh::StaticMesh(ID3D11Device* device, const wchar_t* obj_filename, bool flipping_v_coordinates)
{
    std::vector<vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t current_index{ 0 };

    std::vector<XMFLOAT3> positions;
    std::vector<XMFLOAT3> normals;
    std::vector<XMFLOAT2> texcoords;
    std::vector<std::wstring> mtl_filenames;

    std::wifstream fin(obj_filename);
    _ASSERT_EXPR(fin, L"'OBJ file not found.");


    // �s�̏��
    wchar_t command[256];
    while (fin)
    {
        // �P�ԏ�ɂ���P�s���擾
        fin >> command;

        if (0 == wcscmp(command, L"v"))
        {
            // ���_���W���擾
            float x, y, z;
            // ��؂�P�ʂŃf�[�^���擾���Ă���
            fin >> x >> y >> z;
            positions.push_back({ x, y, z });

            // �P�ԏ�ɂ���P�s���폜
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vn"))
        {
            // �@�����擾
            float i, j, k;
            fin >> i >> j >> k;
            normals.push_back({ i, j, k });

            // �P�ԏ�ɂ���P�s���폜
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"vt"))
        {
            float u, v;
            fin >> u >> v;
            texcoords.push_back({ u, flipping_v_coordinates ? 1.0f - v : v });
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"f"))
        {
            for (size_t i = 0; i < 3; i++)
            {
                vertex vertex;
                size_t v, vt, vn;

                // �P�ԏ�ɂ���P�s�ڂ̂P�ڂ̃f�[�^���擾��
                // �f�[�^�̂����̂P�ڂ̐��l���擾
                fin >> v;
                vertex.position = positions.at(v - 1);
                if (L'/' == fin.peek())
                {
                    // �P�����폜
                    fin.ignore(1);
                    if (L'/' != fin.peek())
                    {
                        // �X���b�V����؂�Ŏ��̐��l���擾
                        fin >> vt;
                        vertex.texcoord = texcoords.at(vt - 1);
                    }
                    if (L'/' == fin.peek())
                    {
                        // �P�����폜
                        fin.ignore(1);
                        // �X���b�V����؂�Ŏ��̐��l���擾
                        fin >> vn;
                        vertex.normal = normals.at(vn - 1);
                    }
                }

                // ���_�f�[�^��ݒ�
                vertices.push_back(vertex);

                // ���_�C���f�b�N�X��ݒ�
                indices.push_back(current_index++);
            }

            // �P�ԏ�ɂ���P�s���폜
            fin.ignore(1024, L'\n');
        }
        else if(0 == wcscmp(command, L"mtllib"))
        {
            //�}�e���A���t�@�C�������擾
            wchar_t mtllib[256];
            fin >> mtllib;
            mtl_filenames.push_back(mtllib);
        }
        else if (0 == wcscmp(command, L"usemtl"))
        {
            // �}�e���A����
            wchar_t usemtl[MAX_PATH]{ 0 };
            fin >> usemtl;
            
            // �}�e���A�����ƃC���f�b�N�X�J�n�ԍ���ۑ�
            subsets.push_back({ usemtl, static_cast<uint32_t>(indices.size()),0 });
       
        }
        else
        {
            // �P�ԏ�ɂ���P�s���폜
            fin.ignore(1024, L'\n');
        }
    }
    fin.close();

    // ���ꂼ��̃T�u�Z�b�g�̃C���f�b�N�X�̐����v�Z���Ă���
    std::vector<subset>::reverse_iterator iterator = subsets.rbegin();
    iterator->indexCount = static_cast<uint32_t>(indices.size()) - iterator->indexStart;
    for (iterator = subsets.rbegin() + 1; iterator != subsets.rend(); ++iterator)
    {
        iterator->indexCount = (iterator - 1)->indexStart - iterator->indexStart;
    }
    // MTL�@�t�@�C�����AOBJ�t�@�C������ݒ�
    std::filesystem::path mtl_filename(obj_filename);

    //�t�@�C���������̂�MTL�t�@�C�����ɓ���ւ���
    mtl_filename.replace_filename(std::filesystem::path(mtl_filenames[0]).filename());

    //�}�e���A���t�@�C�����J��
    fin.open(mtl_filename);
    //_ASSERT_EXPR(fin, L"'MTL file not found.");



    while (fin)
    {
        //��ԏ�ɂ���P�s���擾
        fin >> command;

        if (0 == wcscmp(command, L"map_Kd"))
        {
            fin.ignore();

            //�e�N�j�b�N�t�@�C����ǂݍ���
            wchar_t map_kd[256];
            fin >> map_kd;

            //�e�N�X�`���t�@�C�����Ƀp�X�����t����
            std::filesystem::path path(obj_filename);
            path.replace_filename(std::filesystem::path(map_kd).filename());
            materials.rbegin()->textureFilenames[0] = path;

            fin.ignore(1024, L'\n');
        }
        else if(0 == wcscmp(command, L"map_bump") || 0 == wcscmp(command, L"bump"))
        {
            fin.ignore();

            //�o���v�}�b�v�p�̃e�N�X�`���t�@�C�������擾
            wchar_t map_bump[256];
            fin >> map_bump;
            std::filesystem::path path(obj_filename);
            path.replace_filename(std::filesystem::path(map_bump).filename());
            materials.rbegin()->textureFilenames[1] = path;
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"newmtl"))
        {
            fin.ignore();
            wchar_t newmtl[256];
            // �V�K�Ń}�e���A�����쐬
            material mat;
            fin >> newmtl;
            mat.name = newmtl;
            // �}�e���A�������t��
            materials.push_back(mat);
        
        }
        else if (0 == wcscmp(command, L"Kd"))
        {
            // �F���擾�����������}�e���A���ɐݒ�
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->Kd = { r, g, b, 1 };
            fin.ignore(1024, L'\n');
            
        }
        else
        {
            fin.ignore(1024, L'\n');
        }

        
    }

    CreateComBuffers(device, vertices.data(), vertices.size(), indices.data(), indices.size());

    HRESULT hr{ S_OK };

    // ���̓��C�A�E�g�I�u�W�F�N�g�̐ݒ�
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
            D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
            D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    // ���_�V�F�[�_�[�I�u�W�F�N�g�̍쐬
    create_vs_from_cso(device, "static_mesh_vs.cso", vertexShader.GetAddressOf(),
        inputLayout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
    // �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�̍쐬
    create_ps_from_cso(device, "static_mesh_ps.cso", pixelShader.GetAddressOf());

    // �萔�o�b�t�@�̍쐬
    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&buffer_desc, nullptr, constantBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    
    if (materials.size() == 0)
    {
        for (const subset& s : subsets)
        {
            materials.push_back({ s.usemtl });
        }
    }
    
    // �ǂݍ��񂾃e�N�X�`���𐶐�����
    D3D11_TEXTURE2D_DESC texture2d_desc{};


    //load_texture_from_file(device, texture_filename.c_str(), shader_resource_view.GetAddressOf(), &texture2d_desc);
    
#if false
    for (material & mat : materials)
    {
        if (mat.textureFilenames[0].size() > 0) {
            load_texture_from_file(device, mat.textureFilenames[0].c_str(),
                mat.shader_resource_views[0].GetAddressOf(), &texture2d_desc);
        }
        if (mat.textureFilenames[1].size() > 0) {
            load_texture_from_file(device, mat.textureFilenames[1].c_str(),
                mat.shader_resource_views[1].GetAddressOf(), &texture2d_desc);
        }
    }
#else
    for (material& mat : materials)
    {
        if (mat.textureFilenames[0].size() > 0) {
            LoadTextureFromFile(device, mat.textureFilenames[0].c_str(),
                mat.shader_resource_views[0].GetAddressOf(), &texture2d_desc);
        }
        else
        {
            //�_�~�[�p�̃J���[�}�b�v�e�N�X�`���𐶐����ݒ肷��
            MakeDummyTexture(device, mat.shader_resource_views[0].GetAddressOf(),
                0xFFFFFFFF, 16);
        }
        if (mat.textureFilenames[1].size() > 0) {
            LoadTextureFromFile(device, mat.textureFilenames[1].c_str(),
                mat.shader_resource_views[1].GetAddressOf(), &texture2d_desc);
        }
        else
        {
            // �_�~�[�p�̖@���}�b�v�e�N�X�`���𐶐����ݒ肷��
                MakeDummyTexture(device, mat.shader_resource_views[1].GetAddressOf(),
                    0xFFFF7F7F, 16);
        }
    }
#endif
    // �ǂݍ��񂾒��_���W�� x y z �̍ŏ��A�ő�����ꂼ��o�E���f�B���O�{�b�N�X�ɐݒ肷��
    for (const vertex& v : vertices)
     {
      boundingBox[0].x = std::min<float>(boundingBox[0].x, v.position.x);
      boundingBox[0].y = std::min<float>(boundingBox[0].y, v.position.y);
      boundingBox[0].z = std::min<float>(boundingBox[0].z, v.position.z);
      boundingBox[1].x = std::max<float>(boundingBox[1].x, v.position.x);
      boundingBox[1].y = std::max<float>(boundingBox[1].y, v.position.y);
      boundingBox[1].z = std::max<float>(boundingBox[1].z, v.position.z);
        
    }
    
}

void StaticMesh::Render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world,
    const DirectX::XMFLOAT4& materialColor,
    ID3D11PixelShader* alternative_pixel_shader)
{
    uint32_t stride{ sizeof(vertex) };
    uint32_t offset{ 0 };
    immediate_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    // �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�̐ݒ�
    immediate_context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediate_context->IASetInputLayout(inputLayout.Get());

    immediate_context->VSSetShader(vertexShader.Get(), nullptr, 0);
    // �ؑ֗p�̃s�N�Z���V�F�[�_�[���������炻�����ݒ�
    alternative_pixel_shader ? immediate_context->PSSetShader(alternative_pixel_shader, nullptr, 0) : immediate_context->PSSetShader(pixelShader.Get(), nullptr, 0);

    //// �e�N�X�`���̏����V�F�[�_�[���ɐݒ�
    //immediate_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());
    //
    //
    //// �萔�o�b�t�@�Ƃ��āA���[���h�s��ƃ}�e���A���J���[��ݒ�
    //constants data{ world, materialColor };
    //immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
    //// �萔(�R���X�^���g)�o�b�t�@�I�u�W�F�N�g�̐ݒ�
    //immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
    //
    //// �C���f�b�N�X���w�肵�ĕ`��
    //D3D11_BUFFER_DESC buffer_desc{};
    //index_buffer->GetDesc(&buffer_desc);
    //immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
    // �}�e���A���z��(�x�N�^�[)�ɂ���}�e���A�������ׂĕ`��
    for (const material& mat : materials)
    {
        // �e�N�X�`���̏����V�F�[�_�[���ɐݒ�
        immediate_context->PSSetShaderResources(0, 1, mat.shader_resource_views[0].GetAddressOf());
        // �o���v�}�b�v�p�̃e�N�X�`�����V�F�[�_�[���ɐݒ�
        immediate_context->PSSetShaderResources(1, 1, mat.shader_resource_views[1].GetAddressOf());
        // �萔�o�b�t�@�Ƃ��āA���[���h�s��ƃ}�e���A���J���[��ݒ�
        constants data{ world, materialColor };
        // �}�e���A���J���[�͓ǂݍ��񂾐F�����f
        XMStoreFloat4(&data.materialColor, XMLoadFloat4(&materialColor) * XMLoadFloat4(&mat.Kd));
        immediate_context->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);
        // �萔(�R���X�^���g)�o�b�t�@�I�u�W�F�N�g�̐ݒ�
        immediate_context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

        // �T�u�Z�b�g��{��
        for (const subset& s : subsets)
        {
            // �T�u�Z�b�g�ɂ���}�e���A�����ƈ�v������̂��`�F�b�N
            if (mat.name == s.usemtl)
            {
                // ��v�����T�u�Z�b�g�̃C���f�b�N�X�̐��ƊJ�n�ԍ������w��
                immediate_context->DrawIndexed(s.indexCount, s.indexStart, 0);
            }
        }
    }
}


/// <summary>
/// ���_�o�b�t�@�I�u�W�F�N�g�̍쐬
/// </summary>
/// <param name="device"></param>
/// <param name="vertices">���_���</param>
/// <param name="vertex_count">���_��</param>
/// <param name="indices">�C���f�b�N�X���</param>
/// <param name="index_count">�C���f�b�N�X��</param>
void StaticMesh::CreateComBuffers(ID3D11Device* device, vertex* vertices, size_t vertex_count,
    uint32_t* indices, size_t index_count)
{
    HRESULT hr{ S_OK };

    D3D11_BUFFER_DESC buffer_desc{};
    D3D11_SUBRESOURCE_DATA subresource_data{};
    // ���_���W���̃T�C�Y���v�Z
    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertex) * vertex_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    // ���_���W���ݒ肳��Ă���z���ݒ�
    subresource_data.pSysMem = vertices;
    subresource_data.SysMemPitch = 0;
    subresource_data.SysMemSlicePitch = 0;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertexBuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    subresource_data.pSysMem = indices;
    hr = device->CreateBuffer(&buffer_desc, &subresource_data, indexBuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}