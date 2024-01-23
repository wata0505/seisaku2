#pragma once

#include "Model.h"
#include "Stage.h"

class StageMain : public Stage
{
public:
	StageMain();
	~StageMain() override {};

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) override;

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;

	// 行列更新処理
	void UpdateTransform();

private:
	std::unique_ptr<Model>	model = nullptr;		// モデル
	float					progressTimer = 0.0f;	// 経過時間
};