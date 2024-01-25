#include"InstancingSprite.h"
#include"InstancingSpriteManager.h"
#include"Player.h"
#include"Enemy.h"
#include"EnemyManager.h"

// コンストラクタ
InstancingSprite::InstancingSprite()//基底クラスのコンストラクタ呼び出し
{

	//表示サイズを調整
	scale.x = scale.y = scale.z = 0.2f;
	//scale.x = scale.y = scale.z = 1.0f;
	randX = rand() % 20 - 10;
}

// デストラクタ
InstancingSprite::~InstancingSprite()
{

}

//更新処理
void InstancingSprite::Update(float elapsedTime)
{
	//寿命処理

	lifeTimer -= elapsedTime;
	if (lifeTimer < 0)
	{
		//自分を削除
		uvStatus.x -= elapsedTime;
		if (uvStatus.x < 0) {
			Destroy();
		}
	}
	//移動
	switch (type)
	{
	case InstancingSpriteType::FlameBreath:
		FlameBreathUpdate(elapsedTime);
		break;
	case InstancingSpriteType::SeirlConvergence:
		SeirlConvergenceUpdate(elapsedTime);
		break;
	case InstancingSpriteType::VortexDiffusion:
		VortexDiffusionUpdate(elapsedTime);
		break;
	case InstancingSpriteType::Corruption:
		CorruptionUpdate(elapsedTime);
		break;
	case InstancingSpriteType::Corruption2:
		Corruption2Update(elapsedTime);
		break;
	}


}
void InstancingSprite::FlameBreathUpdate(float elapsedTime) {
	float speed = this->speed * elapsedTime;
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;
	scale.x += speed;
	scale.y += speed;
	texTimer += speed;//テクスチャアニメーション
	if (texTimer > texTimerMax) {
		texTimer = 0;
	}
	uvStatus.z += elapsedTime;//UVアニメーション
	uvStatus.y = 1.5;//ディゾルブ値
	radius += speed * 0.4;//当たり判定はスピードの半分
}

void InstancingSprite::SeirlConvergenceUpdate(float elapsedTime) {
	scale.x = scale.y = scale.z = 0.03f;
	float speed = this->speed * elapsedTime;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 Vec;
	angleY += DirectX::XMConvertToRadians(randX);
	angleX += DirectX::XMConvertToRadians(randX);
	//angleY += DirectX::XMConvertToRadians(10);
	//angleX += DirectX::XMConvertToRadians(10);
	Vec.y = sin(angleY);
	Vec.x = cosf(angleY) * sinf(angleX);
	Vec.z = cosf(angleY) * cosf(angleX);


	position.x = direction.x + Vec.x * leng;
	position.y = direction.y + Vec.y * leng;
	position.z = direction.z + Vec.z * leng;
	leng -= speed*0.1;
	if (leng < 0.2) {
		Destroy();
	}
	
	
	uvStatus.z += elapsedTime;//UVアニメーション
	uvStatus.y = 1.5;//ディゾルブ値
	radius += speed * 0.4;//当たり判定はスピードの半分
}

void InstancingSprite::VortexDiffusionUpdate(float elapsedTime) {
	//scale.x = scale.y = scale.z = 0.1f;

	float speed = this->speed * elapsedTime;
	scale.x += speed * 0.01;
	scale.y += speed * 0.01;
	scale.z += speed * 0.01;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 Vec;
	angleY += DirectX::XMConvertToRadians(10);
	Vec.y = sin(angleY);
	Vec.x = cosf(angleY) * sinf(angleX);
	Vec.z = cosf(angleY) * cosf(angleX);


	position.x = direction.x + Vec.x * leng;
	position.y = direction.y + Vec.y * leng;
	position.z = direction.z + Vec.z * leng;
	leng += speed * 0.3;


	uvStatus.z += elapsedTime;//UVアニメーション
	uvStatus.y = 1.5;//ディゾルブ値
	radius += speed * 0.4;//当たり判定はスピードの半分
}

void InstancingSprite::CorruptionUpdate(float elapsedTime) {
	//scale.x = scale.y = scale.z = 0.1f;
	Enemy* enemy = EnemyManager::Instance().GetEnemyFromId(angleX);
	float speed = this->speed * elapsedTime;
	//scale.x += speed * 0.01;
	//scale.y += speed * 0.01;
	//scale.z += speed * 0.01;


	//position.x += direction.x * speed;
	//position.y += direction.y * speed;
	//position.z += direction.z * speed;
	//leng += speed * 0.3;
	position.x = enemy->GetPosition().x + direction.x * 3;
	position.y += speed;
	position.z = enemy->GetPosition().z + direction.z * 3;

	//uvStatus.z += elapsedTime;//UVアニメーション
	//uvStatus.y = 1.5;//ディゾルブ値
	radius = 0;//当たり判定はスピードの半分
}

void InstancingSprite::Corruption2Update(float elapsedTime) {
	//scale.x = scale.y = scale.z = 0.1f;
	float speed = this->speed * elapsedTime;
	//scale.x += speed * 0.01;
	//scale.y += speed * 0.01;
	//scale.z += speed * 0.01;


	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;
	//leng += speed * 0.3;

	//uvStatus.z += elapsedTime;//UVアニメーション
	//uvStatus.y = 1.5;//ディゾルブ値
	radius = 0;//当たり判定はスピードの半分
}



void InstancingSprite::Destroy()
{
	InstancingSpriteManager::Instance().Remove(this);
}


//発射
void InstancingSprite::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, int type, int spriteType, float lifeTimer, float speed,float angleX,float angleY, DirectX::XMFLOAT4 materialColor)
{
	this->position = { position.x,position.y,position.z, 1};
	this->direction = direction;
	this->type = type;
	this->lifeTimer = lifeTimer;
	this->speed = speed;
	this->materialColor = materialColor;
	this->spriteType = spriteType;
	this->angleX = angleX;
	this->angleY = angleY;
	texTimer = -1;
	if (type == VortexDiffusion) {
		leng = 0;
	}
}