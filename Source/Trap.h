#pragma once
#include "Shader.h"


//トラップ
class Trap 
{
public:
	Trap() {}
	~Trap() {}

	//更新処理
	virtual void Update(float elapsedTime) = 0;

	//描画処理
	virtual void Render(ID3D11DeviceContext* dc, ModelShader* shader) = 0;

	//デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	//破棄
	void Destroy();

	
	//行列更新処理
	void UpdateTransform(int axisType, int lengthType);

	//一番近い敵索敵
	bool SearchEnemy(float territoryRange, float radius);
	// 目標地点へ移動
	void MoveToTarget(float elapsedTime, float speedRate);
	void TurnToTarget(float elspsedTime, float speedRate);
	//旋回処理
	void Turn(float elapsedTime, float vx, float vz, float speed);
	void TurnVertical();

	//位置設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	//位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 縄張り設定
	void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

	//半径取得
	float GetRadius()const { return radius; }

	//高さ取得
	float GetHeight() const { return height; }

protected:
	DirectX::XMFLOAT3		position = { 0,0,0 };
	DirectX::XMFLOAT3		targetPosition = { 0,0,0 };
	DirectX::XMFLOAT3		angle = { 0,0,0 };
	DirectX::XMFLOAT3		scale = { 50,50,50 };
	DirectX::XMFLOAT4X4		transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	float attack = 1.0;
	float radius = 1.0f;
	float height = 1.0f;
	float turnSpeed = 5.0f;

	float dist = 0.0f;

	DirectX::XMFLOAT3	territoryOrigin = { 0.0f,0.0f,0.0f };
	float				territoryRange =10.0f;
	float notAttackRange = 5.0f;

	bool center = false;

};