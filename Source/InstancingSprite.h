#pragma once
#include "Model.h"
#include "Object.h"
#include "InstancingSpriteShader.h"

enum  InstancingSpriteType
{
	FlameBreath,//�u���X
	SeirlConvergence,
	VortexDiffusion,
	Corruption,
	Corruption2
};

//���i�e��
class InstancingSprite
{
public:
	enum InstancingType
	{
		FlameBreath,
		SeirlConvergence,
		VortexDiffusion,
		Corruption,
		Corruption2,
		Max
	};
public:
	InstancingSprite();
	~InstancingSprite();

	//�X�V����
	void Update(float elapsedTime);


	//�`�揈��
	void Render(ID3D11DeviceContext* dc, InstancingSpriteShader* shader);

	DirectX::XMFLOAT4 GetPosition() { return position; }

	DirectX::XMFLOAT3 GetPos() { return { position.x,position.y,position.z }; }

	DirectX::XMFLOAT4 GetMaterialColor() { return materialColor; }

	DirectX::XMFLOAT4 GetSpriteSize() { return scale; }

	float GetTexTimer() { return texTimer; }

	DirectX::XMFLOAT4 GetUvStatus() { return uvStatus; };
	//�O�����A�ʒu�A�����A�`�A���ԁA���x�A��]��X�A��]��Y�A�F
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, int type, int spriteType, float lifeTimer, float speed, float angleX,float angleY, DirectX::XMFLOAT4 materialColor = { 1,0,0,1 });


	int GetSpriteType() { return spriteType; }

	float GetRadius() { return radius; }
private:
	void FlameBreathUpdate(float elapsedTime);

	void SeirlConvergenceUpdate(float elapsedTime);

	void VortexDiffusionUpdate(float elapsedTime);

	void CorruptionUpdate(float elapsedTime);

	void Corruption2Update(float elapsedTime);
	// �j��
	void Destroy();

	
private:
	DirectX::XMFLOAT4  position = { 0,0,0,1};
	DirectX::XMFLOAT3  pos = {0,0,0};
	DirectX::XMFLOAT3  direction = { 0,0,1 };
	DirectX::XMFLOAT4  scale = {1,1,1,1};
	float lifeTimer = { 0 };
	float texTimer = 0;
	float texTimerMax = 120;
	float speed = 0;
	int type = 0;
	int spriteType = 0;
	float radius = 0.0;
	float height = 1;
	float angleX = 0;
	float angleY = 0;
	float randX = 0;
	float leng = 2;
	DirectX::XMFLOAT4 uvStatus = { 1,0,1,0};

	DirectX::XMFLOAT4	materialColor = { 1.0,0.0,0.0,1 };

};