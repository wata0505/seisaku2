#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>

class SpriteBatch
{
private:
	//���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	//�s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	//���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	//���_�f�[�^
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	//�V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	//�e�N�X�`��
	D3D11_TEXTURE2D_DESC texture2dDesc;
public:
	//���_�t�H�[�}�b�g
	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
	};

private:
	const size_t max_vertices;
	std::vector<vertex> vertices;
public:
	SpriteBatch(ID3D11Device* device, const wchar_t* filename, size_t max_sprites);
	virtual ~SpriteBatch();
	//�@�`��
	void Render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh, float r, float g, float b, float a, float angle/*degree*/);

	void Render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh, float r, float g, float b, float a, float angle/*degree*/, float sx, float sy, float sw, float sh);

	void Render(ID3D11DeviceContext* immediate_context, float dx, float dy, float dw, float dh);

	//void begin(ID3D11DeviceContext* immediate_context);

	void Begin(ID3D11DeviceContext* immediate_context,ID3D11PixelShader* replaced_pixel_shader = nullptr,ID3D11ShaderResourceView* replaced_shader_resource_view = nullptr);

	void End(ID3D11DeviceContext* immediate_constext);
private:
	static void rotate(float& x, float& y, float cx, float cy, float sin, float cos)
	{
		x -= cx;
		y -= cy;

		float tx{ x }, ty{ y };
		x = cos * tx + -sin * ty;
		y = sin * tx + cos * ty;

		x += cx;
		y += cy;

	}
};
