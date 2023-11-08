#include "Enemy.h"
#include "Graphics.h"
#include "EnemyManager.h"
#include "StageManager.h"
#include "Camera.h"
#include "Collision.h"
#include "Mathf.h"
#include "Player.h"
#include "AudioAll.h"
#include "Base.h"

// デバッグプリミティブ描画
void Enemy::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用デバッグ球を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(1, 1, 1, 1));
}

// デバッグプリミティブ描画
void Enemy::DrawDebugGUI()
{

}
void Enemy::UpdateVerticalMove(float elapsedTime) {
	// 垂直方向の移動量
	float my = velocity.y * elapsedTime;

	slopeRate = 0.0f;

	// キャラクターのY軸方向となる法線ベクトル
	DirectX::XMFLOAT3 normal = { 0,1,0 };
	if (enemyTimer % 2 != id % 2) return;
	// 落下中
	if (my < 0.0f)
	{
		// レイの開始位置は足元より少し上
		DirectX::XMFLOAT3 start = { position.x,position.y,position.z };
		// レイの終点位置は移動後の位置
		DirectX::XMFLOAT3 end = { position.x,position.y - my - lowAltitude,position.z };
		if (velocity.x == 0 && velocity.z == 0) {
			if (isGround) {
				velocity.y = 0.0f;
				return;
			}
		}

		// レイキャストによる地面判定
		HitResult hit;
		//if (health > 0) {
		if (my < 0.0f)
		{
			
			position.y = 0.4;

			// 着地した
			if (!isGround && lowAltitude == 0)
			{
				OnLanding();
				jumpflag = false;
				isGround = true;
			}

			velocity.y = 0.0f;
		}
		else
		{
			// 空中に浮いている
			position.y += my;
			isGround = false;
		}

	}
	// 上昇中
	else if (my > 0.0f)
	{

		// レイの開始位置は足元より少し上
		DirectX::XMFLOAT3 start = { position.x,position.y,position.z };
		// レイの終点位置は移動後の位置
		DirectX::XMFLOAT3 end = { position.x,position.y - 30,position.z };

		// レイキャストによる地面判定
		HitResult hit;
		if (health > 0) {
			if (StageManager::Instance().RayCast(start, end, hit))
			{
				// 地面からの距離が離れすぎていれば上昇停止
				if (hit.position.y + altitude < position.y) {
					//position.y = hit.position.y + altitude;
					my = 0.0f;
				}
			}

		}
		position.y += my;
		isGround = false;
	}
}
void Enemy::UpdateHorizontalMove(float elapsedTime) {
	//フレームごとにIDが偶数か奇数で処理する（処理軽減）
	if (enemyTimer % 2 != id % 2) return;
	// 水平速力量計算
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		// 水平移動値
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;
		// レイの開始位置と終点位置
		DirectX::XMFLOAT3 start = { position.x,position.y + height * 0.5f,position.z };
		DirectX::XMFLOAT3 end = { position.x + mx, position.y + height * 0.5f, position.z + mz };

		// レイキャストによる壁判定
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			// 壁までのベクトル
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

			// 壁の法線
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			// 入射ベクトルを法線に射影
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			// 補正位置の計算
			DirectX::XMVECTOR apos = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			DirectX::XMFLOAT3 aPos;
			DirectX::XMStoreFloat3(&aPos, apos);

			DirectX::XMVECTOR Vec2 = DirectX::XMVectorScale(DirectX::XMVector3Normalize(Vec), -1);
			DirectX::XMFLOAT3 vec2;
			DirectX::XMStoreFloat3(&vec2, Vec2);

			// 壁擦り方向にレイキャスト
			HitResult hit2;
			if (!StageManager::Instance().RayCast(hit.position, aPos, hit2))
			{
				// 壁擦りで補正位置に移動
				position.x = aPos.x;
				position.z = aPos.z;
			}
			else
			{
				position.x = hit2.position.x;//+vec2.x * radius;
				position.z = hit2.position.z;//+vec2.z * radius;
			}

		}
		else
		{
			// 移動
			position.x += mx;
			position.z += mz;
		}
		//負荷軽減のため直接値代入
		if (position.x > eria) {
			position.x = eria;
		}
		if (position.x < -eria) {
			position.x = -eria;
		}
		if (position.z > eria) {
			position.z = eria;
		}
		if (position.z < -eria) {
			position.z = -eria;
		}
	}
}
void Enemy::UpdateRnderflag() {
	//カメラで作った弧の中にいるモデルだけ描画する
	if(Player::Instance().GetPosition().y <= position.y) {
	renderflag = Collision::IntersectFanVsSphere(
		Camera::Instance().GetEye(),
		Camera::Instance().GetFront(),
		Camera::Instance().GetFovY(),
		Camera::Instance().GetFarZ(),
		position,
		radius);
     }
}


// 破棄
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}

bool Enemy::OnMessage(const Telegram& msg)
{
	return false;
}
// 縄張り設定
void Enemy::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
	territoryOrigin = origin;
	territoryRange = range;
}
// ターゲット位置をランダム設定
void Enemy::SetRandomTargetPosition()
{
	float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
	float range = Mathf::RandomRange(0.0f, territoryRange);
	targetPosition.x = territoryOrigin.x + sinf(theta) * range;
	targetPosition.y = territoryOrigin.y;
	targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}
void Enemy::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius, DirectX::XMFLOAT2 pow, float damage,float invincibleTime)
{

	nodePosition = SearchNodePos(nodeName);
	Player& player = Player::Instance();
	if (!player.GetRenderflag())return;
	DirectX::XMFLOAT3 outPosition;
	if (Collision::IntersectSphereVsCylinder(
		nodePosition, nodeRadius,
		player.GetHitPos(), player.GetRadius(), player.GetHeight(),
		outPosition))
	{
		HitInpnt(player.GetEfPos(), pow, damage, invincibleTime);
	}
}

void Enemy::CollisionNodeVsBase(const char* nodeName, float nodeRadius, DirectX::XMFLOAT2 pow, float damage, float invincibleTime)
{

	nodePosition = SearchNodePos(nodeName);
	Base& base = Base::Instance();
	DirectX::XMFLOAT3 outPosition;
	if (Collision::IntersectSphereVsCylinder(
		nodePosition, nodeRadius,
		base.GetPos(), base.GetRadius(), base.GetHeight(),
		outPosition))
	{
		base.InputDamage(damage);
	}
}
void Enemy::HitInpnt(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 pow, float damage, float invincibleTime) {
	Player& player = Player::Instance();
	if (player.GetGuardflag()&& player.GetInvincibleTimer() < 0) {
		player.SetCounterflag(true);
		OnDamaged();
		//ParticleSprite* particleSprite = new ParticleSprite(pos, { NULL,NULL,NULL }, ParticleSprite::ParticleSquare, ParticleSprite::Expansion, (int)EffectTexAll::EfTexAll::BlackThunder, 3, 0.3);
		KnockBack(*this, pos,position, pow.x*5, pow.y);
	}
	if (player.GetCounterflag())return;
	if (player.GetState() == Player::State::Guard && player.GetInvincibleTimer() < 0) {
		ParticleSprite* particleSprite = new ParticleSprite(nodePosition, { NULL,NULL,NULL }, ParticleSprite::ParticleImpact, ParticleSprite::Expansion, (int)EffectTexAll::EfTexAll::Impact, efMax, efLife);
		player.PlayerKnockBack(this->position, player.GetPosition(), pow.x, pow.y, damage);
		player.SetInvincibleTime(invincibleTime);
		return;
	}
	if (player.ApplyDamage(damage, invincibleTime, true))
	{
		float power = 10;
		//吹き飛ばす
		if (player.GetState() != Player::State::Attack) {
			player.PlayerKnockBack(this->position, player.GetPosition(), pow.x, pow.y, damage);
			//EffectAll::Instance().hitEffect->Play(player.GetEfPos(), 0.1);
			ParticleSprite* particleSprite = new ParticleSprite(pos, { NULL,NULL,NULL }, ParticleSprite::ParticleImpact, ParticleSprite::Expansion, (int)EffectTexAll::EfTexAll::Impact, efMax, efLife);

		}
		AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Hit)->Play(false, SE);

	}

}