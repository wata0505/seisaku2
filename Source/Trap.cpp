#include "Trap.h"
#include "TrapManager.h"
#include "Graphics.h"
#include "EnemyManager.h"
#include "Camera.h"
#include "StageManager.h"
#include "AudioAll.h"
#include <math.h>

// デバッグプリミティブ描画処理
void Trap::DrawDebugPrimitive()
{
	DebugRenderer* debugRendere = Graphics::Instance().GetDebugRenderer();

	//衝突判定用のデバック円柱を描画
	debugRendere->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

// 行列更新処理
void Trap::UpdateTransform(int axisType, int lengthType)
{
	const DirectX::XMFLOAT4X4 coordinateSystemTransforms[] =
	{
		{ -1,0,0,0, 0,1, 0,0, 0,0,1,0, 0,0,0,1 },		// 0:RHS Y-UP　右手系
		{  1,0,0,0, 0,1, 0,0, 0,0,1,0, 0,0,0,1 },		// 1:LHS Y-UP　左手系
		{ -1,0,0,0, 0,0,-1,0, 0,1,0,0, 0,0,0,1 },		// 2:RHS Z-UP　右手系
		{  1,0,0,0, 0,0, 1,0, 0,1,0,0, 0,0,0,1 },		// 3:LHS Z-UP　左手系
	};

	const float scaleFactor[] = { 0.01f, 1.0f };

	DirectX::XMMATRIX C = { DirectX::XMLoadFloat4x4(&coordinateSystemTransforms[axisType]) * DirectX::XMMatrixScaling(scaleFactor[lengthType], scaleFactor[lengthType], scaleFactor[lengthType]) };
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

// 破棄
void Trap::Destroy()
{
	TrapManager::Instance().Remove(this);
}

// 一番近い敵索敵
bool Trap::SearchEnemy(float territoryRange, float radius)
{
	EnemyManager& enemyManager = EnemyManager::Instance();
	dist = territoryRange;	// 縄張り範囲
	HitResult hit;
	for (int i = 0; i < enemyManager.GetEnemyCount(); i++) 
	{
		// 敵との距離判定
		Enemy* e = enemyManager.GetEnemy(i);
		float vx = e->GetPosition().x - position.x;
		float vz = e->GetPosition().z - position.z;
		float d = sqrtf(vx * vx + vz * vz);

		if (d < dist && radius < d && e->GetHealth() > 0 && !StageManager::Instance().RayCast(position, e->GetPosition(), hit))	// 近すぎると反応しない
		{
			dist = d;
			targetPosition = e->GetPosition();
		}
	}

	if (dist != territoryRange)	// 敵が見つかっていたら
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 旋回処理
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

	// XZ平面旋回処理
	TurnHorizontal(elapsedTime, vx, vz, turnSpeed * speedRate);
	// Y軸旋回処理
	TurnVertical();
}
// XZ平面旋回処理
void Trap::TurnHorizontal(float elapsedTime, float vx, float vz, float speed)
{
	float rotateSpeed = speed * elapsedTime;

	// 進行ベクトルがゼロベクトルの場合は処理する必要なし
	float Length = sqrtf((vx * vx) + (vz * vz));
	if (Length == 0.0f)
	{
		return;
	}

	// 進行ベクトルを単位ベクトル化
	vx /= Length;
	vz /= Length;

	// 自身の回転値から前方向を求める
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
	}
	else
	{
		angle.y += rotateSpeed;
	}
}
// Y軸旋回処理
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

// ダメージ処理
void Trap::InputDamage(int damage) 
{
	// 体力が残っていれば
	if (health > 0)
	{
		health -= damage;	// 体力減少
		// 体力が残っていなければ
		if (health < 0)
		{
			health = 0;	// 体力を固定
		}
	}
}

// ホログラムシェーダー情報初期化
void Trap::HologramShaderDataInitialize(float minHeight, float maxHeight)
{
	scanTimer = 1.0f;				// スキャンラインの時間
	scanBorder = 0.0f;				// スキャンラインの描画範囲
	glowTimer = 1.0f;				// グロウラインの時間
	glowBorder = 0.0f;				// グロウラインの描画範囲
	hologramTimer = 0.0f;			// ホログラムラインの時間
	hologramBorder = 0.0f;			// ホログラムラインの描画範囲
	this->minHeight = minHeight;	// 最低点
	this->maxHeight = maxHeight;	// 最高点
}
// ホログラムシェーダー更新処理
void Trap::UpdateHologramShader(float elapsedTime, bool isActive)
{
	// minHeight : 足元位置
	// maxHeight : 頭頂位置
	// 高さ算出
	float length = maxHeight - minHeight;

	float inverse = -1.0f;
	length *= inverse;
	float startPosition = minHeight;
	if (minHeight < 0.0f)
	{
		startPosition = maxHeight;
	}

	// 許容時間
	static constexpr float permissionTimer = 1.0f;
	if (isActive)
	{
		// 設置直後なら
		if (hologramTimer <= 0.0f)
		{
			float volume = 0.3f;
			// カメラからの距離に応じて音量調節
			{
				float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&Camera::Instance().GetEye()), DirectX::XMLoadFloat3(&position))));
				if (length > 20.0f)
				{
					volume = (20.0f / length) * 0.3f;
				}
				else
				{
					volume = 0.3f;
				}
			}

			// ホログラム音停止と再生
			AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Hologram)->Stop();	
			AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Hologram)->Play(false, volume);
		}
		// ホログラムラインの時間加算
		hologramTimer += elapsedTime;
		// ホログラムラインの時間が許容時間以上なら固定
		if (hologramTimer >= permissionTimer)
		{
			hologramTimer = permissionTimer;
		}
	}

	// スキャンラインの描画範囲を足元位置から高さに時間割合で算出
	scanBorder = length * scanTimer + startPosition;
	// グロウラインの描画範囲を足元位置から高さに時間割合で算出
	glowBorder = length * glowTimer + startPosition;
	// ホログラムラインの描画範囲を足元位置から高さに時間割合で算出
	hologramBorder = length * hologramTimer + startPosition;
}

// 縄張り設定
void Trap::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}