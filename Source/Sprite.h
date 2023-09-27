#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

//using namespace std;

class Sprite
{
public:
	Sprite(const wchar_t* filename);
	Sprite();
	//Sprite(const char* filename);
	~Sprite() {}

	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT2	texcoord;
		DirectX::XMFLOAT3   dissolve;
	};

	// 描画実行
	void Render(ID3D11DeviceContext* dc,
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a,float d = 0, float ds = 0, float dd = 0, bool i = true) const;

	void RenderCircle(ID3D11DeviceContext* dc,
		float dx, float dy,
		float radius,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a,
		float n) const;

	//--------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------
	struct vertex { DirectX::XMFLOAT3 position; DirectX::XMFLOAT4 color; };


	// テクスチャ幅取得
	int GetTextureWidth() const { return textureWidth; }

	// テクスチャ高さ取得
	int GetTextureHeight() const { return textureHeight; }

	// シェーダーリソースビューの設定
	void SetShaderResourceView(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv, int texWidth, int texHeight);
	// 頂点バッファの取得
	const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetVertexBuffer() const { return vertexBuffer; }

	// シェーダーリソースビューの取得
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return shaderResourceView; }

	// テクスチャの情報取得
	inline const D3D11_TEXTURE2D_DESC& GetTexture2dDesc() const { return texture2dDesc; }
	
	// テクスチャ幅取得
	inline int GetWidth() const { return texture2dDesc.Width; }
	
	// テクスチャ高さ取得
	inline int GetHeight() const { return texture2dDesc.Height; }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertexBuffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;

	int textureWidth = 0;
	int textureHeight = 0;
	
	D3D11_TEXTURE2D_DESC	texture2dDesc;
};


