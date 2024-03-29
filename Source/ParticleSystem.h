#pragma once
#include<vector>
#include<set>
#include"Particle.h"

//パーティクシステム
class ParticleSystem
{
public:
	ParticleSystem() {
	};
	~ParticleSystem() {};

	// 更新処理
	static ParticleSystem& Instance()
	{
		static ParticleSystem instance;
		return instance;
	}
//ハルバート特殊エフェクト
void LanceEffect(DirectX::XMFLOAT3 position);
//バリアエフェクト
void BarrierEffect(const DirectX::XMFLOAT3& position, float size, int textype, DirectX::XMFLOAT4 color);

void BarrierEnemyEffect(float enemyID, int textype, DirectX::XMFLOAT4 color);
//岩飛び散るエフェクト
void RubbleEffect(DirectX::XMFLOAT3 position,int Max,int SpeedMax);
//岩が飛び出るエフェクト
void Rubble2Effect(DirectX::XMFLOAT3 position, int Max, int SpeedMax);
//岩が飛び出るエフェクト
void Rubble3Effect(DirectX::XMFLOAT3 position, int Max, int SpeedMax,int RockType);

//爆発エフェクト
void BoomEffect(DirectX::XMFLOAT3 position,int max,int textype,float size,float liftimer,DirectX::XMFLOAT4 color);
//衝撃エフェクト
void ImpactEffect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 dir, int textype, float size, DirectX::XMFLOAT4 color);
//爆発エフェクト
void FlameBreathBoomEffect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 dir, int max, int textype, float size, DirectX::XMFLOAT4 color);
//ブレスエフェクト
void FlameBreathEffect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 dir);
//渦巻収束
void SeirlConvergenceEffect(DirectX::XMFLOAT3 position,float angle, float len = 10);
//渦巻収束
void VortexDiffusionEffect(DirectX::XMFLOAT3 position, float angle);

//煙
void CorruptionEffect(DirectX::XMFLOAT3 position,int enemyNo);

void Corruption2Effect(DirectX::XMFLOAT3 position);



private:
	//岩のタイプ
	int rockType[Particle::Rock3] = { Particle::Rock1,Particle::Rock2,Particle::Rock3 };
	//DirectX::XMFLOAT4	orientation = {};
	//const float PIDIV180 = 0.017452f;
};