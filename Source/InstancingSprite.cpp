#include"InstancingSprite.h"
#include"InstancingSpriteManager.h"
#include"Player.h"

// コンストラクタ
InstancingSprite::InstancingSprite()//基底クラスのコンストラクタ呼び出し
{

	//表示サイズを調整
	scale.x = scale.y = scale.z = 1.0f;
	//scale.x = scale.y = scale.z = 1.0f;
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

void InstancingSprite::Destroy()
{
	InstancingSpriteManager::Instance().Remove(this);
}


//発射
void InstancingSprite::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, int type, int spriteType, float lifeTimer, float speed, DirectX::XMFLOAT4 materialColor)
{
	this->position = { position.x,position.y,position.z, 1};
	this->direction = direction;
	this->type = type;
	this->lifeTimer = lifeTimer;
	this->speed = speed;
	this->materialColor = materialColor;
	this->spriteType = spriteType;
}