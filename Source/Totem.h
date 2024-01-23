#pragma once

#include "Trap.h"

class Totem : public Trap
{
public:
	Totem();
	~Totem() {}

public:
	// 更新処理
	void Update(float elapsedTime) override;
	// 描画処理
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) override;
	// 残像エフェクト描画処理
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, ModelShader* shader) override {}
	// デバッグプリミティブ描画処理
	void DrawDebugPrimitive() override;
	// デバッグ情報表示
	void DrawDebugGUI() override;

	// 敵との衝突処理
	void CollisionVsEnemies();

private:
	std::unique_ptr<Model> model = nullptr;	// モデル
	float survivalTime = 0.0f;				// 生存時間
};