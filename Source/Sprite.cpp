#include "Sprite.h"
#include "misc.h"
#include <sstream>
#include <functional>
#include <WICTextureLoader.h>

#include "Graphics.h"
#include "texture.h"
#include "Shader.h"

Sprite::Sprite(const wchar_t* filename)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	HRESULT hr = S_OK;

	// ���_�f�[�^�̒�`
	// 0           1
	// +-----------+
	// |           |
	// |           |
	// +-----------+
	// 2           3
	Vertex vertices[] = {
		{ DirectX::XMFLOAT3(-0.5, +0.5, 0), DirectX::XMFLOAT4(1, 1, 1, 1) },
		{ DirectX::XMFLOAT3(+0.5, +0.5, 0), DirectX::XMFLOAT4(1, 0, 0, 1) },
		{ DirectX::XMFLOAT3(-0.5, -0.5, 0), DirectX::XMFLOAT4(0, 1, 0, 1) },
		{ DirectX::XMFLOAT3(+0.5, -0.5, 0), DirectX::XMFLOAT4(0, 0, 1, 1) },
	};
	Vertex vertices2[] = {
		{ DirectX::XMFLOAT3(-0.5, +0.5, 0), DirectX::XMFLOAT4(1, 1, 1, 1) },
		{ DirectX::XMFLOAT3(+0.5, +0.5, 0), DirectX::XMFLOAT4(1, 0, 0, 1) },
		{ DirectX::XMFLOAT3(-0.5, -0.5, 0), DirectX::XMFLOAT4(0, 1, 0, 1) },
		
	};
	// �|���S����`�悷��ɂ�GPU�ɒ��_�f�[�^��V�F�[�_�[�Ȃǂ̃f�[�^��n���K�v������B
	// GPU�Ƀf�[�^��n���ɂ�ID3D11***�̃I�u�W�F�N�g����ăf�[�^��n���܂��B

	// ���_�o�b�t�@�̐���
	{
		// ���_�o�b�t�@���쐬���邽�߂̐ݒ�I�v�V����
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(vertices);	// �o�b�t�@�i�f�[�^���i�[������ꕨ�j�̃T�C�Y
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;	// UNIT.03
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��ăo�b�t�@���쐬����B
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// UNIT.03
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		// ���_�o�b�t�@�ɒ��_�f�[�^�����邽�߂̐ݒ�
		D3D11_SUBRESOURCE_DATA subresource_data = {};
		subresource_data.pSysMem = vertices;	// �����Ɋi�[���������_�f�[�^�̃A�h���X��n�����Ƃ�CreateBuffer()���Ƀf�[�^�����邱�Ƃ��ł���B
		subresource_data.SysMemPitch = 0; //Not use for vertex buffers.
		subresource_data.SysMemSlicePitch = 0; //Not use for vertex buffers.
		// ���_�o�b�t�@�I�u�W�F�N�g�̐���
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, &vertexBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	
	{// �_�~�[�e�N�X�`���̐���
		const int width = 8;
		const int height = 8;
		UINT pixels[width * height];
		::memset(pixels, 0xFF, sizeof(pixels));

		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		::memset(&data, 0, sizeof(data));
		data.pSysMem = pixels;
		data.SysMemPitch = width;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
		HRESULT hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		hr = device->CreateShaderResourceView(texture.Get(), nullptr, shaderResourceView.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		textureWidth = desc.Width;
		textureHeight = desc.Height;
	}
	if (filename) {
		LoadTextureFromFile(device, filename, shaderResourceView.GetAddressOf(), &texture2dDesc);
		textureWidth = texture2dDesc.Width;
		textureHeight = texture2dDesc.Height;
	}
}
Sprite::Sprite()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	HRESULT hr = S_OK;

	// ���_�f�[�^�̒�`
	// 0           1
	// +-----------+
	// |           |
	// |           |
	// +-----------+
	// 2           3
	Vertex vertices[] = {
		{ DirectX::XMFLOAT3(-0.5, +0.5, 0), DirectX::XMFLOAT4(1, 1, 1, 1) },
		{ DirectX::XMFLOAT3(+0.5, +0.5, 0), DirectX::XMFLOAT4(1, 0, 0, 1) },
		{ DirectX::XMFLOAT3(-0.5, -0.5, 0), DirectX::XMFLOAT4(0, 1, 0, 1) },
		{ DirectX::XMFLOAT3(+0.5, -0.5, 0), DirectX::XMFLOAT4(0, 0, 1, 1) },
	};
	Vertex vertices2[] = {
		{ DirectX::XMFLOAT3(-0.5, +0.5, 0), DirectX::XMFLOAT4(1, 1, 1, 1) },
		{ DirectX::XMFLOAT3(+0.5, +0.5, 0), DirectX::XMFLOAT4(1, 0, 0, 1) },
		{ DirectX::XMFLOAT3(-0.5, -0.5, 0), DirectX::XMFLOAT4(0, 1, 0, 1) },

	};
	// �|���S����`�悷��ɂ�GPU�ɒ��_�f�[�^��V�F�[�_�[�Ȃǂ̃f�[�^��n���K�v������B
	// GPU�Ƀf�[�^��n���ɂ�ID3D11***�̃I�u�W�F�N�g����ăf�[�^��n���܂��B

	// ���_�o�b�t�@�̐���
	{
		// ���_�o�b�t�@���쐬���邽�߂̐ݒ�I�v�V����
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(vertices);	// �o�b�t�@�i�f�[�^���i�[������ꕨ�j�̃T�C�Y
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;	// UNIT.03
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// ���_�o�b�t�@�Ƃ��ăo�b�t�@���쐬����B
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// UNIT.03
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		// ���_�o�b�t�@�ɒ��_�f�[�^�����邽�߂̐ݒ�
		D3D11_SUBRESOURCE_DATA subresource_data = {};
		subresource_data.pSysMem = vertices;	// �����Ɋi�[���������_�f�[�^�̃A�h���X��n�����Ƃ�CreateBuffer()���Ƀf�[�^�����邱�Ƃ��ł���B
		subresource_data.SysMemPitch = 0; //Not use for vertex buffers.
		subresource_data.SysMemSlicePitch = 0; //Not use for vertex buffers.
		// ���_�o�b�t�@�I�u�W�F�N�g�̐���
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, &vertexBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	{// �_�~�[�e�N�X�`���̐���
		const int width = 8;
		const int height = 8;
		UINT pixels[width * height];
		::memset(pixels, 0xFF, sizeof(pixels));

		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		::memset(&data, 0, sizeof(data));
		data.pSysMem = pixels;
		data.SysMemPitch = width;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
		HRESULT hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		hr = device->CreateShaderResourceView(texture.Get(), nullptr, shaderResourceView.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		textureWidth = desc.Width;
		textureHeight = desc.Height;
	}
}
// �`����s
void Sprite::Render(ID3D11DeviceContext* immediate_context,
	// �ꏊ
	float dx, float dy,
	// ������
	float dw, float dh,
	// �؂���n��
	float sx, float sy,
	// �؂��蕝����
	float sw, float sh,
	float angle,
	float r, float g, float b, float a,float d, float ds, float dd, bool i) const
{
	{
		// ���ݐݒ肳��Ă���r���[�|�[�g����X�N���[���T�C�Y���擾����B
		D3D11_VIEWPORT viewport;
		UINT numViewports = 1;
		immediate_context->RSGetViewports(&numViewports, &viewport);
		float screen_width = viewport.Width;
		float screen_height = viewport.Height;

		// �X�v���C�g���\������S���_�̃X�N���[�����W���v�Z����
		DirectX::XMFLOAT2 positions[] = {
			DirectX::XMFLOAT2(dx,      dy),			// ����
			DirectX::XMFLOAT2(dx + dw, dy),			// �E��
			DirectX::XMFLOAT2(dx,      dy + dh),	// ����
			DirectX::XMFLOAT2(dx + dw, dy + dh),	// �E��
		};

		// �X�v���C�g���\������S���_�̃e�N�X�`�����W���v�Z����
		DirectX::XMFLOAT2 texcoords[] = {
			DirectX::XMFLOAT2(sx,      sy),			// ����
			DirectX::XMFLOAT2(sx + sw, sy),			// �E��
			DirectX::XMFLOAT2(sx,      sy + sh),	// ����
			DirectX::XMFLOAT2(sx + sw, sy + sh),	// �E��
		};

		// �X�v���C�g�̒��S�ŉ�]�����邽�߂ɂS���_�̒��S�ʒu��
		// ���_(0, 0)�ɂȂ�悤�Ɉ�U���_���ړ�������B
		float mx = dx;
		float my = dy;
		if (i) {
			mx = dx + dw * 0.5f;
			my = dy + dh * 0.5f;
		}

		for (auto& p : positions)
		{
			p.x -= mx;
			p.y -= my;
		}


		// ���_����]������
		const float PI = 3.141592653589793f;
		float theta = angle * (PI / 180.0f);	// �p�x�����W�A��(��)�ɕϊ�
		float c = cosf(theta);
		float s = sinf(theta);
		for (auto& p : positions)
		{
			DirectX::XMFLOAT2 r = p;
			p.x = c * r.x + -s * r.y;
			p.y = s * r.x + c * r.y;
		}

		// ��]�̂��߂Ɉړ����������_�����̈ʒu�ɖ߂�

		for (auto& p : positions)
		{
			p.x += mx;
			p.y += my;
		}


		// �X�N���[�����W�n����NDC���W�n�֕ϊ�����B
		for (auto& p : positions)
		{
			p.x = 2.0f * p.x / screen_width - 1.0f;
			p.y = 1.0f - 2.0f * p.y / screen_height;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���J�n����B
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		HRESULT hr = immediate_context->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// pData��ҏW���邱�ƂŒ��_�f�[�^�̓��e�����������邱�Ƃ��ł���B
		Vertex* v = static_cast<Vertex*>(mappedBuffer.pData);
		for (int i = 0; i < 4; ++i)
		{
			v[i].position.x = positions[i].x;
			v[i].position.y = positions[i].y;
			v[i].position.z = 0.0f;

			v[i].color.x = r;
			v[i].color.y = g;
			v[i].color.z = b;
			v[i].color.w = a;

			v[i].texcoord.x = texcoords[i].x / textureWidth;
			v[i].texcoord.y = texcoords[i].y / textureHeight;

			v[i].dissolve.x = d;
			v[i].dissolve.y = ds;
			v[i].dissolve.z = dd;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���I������B
		immediate_context->Unmap(vertexBuffer.Get(), 0);
	}

}
void Sprite::Render(ID3D11DeviceContext* dc, float d, float ds, float dd, bool i) const
{
	// ���ݐݒ肳��Ă���r���[�|�[�g����X�N���[���T�C�Y���擾����B
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);
	float screen_width = viewport.Width;
	float screen_height = viewport.Height;

	float dx = 0.0f;
	float dy = 0.0f;
	float dw = screen_width;
	float dh = screen_height;
	// �X�v���C�g���\������S���_�̃X�N���[�����W���v�Z����
	DirectX::XMFLOAT2 positions[] = 
	{
		DirectX::XMFLOAT2(dx,      dy),			// ����
		DirectX::XMFLOAT2(dx + dw, dy),			// �E��
		DirectX::XMFLOAT2(dx,      dy + dh),	// ����
		DirectX::XMFLOAT2(dx + dw, dy + dh),	// �E��
	};

	float sx = 0.0f;
	float sy = 0.0f;
	float sw = static_cast<float>(textureWidth);
	float sh = static_cast<float>(textureHeight);
	// �X�v���C�g���\������S���_�̃e�N�X�`�����W���v�Z����
	DirectX::XMFLOAT2 texcoords[] = 
	{
		DirectX::XMFLOAT2(sx,      sy),			// ����
		DirectX::XMFLOAT2(sx + sw, sy),			// �E��
		DirectX::XMFLOAT2(sx,      sy + sh),	// ����
		DirectX::XMFLOAT2(sx + sw, sy + sh),	// �E��
	};

	// �X�v���C�g�̒��S�ŉ�]�����邽�߂ɂS���_�̒��S�ʒu��
	// ���_(0, 0)�ɂȂ�悤�Ɉ�U���_���ړ�������B
	float mx = dx;
	float my = dy;
	if (i) 
	{
		mx = dx + dw * 0.5f;
		my = dy + dh * 0.5f;
	}

	for (auto& p : positions)
	{
		p.x -= mx;
		p.y -= my;
	}

	// ���_����]������
	const float PI = 3.141592653589793f;
	float theta = 0.0f;	// �p�x�����W�A��(��)�ɕϊ�
	float c = cosf(theta);
	float s = sinf(theta);
	for (auto& p : positions)
	{
		DirectX::XMFLOAT2 r = p;
		p.x = c * r.x + -s * r.y;
		p.y = s * r.x + c * r.y;
	}

	// ��]�̂��߂Ɉړ����������_�����̈ʒu�ɖ߂�
	for (auto& p : positions)
	{
		p.x += mx;
		p.y += my;
	}

	// �X�N���[�����W�n����NDC���W�n�֕ϊ�����B
	for (auto& p : positions)
	{
		p.x = 2.0f * p.x / screen_width - 1.0f;
		p.y = 1.0f - 2.0f * p.y / screen_height;
	}

	// ���_�o�b�t�@�̓��e�̕ҏW���J�n����B
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	HRESULT hr = dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// pData��ҏW���邱�ƂŒ��_�f�[�^�̓��e�����������邱�Ƃ��ł���B
	Vertex* v = static_cast<Vertex*>(mappedBuffer.pData);
	for (int i = 0; i < 4; ++i)
	{
		v[i].position.x = positions[i].x;
		v[i].position.y = positions[i].y;
		v[i].position.z = 0.0f;

		v[i].color.x = 1.0f;
		v[i].color.y = 1.0f;
		v[i].color.z = 1.0f;
		v[i].color.w = 1.0f;

		v[i].texcoord.x = texcoords[i].x / textureWidth;
		v[i].texcoord.y = texcoords[i].y / textureHeight;

		v[i].dissolve.x = d;
		v[i].dissolve.y = ds;
		v[i].dissolve.z = dd;
	}

	// ���_�o�b�t�@�̓��e�̕ҏW���I������B
	dc->Unmap(vertexBuffer.Get(), 0);
}
void Sprite::Render(ID3D11DeviceContext* dc,
	float dx, float dy, float dw, float dh,
	float d, float ds, float dd, bool i) const
{
	{
		// ���ݐݒ肳��Ă���r���[�|�[�g����X�N���[���T�C�Y���擾����B
		D3D11_VIEWPORT viewport;
		UINT numViewports = 1;
		dc->RSGetViewports(&numViewports, &viewport);
		float screen_width = viewport.Width;
		float screen_height = viewport.Height;

		// �X�v���C�g���\������S���_�̃X�N���[�����W���v�Z����
		DirectX::XMFLOAT2 positions[] = 
		{
			DirectX::XMFLOAT2(dx,      dy),			// ����
			DirectX::XMFLOAT2(dx + dw, dy),			// �E��
			DirectX::XMFLOAT2(dx,      dy + dh),	// ����
			DirectX::XMFLOAT2(dx + dw, dy + dh),	// �E��
		};

		float sx = 0.0f;
		float sy = 0.0f;
		float sw = static_cast<float>(textureWidth);
		float sh = static_cast<float>(textureHeight);
		// �X�v���C�g���\������S���_�̃e�N�X�`�����W���v�Z����
		DirectX::XMFLOAT2 texcoords[] = 
		{
			DirectX::XMFLOAT2(sx,      sy),			// ����
			DirectX::XMFLOAT2(sx + sw, sy),			// �E��
			DirectX::XMFLOAT2(sx,      sy + sh),	// ����
			DirectX::XMFLOAT2(sx + sw, sy + sh),	// �E��
		};

		// �X�v���C�g�̒��S�ŉ�]�����邽�߂ɂS���_�̒��S�ʒu��
		// ���_(0, 0)�ɂȂ�悤�Ɉ�U���_���ړ�������B
		float mx = dx;
		float my = dy;
		if (i) {
			mx = dx + dw * 0.5f;
			my = dy + dh * 0.5f;
		}

		for (auto& p : positions)
		{
			p.x -= mx;
			p.y -= my;
		}

		// ���_����]������
		const float PI = 3.141592653589793f;
		float theta = 0.0f;	// �p�x�����W�A��(��)�ɕϊ�
		float c = cosf(theta);
		float s = sinf(theta);
		for (auto& p : positions)
		{
			DirectX::XMFLOAT2 r = p;
			p.x = c * r.x + -s * r.y;
			p.y = s * r.x + c * r.y;
		}

		// ��]�̂��߂Ɉړ����������_�����̈ʒu�ɖ߂�
		for (auto& p : positions)
		{
			p.x += mx;
			p.y += my;
		}

		// �X�N���[�����W�n����NDC���W�n�֕ϊ�����B
		for (auto& p : positions)
		{
			p.x = 2.0f * p.x / screen_width - 1.0f;
			p.y = 1.0f - 2.0f * p.y / screen_height;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���J�n����B
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		HRESULT hr = dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// pData��ҏW���邱�ƂŒ��_�f�[�^�̓��e�����������邱�Ƃ��ł���B
		Vertex* v = static_cast<Vertex*>(mappedBuffer.pData);
		for (int i = 0; i < 4; ++i)
		{
			v[i].position.x = positions[i].x;
			v[i].position.y = positions[i].y;
			v[i].position.z = 0.0f;

			v[i].color.x = 1.0f;
			v[i].color.y = 1.0f;
			v[i].color.z = 1.0f;
			v[i].color.w = 1.0f;

			v[i].texcoord.x = texcoords[i].x / textureWidth;
			v[i].texcoord.y = texcoords[i].y / textureHeight;

			v[i].dissolve.x = d;
			v[i].dissolve.y = ds;
			v[i].dissolve.z = dd;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���I������B
		dc->Unmap(vertexBuffer.Get(), 0);
	}
}
void Sprite::RotateRender(ID3D11DeviceContext* dc,
	float dx, float dy, float dw, float dh, float angle,
	float d, float ds, float dd, bool i) const
{
	{
		// ���ݐݒ肳��Ă���r���[�|�[�g����X�N���[���T�C�Y���擾����B
		D3D11_VIEWPORT viewport;
		UINT numViewports = 1;
		dc->RSGetViewports(&numViewports, &viewport);
		float screen_width = viewport.Width;
		float screen_height = viewport.Height;

		// �X�v���C�g���\������S���_�̃X�N���[�����W���v�Z����
		DirectX::XMFLOAT2 positions[] = 
		{
			DirectX::XMFLOAT2(dx,      dy),			// ����
			DirectX::XMFLOAT2(dx + dw, dy),			// �E��
			DirectX::XMFLOAT2(dx,      dy + dh),	// ����
			DirectX::XMFLOAT2(dx + dw, dy + dh),	// �E��
		};

		float sx = 0.0f;
		float sy = 0.0f;
		float sw = static_cast<float>(textureWidth);
		float sh = static_cast<float>(textureHeight);
		// �X�v���C�g���\������S���_�̃e�N�X�`�����W���v�Z����
		DirectX::XMFLOAT2 texcoords[] = 
		{
			DirectX::XMFLOAT2(sx,      sy),			// ����
			DirectX::XMFLOAT2(sx + sw, sy),			// �E��
			DirectX::XMFLOAT2(sx,      sy + sh),	// ����
			DirectX::XMFLOAT2(sx + sw, sy + sh),	// �E��
		};

		// �X�v���C�g�̒��S�ŉ�]�����邽�߂ɂS���_�̒��S�ʒu��
		// ���_(0, 0)�ɂȂ�悤�Ɉ�U���_���ړ�������B
		float mx = dx;
		float my = dy;
		if (i) 
		{
			mx = dx + dw * 0.5f;
			my = dy + dh * 0.5f;
		}

		for (auto& p : positions)
		{
			p.x -= mx;
			p.y -= my;
		}

		// ���_����]������
		const float PI = 3.141592653589793f;
		float theta = angle;	// �p�x�����W�A��(��)�ɕϊ�
		float c = cosf(theta);
		float s = sinf(theta);
		for (auto& p : positions)
		{
			DirectX::XMFLOAT2 r = p;
			p.x = c * r.x + -s * r.y;
			p.y = s * r.x + c * r.y;
		}

		// ��]�̂��߂Ɉړ����������_�����̈ʒu�ɖ߂�
		for (auto& p : positions)
		{
			p.x += mx;
			p.y += my;
		}

		// �X�N���[�����W�n����NDC���W�n�֕ϊ�����B
		for (auto& p : positions)
		{
			p.x = 2.0f * p.x / screen_width - 1.0f;
			p.y = 1.0f - 2.0f * p.y / screen_height;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���J�n����B
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		HRESULT hr = dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// pData��ҏW���邱�ƂŒ��_�f�[�^�̓��e�����������邱�Ƃ��ł���B
		Vertex* v = static_cast<Vertex*>(mappedBuffer.pData);
		for (int i = 0; i < 4; ++i)
		{
			v[i].position.x = positions[i].x;
			v[i].position.y = positions[i].y;
			v[i].position.z = 0.0f;

			v[i].color.x = 1.0f;
			v[i].color.y = 1.0f;
			v[i].color.z = 1.0f;
			v[i].color.w = 1.0f;

			v[i].texcoord.x = texcoords[i].x / textureWidth;
			v[i].texcoord.y = texcoords[i].y / textureHeight;

			v[i].dissolve.x = d;
			v[i].dissolve.y = ds;
			v[i].dissolve.z = dd;
		}

		// ���_�o�b�t�@�̓��e�̕ҏW���I������B
		dc->Unmap(vertexBuffer.Get(), 0);
	}
}

void Sprite::RenderCircle(ID3D11DeviceContext* dc,
	float dx, float dy,
	float radius,
	float sx, float sy,
	float sw, float sh,
	float angle,
	float r, float g, float b, float a,
	float n) const
{
	{
		if (n < 3 || radius <= 0.0f) return;
		if (n > 64) n = 64;//�ő�64�p�`

		D3D11_VIEWPORT viewport;
		UINT numViewports = 1;
		dc->RSGetViewports(&numViewports, &viewport);

		vertex vertices[130] = { DirectX::XMFLOAT3(0,0,0) };
		float arc = DirectX::XM_PI * 2 / n;
		vertex* p = &vertices[0];

		float sinValue = sinf(angle);
		float cosValue = cosf(angle);
		for (int i = 0; i <= n; i++)
		{
			float rx = cosf(arc * i) * radius * (sx + sw);
			float ry = sinf(arc * i) * radius * (sy + sh);
			p->position.x = dx + rx * cosValue - ry * sinValue;
			p->position.y = dy + rx * sinValue + ry * cosValue;

			p->position.x = -1.0f + p->position.x * 2 / viewport.Width;
			p->position.y = 1.0f - p->position.y * 2 / viewport.Height;
			p->color = DirectX::XMFLOAT4(r, g, b, a);
			p++;

			p->position.x = dx;
			p->position.y = dy;

			p->position.x = -1.0f + p->position.x * 2 / viewport.Width;
			p->position.y = 1.0f - p->position.y * 2 / viewport.Height;
			p->color = DirectX::XMFLOAT4(r, g, b, a);
			p++;
		}

		D3D11_MAPPED_SUBRESOURCE msr;
		dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, vertices, sizeof(vertex) * (size_t)(n + 1) * 2);
		dc->Unmap(vertexBuffer.Get(), 0);

		UINT stride = sizeof(vertex);
		UINT offset = 0;
		dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		dc->IASetInputLayout(inputLayout.Get());
		dc->RSSetState(rasterizerState.Get());
		dc->VSSetShader(vertexShader.Get(), NULL, 0);
		dc->PSSetShader(pixelShader.Get(), NULL, 0);
		dc->OMSetDepthStencilState(depthStencilState.Get(), 1);

		dc->Draw(((UINT)n + 1) * 2 - 1, 0);


	}
}

void Sprite::SetShaderResourceView(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv, int texWidth, int texHeight)
{
	shaderResourceView = srv;
	textureWidth = texWidth;
	textureHeight = texHeight;
}

