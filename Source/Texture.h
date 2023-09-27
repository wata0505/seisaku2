#pragma once

#include<d3d11.h>

//class texture
//{
//
//public:
//	//頂点フォーマット
//	
HRESULT LoadTextureFromFile(ID3D11Device* device, const wchar_t* filename,
		ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc);
HRESULT LoadTextureFromFile2(ID3D11Device* device, const char* filename,
	ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc);
void ReleaseAllTextures();

//};

///<summary>
///ダミーのテクスチャを作成
///</summary>
///<param name = "device">デバイス</param>
///<param name ="shader_resource_view">生成されたテクスチャ</param>
///<param name ="value">色</param>
///<param name ="dimension">サイズ</param>
///<returns></returns>
HRESULT MakeDummyTexture(
	ID3D11Device* device,
	ID3D11ShaderResourceView** shader_resource_view,
	DWORD value,
	UINT dimension);