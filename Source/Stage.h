#pragma once

#include "Shader.h"
#include "Collision.h"

class Stage
{
public:
	Stage() {}
	virtual ~Stage() {}

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// 描画処理
	virtual void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) = 0;

	// レイキャスト
	virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) = 0;

public:
	// 行列取得
	const DirectX::XMFLOAT4X4& GetTransform() const { return transform; }

	// 位置設定と取得
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// スケール設定と取得
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// インスタンシング取得
	bool GetInstancing() { return instancing; }

protected:
	DirectX::XMFLOAT4X4	transform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };	// モデル行列
	DirectX::XMFLOAT3	position = {};										// 位置
	DirectX::XMFLOAT3	angle = {};											// 角度
	DirectX::XMFLOAT3	scale = { 1.0f, 1.0f, 1.0f };						// サイズ
	bool				instancing = false;									// インスタンシング
};