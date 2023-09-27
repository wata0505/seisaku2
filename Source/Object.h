#pragma once
#include "Shader.h"
#include "SwordTrailShader.h"

// 前方宣言
class ObjectManager;

// 弾丸
class Object
{
public:
	Object(ObjectManager* manager);
	virtual ~Object() {}

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// 描画処理
	virtual void Render(ID3D11DeviceContext* dc, ModelShader* shader) = 0;

	// エフェクト描画処理
	virtual void TrailRender(RenderContext& rc, SwordTrailShader* shader)= 0;

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	// 位置取得
	const DirectX::XMFLOAT3& GetPosition()const { return position; }

	// 方向取得
	const DirectX::XMFLOAT3& GetDirection()const { return direction; }

	// スケール取得
	const DirectX::XMFLOAT3& GetScale()const { return scale; }

	// 半径所得
	const float GetRadius()const { return radius; }

	//タイプ取得
	const int GetType()const { return type; }

	// 無敵時間
	const float GetInvincibleTime()const { return invincibleTime; }

	//ダメージ
	const int GetDamage()const { return damage; }

	void SetDirectionUp(DirectX::XMFLOAT3 up) { directionUp = up; }

	DirectX::XMFLOAT3 GetDirectionUp() { return directionUp; }

	// 破棄
	void Destroy();

protected:
	// 行列更新処理
	void UpdateTransform();
protected:
	DirectX::XMFLOAT3  position = { 0,0,0 };
	DirectX::XMFLOAT3  direction = { 0,0,1 };
	DirectX::XMFLOAT3  scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	DirectX::XMFLOAT3 directionUp = { 0,1,0};
	int type = -1;
	float damage = 1;
	float invincibleTime = 1.0;
	float lifeTimer = 3.0f;
	ObjectManager* manager = nullptr;
	float radius = 0.5f;
};