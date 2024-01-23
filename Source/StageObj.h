#pragma once

#include "Model.h"
#include "Stage.h"

class StageObj : public Stage
{
public:
	StageObj(DirectX::XMFLOAT3 position, float angleY = 0.0f, float angleZ = 0.0f);
	~StageObj() override {};

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) override;

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;
	
	// 行列更新処理
	void UpdateTransform();

private:
	std::unique_ptr<Model>	obj = nullptr;												// モデル
	DirectX::XMFLOAT4X4		raycastTransform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };	// レイキャスト用行列
	//float					radius = 5.0f;												// 半径
	//bool					isRender = true;											// 描画フラグ
};