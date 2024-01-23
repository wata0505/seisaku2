#pragma once

#include <vector>
#include "Stage.h"

class StageManager
{
private:
	StageManager();
	~StageManager() {}

public:
	static StageManager& Instance()
	{
		static StageManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader);
	// インスタンシング描画処理
	void InstaningRender(ID3D11DeviceContext* deviceContext, ModelShader* shader);

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

	// ステージ登録
	void Register(Stage* stage);
	// ステージ全解除
	void Clear();

private:
	std::unique_ptr<Model>	obj = nullptr;
	std::vector<Stage*>		stages;
	InstancingConstants		constants;
	int						instangNo = 0;
};