#pragma once

#include<d3d11.h>

//class texture
//{
//
//public:
//	//���_�t�H�[�}�b�g
//	
HRESULT LoadTextureFromFile(ID3D11Device* device, const wchar_t* filename,
		ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc);
HRESULT LoadTextureFromFile2(ID3D11Device* device, const char* filename,
	ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc);
void ReleaseAllTextures();

//};

///<summary>
///�_�~�[�̃e�N�X�`�����쐬
///</summary>
///<param name = "device">�f�o�C�X</param>
///<param name ="shader_resource_view">�������ꂽ�e�N�X�`��</param>
///<param name ="value">�F</param>
///<param name ="dimension">�T�C�Y</param>
///<returns></returns>
HRESULT MakeDummyTexture(
	ID3D11Device* device,
	ID3D11ShaderResourceView** shader_resource_view,
	DWORD value,
	UINT dimension);