#pragma once
#include <memory>
#include <wrl.h>
#include "Shader.h"
#include "Sprite.h"

class DefaultSpriteShader : public SpriteShader
{
public:
	DefaultSpriteShader(ID3D11Device* device);
	~DefaultSpriteShader() override {}

	void Begin(const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Sprite* sprite) override;
	void End(const RenderContext& rc) override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	
	

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
};
