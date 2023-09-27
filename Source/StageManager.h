#pragma once

#include <vector>
#include "Stage.h"

// ステージマネージャー
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
	void Render(ID3D11DeviceContext* dc, ModelShader* shader);

	void InstaningRender(ID3D11DeviceContext* dc, ModelShader* shader);

	// ステージ登録
	void Register(Stage* stage);

	// ステージ全解除
	void clear();

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

	bool PillarVS(const DirectX::XMFLOAT3& pos, const float radius, const float height, DirectX::XMFLOAT3& outPos);

private:
	static const int MAX_BONES{ 256 };
	std::unique_ptr<Model> obj = nullptr;
	std::vector<Stage*> stages;
	InstancingConstants constants;
	int instangNo = 0;
};