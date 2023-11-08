#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <cstdint>
#include "RenderContext.h"

class FullscreenQuad
{
public:
	FullscreenQuad(ID3D11Device *device);
	virtual ~FullscreenQuad() = default;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> embeddedVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> embeddedPixelShader;

	// ジッタードリフト用定数バッファ
	struct JitterDriftConstantBuffer
	{
		float jitterDriftStrength;
		float time;
		DirectX::XMFLOAT2 dummy;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> jitterDriftConstantBuffer;

public:
	void blit(ID3D11DeviceContext* immediate_contextbool, ID3D11ShaderResourceView** shader_resource_view,
		uint32_t start_slot, uint32_t num_views, ID3D11PixelShader* replaced_pixel_shader = nullptr);
	// ジッタードリフトシェーダー
	void JitterDrift(ID3D11DeviceContext* immediate_contextbool, ID3D11ShaderResourceView** shader_resource_view,
		uint32_t start_slot, uint32_t num_views, JitterDriftData jitterDriftData, ID3D11PixelShader* replaced_pixel_shader = nullptr);
};


