#include"FireBall.h"
#include"StageManager.h"
#include"Light.h"
#include"AudioAll.h"
// コンストラクタ
FireBall::FireBall(ObjectManager* manager,float size)
	:Object(manager)//基底クラスのコンストラクタ呼び出し
{

	//表示サイズを調整
	radius =size;
	type = 0;
	handle = EffectAll::Instance().fireBallEffect->Play(position,size);
	//EffectAll::Instance().swordEffect->SetScale(handle, { 0.5,0.5,0.5 });
	//ライト設定
	light = new Light(position,0,1,1,1);
	LightManager::Instance().Register(light);
	damage = 2;
	invincibleTime = 2;
}

// デストラクタ
FireBall::~FireBall()
{
	EffectAll::Instance().fireBallEffect->Stop(handle);
}

//更新処理
void FireBall::Update(float elapsedTime)
{
	//寿命処理
	lifeTimer -= elapsedTime;
	if (lifeTimer < 0)
	{
		//自分を削除
		Destroy();
		light->Destroy();
	}
	//移動
	switch (type)
	{
	case FireBallType::FirBallStraight:
		StraightUpdate(elapsedTime);
		break;
	}
	light->SetParameter(position,100, 2, 1, 0);//ライト更新
	//オブジェクト行列を更新
	UpdateTransform();

}
void FireBall::StraightUpdate(float elapsedTime) {
	float speed = this->speed * elapsedTime;
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;

	DirectX::XMFLOAT3 start = { position.x,position.y,position.z };
	// レイの終点位置は移動後の位置
	DirectX::XMFLOAT3 end = { position.x + direction.x,position.y + direction.y,position.z + direction.z };

	// レイキャストによる地面判定
	HitResult hit;
	//if (health > 0) {
	if (StageManager::Instance().RayCast(start, end, hit)) {//地面に着弾したら
		EffectAll::Instance().swordEffect->Play(hit.position, 0.4);
		EffectAll::Instance().fireBallEffect->Stop(handle);
		AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::DragonFBE)->Play(false, SE);
		direction = { NULL,NULL,NULL};
		lifeTimer = 1.0f;
	}

	EffectAll::Instance().fireBallEffect->SetPosition(handle, position);
	EffectAll::Instance().fireBallEffect->SetRotation(handle, { 0,angle,0 });

}

//描画処理
void FireBall::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
}


//発射
void FireBall::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float angle)
{
	this->direction = direction;
	this->position = position;
	this->angle = angle;
	this->lifeTimer = lifeTimer;
	UpdateTransform();

}