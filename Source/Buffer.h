#pragma once

#include<d3d11.h>
#include <wrl.h>
#include <cstdint>
//バッファ生成
//{
//
//public:
//	//頂点フォーマット
//	
void MakeBuffer(ID3D11Device* device, uint32_t width, uint32_t height, Microsoft::WRL::ComPtr<ID3D11Texture2D>* TEX, Microsoft::WRL::ComPtr<ID3D11RenderTargetView>* RTV, Microsoft::WRL::ComPtr <ID3D11ShaderResourceView>* SRV, DXGI_FORMAT Format);

void DsvBuffer(ID3D11Device* device, uint32_t width, uint32_t height, Microsoft::WRL::ComPtr<ID3D11Texture2D>* DEP, Microsoft::WRL::ComPtr<ID3D11DepthStencilView>* DSV, Microsoft::WRL::ComPtr <ID3D11ShaderResourceView>* SRV);