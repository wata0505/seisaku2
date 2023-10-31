#pragma once
#include<vector>
#include<set>
#include"Particle.h"

//�p�[�e�B�N�V�X�e��
class ParticleSystem
{
public:
	ParticleSystem() {};
	~ParticleSystem() {};

	// �X�V����
	static ParticleSystem& Instance()
	{
		static ParticleSystem instance;
		return instance;
	}
//�n���o�[�g����G�t�F�N�g
void LanceEffect(DirectX::XMFLOAT3 position);
//�o���A�G�t�F�N�g
void BarrierEffect(const DirectX::XMFLOAT3& position, float size, int textype, DirectX::XMFLOAT4 color);

void BarrierEnemyEffect(float enemyID, int textype, DirectX::XMFLOAT4 color);
//���юU��G�t�F�N�g
void RubbleEffect(DirectX::XMFLOAT3 position,int Max,int SpeedMax);
//�����G�t�F�N�g
void BoomEffect(DirectX::XMFLOAT3 position,int max,int textype,float size,float liftimer,DirectX::XMFLOAT4 color);
//�Ռ��G�t�F�N�g
void ImpactEffect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 dir, int textype, float size, DirectX::XMFLOAT4 color);
//�����G�t�F�N�g
void FlameBreathBoomEffect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 dir, int max, int textype, float size, DirectX::XMFLOAT4 color);
//�u���X�G�t�F�N�g
void FlameBreathEffect(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 dir);

private:
	//��̃^�C�v
	int rockType[Particle::Rock3] = { Particle::Rock1,Particle::Rock2,Particle::Rock3 };
};