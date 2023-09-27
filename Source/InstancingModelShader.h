#pragma once

//#include <memory>
//#include <wrl.h>
//#include "Shader.h"

//class InstancingModelShader
//{
//public:
//	InstancingModelShader(ID3D11Device* device);
//	~InstancingModelShader() {}
//
//	void Begin(ID3D11DeviceContext* dc, const RenderContext& rc);
//	void Draw(ID3D11DeviceContext* dc, const Model* model,StageConstants buffer,int max, const DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f });
//	void End(ID3D11DeviceContext* dc);
//
//
//private:
//	static const int MaxBones = 128;
//
//	static const int MAX_BONES{ 256 };
//	// �萔�o�b�t�@�t�H�[�}�b�g
//	
//	struct scene_constants
//	{
//		DirectX::XMFLOAT4X4 view_projection;	// �r���[�E�v���W�F�N�V�����ϊ��s��
//		DirectX::XMFLOAT4 light_direction;		// ���C�g�̌���
//		DirectX::XMFLOAT4 camera_position;  //�J�����ʒu
//
//	};
//	struct ShadowConstants
//	{
//		DirectX::XMFLOAT4X4 shadow_projection;
//	};
//	Microsoft::WRL::ComPtr<ID3D11Buffer> shadow_buffer;
//	Microsoft::WRL::ComPtr<ID3D11Buffer> scene_buffers;
//	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;
//	Microsoft::WRL::ComPtr<ID3D11Buffer> uv_buffer;
//
//	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
//	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
//	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;
//
//
//	Microsoft::WRL::ComPtr<ID3D11SamplerState>      shadowMapSamplerState;
//
//	DirectX::XMFLOAT4	materialColor = { 1,1,1,1 };
//};