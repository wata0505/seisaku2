#pragma once
#pragma once

#include "Model.h"
#include "Stage.h"

//メインステージ
class StageObj :public Stage
{
public:
	StageObj(DirectX::XMFLOAT3 pos,float angle2 = 0);
	~StageObj()override;

	//更新処理
	void Update(float elapsedTime)override;

	//描画処理
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)override;
	bool PillarVS(const DirectX::XMFLOAT3& pos, const float radius, const float height, DirectX::XMFLOAT3& outPos)override;
	void UpdateTransform();

	//位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	//位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	//スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	//スケール取得
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

private:
	std::unique_ptr<Model> obj = nullptr;
	DirectX::XMFLOAT3	  position = { 0,0,0 };
	DirectX::XMFLOAT3	  angle{ 0,0,0 };
	DirectX::XMFLOAT3	  scale = { 1,1,1 };
	bool renderflag = true;
	float radius = 5.0;
	float height = 10;
	DirectX::XMFLOAT4X4   transform2 = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
};