#include "Trap.h"
#include "Graphics.h"
#include"TrapManager.h"
#include "EnemyManager.h"
#include "Camera.h"
#include <math.h>

//デバック用
#include "Player.h"

//デバッグプリミティブ描画
void Trap::DrawDebugPrimitive()
{
	DebugRenderer* debugRendere = Graphics::Instance().GetDebugRenderer();

	//衝突判定用のデバック円柱を描画
	debugRendere->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//破棄
void Trap::Destroy()
{
	TrapManager::Instance().Remove(this);
}

//行列更新処理
void Trap::UpdateTransform(int axisType, int lengthType)
{
	const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
		{ -1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},		//0:RHS Y-UP　右手系
		{  1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},		//1:LHS Y-UP　左手系
		{ -1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1},		//2:RHS Z-UP　右手系
		{  1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},		//3:LHS Z-UP　左手系
	};

	const float scale_factor[] = { 0.01f,1.0f };

	DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[axisType]) * DirectX::XMMatrixScaling(scale_factor[lengthType],scale_factor[lengthType],scale_factor[lengthType]) };
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//回転行列を作成
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	//3つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = C * S * R * T;
	//計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

bool Trap::SearchEnemy(float territoryRange, float radius)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	//dist = FLT_MAX;//縄張り範囲
	dist = territoryRange;//縄張り範囲

	for (int i = 0; i < enemyManager.GetEnemyCount(); i++) {
		//敵との距離判定
		Enemy* e = enemyManager.GetEnemy(i);
		float vx = e->GetPosition().x - position.x;
		float vz = e->GetPosition().z - position.z;
		float d = sqrtf(vx * vx + vz * vz);

		if (d < dist && radius < d)//近すぎると反応しない
		{
			dist = d;
			targetPosition = e->GetPosition();

		}
	}

	//デバック用
	//float vx = Player::Instance().GetPosition().x - position.x;
	//float vz = Player::Instance().GetPosition().z - position.z;
	//float d = sqrtf(vx * vx + vz * vz);
	//if (d < dist && radius < d)//近すぎると反応しない
	//{
	//	dist = d;
	//	targetPosition = Player::Instance().GetPosition();
	//}



	if (dist != territoryRange)//敵が見つかっていたら
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Trap::MoveToTarget(float elapsedTime, float speedRate)
{
	//ターゲット方向への進行ベクトルを算出
	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	vx /= dist;
	vy /= dist;
	vz /= dist;
	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

void Trap::TurnToTarget(float elapsedTime, float speedRate)
{
	//ターゲット方向への進行ベクトルを算出
	float vx = targetPosition.x - position.x;
	float vy = targetPosition.y - position.y;
	float vz = targetPosition.z - position.z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	vx /= dist;
	vy /= dist;
	vz /= dist;

	Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
	TurnVertical();
}

//旋回処理
void Trap::TurnVertical()
{
	DirectX::XMVECTOR Operator = DirectX::XMLoadFloat3(&position);        // 操作キャラの位置ベクトル
	DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&targetPosition);    // ターゲットの位置ベクトル

	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&Camera::Instance().GetEye());                // カメラ位置ベクトル

	// 操作キャラ->ターゲットベクトル
	DirectX::XMVECTOR OperatorToTarget = DirectX::XMVectorSubtract(Target, Operator);
	// ターゲット->操作キャラベクトル
	DirectX::XMVECTOR TargetToOperator = DirectX::XMVectorSubtract(Operator, Target);
	// 操作キャラ->カメラベクトル
	DirectX::XMVECTOR OperatorToEye = DirectX::XMVectorSubtract(Eye, Operator);

	// X軸回転の計算
	{
		// 操作キャラ->ターゲットベクトルの長さ算出
		float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(OperatorToTarget));
		// XZ平面の操作キャラ->ターゲットベクトルの長さ算出
		DirectX::XMVECTOR XZFrogToTarget = DirectX::XMVectorSetY(OperatorToTarget, 0);
		float xzLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(XZFrogToTarget));
		// XZ平面の長さを元の斜辺の長さで割って内積値算出
		float dot = xzLength / length;
		dot = acosf(dot);
		DirectX::XMFLOAT3 up = { 0,1,0 };
		// 上ベクトルと操作キャラ->ターゲットベクトルの内積			
		float axisXDot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&up), OperatorToTarget));
		// 上ベクトルと操作キャラ->ターゲットベクトルに対してカメラが上下どちらにあるかの判定
		// カメラが下向きの場合
		if (axisXDot < 0)
		{
			angle.x = dot;
		}
		// カメラが上向きの場合
		else
		{
			angle.x = -dot;
		}
	}
}

//旋回処理
void Trap::Turn(float elapsedTime, float vx, float vz, float speed)
{
	float rotateSpeed = speed * elapsedTime;

	//進行ベクトルがゼロベクトルの場合は処理する必要なし
	float Length = sqrtf((vx * vx) + (vz * vz));
	if (Length == 0.0f)
	{
		center = true;
		return;
	}

	//進行ベクトルを単位ベクトル化
	vx /= Length;
	vz /= Length;

	//自身の回転値から前方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// 回転角を求めるため、２つの単位ベクトルの内積を計算する
	float dot = (frontX * vx) + (frontZ * vz);

	// 内積値は-1.0~1.0で表現されており、２つの単位ベクトルの角度が
	// 小さいほど1.0に近づくという性質を利用して回転速度を調整する
	float rot = 1.0f - dot;
	if (rotateSpeed > rot)rotateSpeed = rot;

	//左右判定を行うために2つの単位ベクトルの外積を計算する
	float cross = (frontZ * vx) - (frontX * vz);

	// 2Dの外積値が正の場合か負の場合によって左右判定が行える
	// 左右判定を行うことによって左右回転を選択する
	if (cross < 0.0f)
	{
		angle.y -= rotateSpeed;
		center = false;
	}
	else
	{
		angle.y += rotateSpeed;
		center = false;
	}
}

// 縄張り設定
void Trap::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}

void Trap::InputDamage(int damage) {
	if (health > 0)
	{
		health -= damage;
		if (health < 0)
		{
			health = 0;
		}
	}
}