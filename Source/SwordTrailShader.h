#pragma once

#include <memory>
#include <wrl.h>
#include "Shader.h"
#include"SwordTrail.h"

class SwordTrailShader
{
public:
	SwordTrailShader(ID3D11Device* device);
	~SwordTrailShader() {}

	void Begin(const RenderContext& rc);
	void Draw(const RenderContext& rc, const SwordTrail* sprite, DirectX::XMFLOAT4 color) ;
	void End(const RenderContext& rc);

private:
	struct constants
	{
		DirectX::XMFLOAT4 materialColor;	// �}�e���A���J���[
	};
	struct scene_constants
	{
		DirectX::XMFLOAT4X4 view_projection;	// �r���[�E�v���W�F�N�V�����ϊ��s��
		DirectX::XMFLOAT4 light_direction;		// ���C�g�̌���
		DirectX::XMFLOAT4 camera_position;  //�J�����ʒu
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>				constantBuffer;


private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;


	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;


};