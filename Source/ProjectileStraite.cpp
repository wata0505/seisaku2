#include"ProjectileStraite.h"
#include"Player.h"
#include"Camera.h"
#include"StageManager.h"
#include"AudioAll.h"
#include"Audio.h"
#include"Calculation.h"
#include"EffectTexAll.h"
#include"ParticleSystem.h"
#include"ParticleSprite.h"
#include"ParticleSystem.h"
// コンストラクタ
ProjectileStraite::ProjectileStraite(ObjectManager* manager)
	:Object(manager)//基底クラスのコンストラクタ呼び出し
{
	//表示サイズを調整
	scale.x = scale.y = scale.z = 1.2;
	//scale.x = scale.y = scale.z = 1.0f;
	radius = 0.5f;
	type = 1;
	se = Audio::Instance().LoadAudioSource("resources\\Audio\\sword.wav");

}

// デストラクタ
ProjectileStraite::~ProjectileStraite()
{
	light->Destroy();
	//AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Sword)->Stop();
	//AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Sift)->Stop();
}

//更新処理
void ProjectileStraite::Update(float elapsedTime)
{
	//寿命処理
	lifeTimer -= elapsedTime;
	if (lifeTimer < 0)
	{
		//自分を削除
		uvStatus.y += dissolveSpeed * elapsedTime;
		if(uvStatus.y > 1)
		{
			Destroy();
		}
	}
	//移動
	switch (type)
	{
	case Type::Straight:
		StraightUpdate(elapsedTime);
		break;
	case Type::Rotate:
		RotateUpdate(elapsedTime);
		break;
	case Type::Column:
		ColumnUpdate(elapsedTime);
		break;
	case Type::Sword:
		SwordUpdate(elapsedTime);
		break;
	case Type::Stinger:
		StingerUpdate(elapsedTime);
		break;
	case Type::ChangeWp:
		ChangeWpUpdate(elapsedTime);
		break;
	case Type::Beem:
		BeemUpdate(elapsedTime);
	
		break;
	}
	
	
	//Trail->Update();
	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	model->UpdateWorldBufferData(transform);
	uvStatus.x += elapsedTime;
	if (lifeTimer > 0) {
		uvStatus.y -= dissolveSpeed * elapsedTime;
		if (uvStatus.y < NULL) {
			uvStatus.y = NULL;
		}
	}
	
}
void ProjectileStraite::StraightUpdate(float elapsedTime){
	float speed = this->speed * elapsedTime;
	DirectX::XMFLOAT3 start = { position.x,position.y,position.z };
	//飛ぶ斬撃
	position = Vector3::PosDir(position, direction, speed);
	
	scale.x += elapsedTime*speed; 
	scale.y += elapsedTime*speed; 
	scale.z += elapsedTime*speed;
	uvStatus.z = 0;
	if (angle < 0) {
		ParticleSprite* particleSprite = new ParticleSprite(position, direction, ParticleSprite::ParticleImpact, ParticleSprite::Expansion, int(EffectTexAll::EfTexAll::Impact), 1, 0.8,rand() % 100 *0.001);
		angle = 0.2;
		ParticleSystem::Instance().RubbleEffect({ position.x,position.y -5,position.z }, 10, 100);
	}
	ParticleSystem::Instance().Rubble2Effect({ position.x,position.y,position.z }, 2, 100);
	ParticleSystem::Instance().VortexDiffusionEffect(position, 90);
	ParticleSystem::Instance().VortexDiffusionEffect(position, 180);
	ParticleSystem::Instance().VortexDiffusionEffect(position, 270);
	ParticleSystem::Instance().VortexDiffusionEffect(position, 45);
	ParticleSystem::Instance().VortexDiffusionEffect(position, 135);
	ParticleSystem::Instance().VortexDiffusionEffect(position, 225);
	angle -= elapsedTime;
	// レイキャストによる壁判定
	HitResult hit;
	if (StageManager::Instance().RayCast(start, { position.x + direction.x,position.y,position.z + direction.z}, hit))
	{
		ParticleSprite* particleSprite = new ParticleSprite(start, {0,0,0}, ParticleSprite::ParticleSoft, ParticleSprite::Diffusion, int(EffectTexAll::EfTexAll::Sumi), 10000, 2.5, 0, true, 0.008, 0.15, { 1,1,1,1 });
		Destroy();
	}
	//light->SetPos(position, 50, 3, 3, 3);
	//uvStatus.y = NULL;
}
void ProjectileStraite::RotateUpdate(float elapsedTime) {
	Player& player = Player::Instance();
	
	//周囲回転
		Lenght = 1.2;
		DirectX::XMFLOAT3 pos = Player::Instance().GetPosition();
		direction.x = sinf(angle);
		direction.y = 0.0f;
		direction.z = cosf(angle);
		position.x = pos.x + direction.x * Lenght;
		position.y = pos.y + Player::Instance().GetHeight() * 0.5;
		position.z = pos.z + direction.z * Lenght;
		angle += rotatespeed * elapsedTime;
		radius = 1.0;
	//}
		
}
void ProjectileStraite::ColumnUpdate(float elapsedTime) {
	if (lifeTimer > 1.0) {//一定時間待機
		damage = 0;
		rotatespeed += 2;
		Player& player = Player::Instance();
		DirectX::XMFLOAT3 pos = player.GetPosition();
		angle += rotatespeed * elapsedTime;
		position.y = positionY + pos.y;
		direction.y = 0;
		if (leftflag) {//右側か左側か
			position.x = pos.x - direction.z * Lenght;
			position.z = pos.z + direction.x * Lenght;
		}
		else {
			radius = 0.5;
			position.x = pos.x + direction.z * Lenght;
			position.z = pos.z - direction.x * Lenght;
		}

		if (player.GetLockOnflag()) {
			
			direction = Vector3::Normalize(Vector3::Subset(player.GetLockEnemyPos(), position));
		}	
	}
	else
	{
		damage = 1;
		float speed = this->speed * (elapsedTime);
		position = Vector3::PosDir(position, direction, speed);
	}
	DirectX::XMFLOAT3 up = {0,0,0};
	//モデル上方向計算
	up.x = cosf(rotationangle) * -direction.z;
	up.y = sinf(rotationangle);
	up.z = cosf(rotationangle) * direction.x;
	directionUp = { up.x,up.y,up.z};
	rotationangle += angle * elapsedTime;
	
}

void ProjectileStraite::StingerUpdate(float elapsedTime) {

	//ステンガーエフェクト
		Player& player = Player::Instance();
		DirectX::XMFLOAT3 pos = player.GetHeadPos();
		float angleY = player.GetAngle().y;
	
		Lenght = 0.5;
		direction.x = cosf(angle) * sinf(angleY);
		direction.y = sinf(angle);
		direction.z = cosf(angle) * cosf(angleY);
		
		position.x = pos.x - direction.z * Lenght;
	    position.y = pos.y + direction.y * Lenght;
		position.z = pos.z + direction.x * Lenght;
		direction = Vector3::Normalize(Vector3::Subset(player.GetTailPos(), position));
		angle += 0.3;

}
void ProjectileStraite::SwordUpdate(float elapsedTime) {
	Player& player = Player::Instance();
	float speed = this->speed * elapsedTime;
	if (lifeTimer > 2.0) {//一定時間待機
		direction = { 0,1,0 };
		DirectX::XMFLOAT3 up = { 0,0,0 };
		up.x = sinf(rotationangle);
		up.z = cosf(rotationangle);
		directionUp = { up.x,0,up.z };
		rotationangle += rotatespeed * elapsedTime;
		position.y += speed;
		angle = DirectX::XMConvertToRadians(90);
	}
	else if (angle > DirectX::XMConvertToRadians(-45))
	{
		angle -= DirectX::XMConvertToRadians(1.5f);
		direction.x = speed * cosf(angle) * sinf(Lenght);
		direction.y = speed * sinf(angle);
		direction.z = speed * cosf(angle) * cosf(Lenght);
		directionUp.x = speed * cosf(angle + DirectX::XMConvertToRadians(90)) * sinf(Lenght);
		directionUp.y = speed * sinf(angle + DirectX::XMConvertToRadians(90));
		directionUp.z = speed * cosf(angle + DirectX::XMConvertToRadians(90)) * cosf(Lenght);
	}
	else
	{
		this->speed = 40;
		if (position.y < under) {
			if (!effectflag) {
				ParticleSystem::Instance().BoomEffect(position, 3, int(EffectTexAll::EfTexAll::Thunder), 2,1,{ 2,NULL,NULL,1 });
				radius = 4;
				effectflag = true;
			}
		}
		else
		{
			position = Vector3::PosDir(position, direction, speed);
		}
	}

}
void ProjectileStraite::ChangeWpUpdate(float elapsedTime) {
	//武器チェンジ
	Player& player = Player::Instance();
	rotatespeed = 6.0f;
	Lenght = 1.2;
	radius = 1.0f;
	DirectX::XMFLOAT3 pos = Player::Instance().GetPosition();
	direction.x = sinf(angle);
	direction.y = 0.0f;
	direction.z = cosf(angle);
	position.x = pos.x - direction.z * Lenght;
	position.y = pos.y + Player::Instance().GetHeight() * 0.5 + positionY;
	position.z = pos.z + direction.x * Lenght;
	angle += changeWpSpeed * elapsedTime;
	changeWpSpeed -= 0.2;
	if (changeWpSpeed < 6)changeWpSpeed = 3;
	direction = { 0,1,0 };
	DirectX::XMFLOAT3 up = { 0,0,0 };
	up.x = sinf(rotationangle);
	up.z = cosf(rotationangle);
	directionUp = { up.x,0,up.z};
	rotationangle += rotatespeed * elapsedTime;
	//light->SetPos(position, 1, 1, 0.5, 0);
	if (this->positionY == 1) {
		light->SetParameter(position, 50, 2, 1, 0);
	}
}

void ProjectileStraite::BeemUpdate(float elapsedTime) {
	float scalerate = 4.0f;
	if (halfLife < lifeTimer) {
		scale.z += scalerate * 0.5;
		DirectX::XMVECTOR dir = DirectX::XMLoadFloat3(&direction);

		position.x += direction.x * scalerate * 29.0f * elapsedTime;
		position.y += direction.y * scalerate * 29.0f * elapsedTime;
		position.z += direction.z * scalerate * 29.0f * elapsedTime;
	}
	else
	{
		scale.z -= scalerate;
		if (scale.z < 0) scale.z = 0;
		DirectX::XMVECTOR dir = DirectX::XMLoadFloat3(&direction);

		position.x += direction.x * scalerate * 50.0f * elapsedTime;
		position.y += direction.y * scalerate * 50.0f * elapsedTime;
		position.z += direction.z * scalerate * 50.0f * elapsedTime;

	}
}
//描画処理
void ProjectileStraite::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	if (type != Type::Straight) {
		shader->Draw(dc, model.get(), uvStatus, materialColor);
	}
}

void ProjectileStraite::TrailRender(RenderContext& rc, SwordTrailShader* shader)
{
	//shader->Draw(rc, Trail.get(), {1,1,1,1});
}

//発射
void ProjectileStraite::Launch(std::shared_ptr <Model> buffer, float height, float dissolveSpeed, float angle, int type,int texType, float lifeTimer,float damage, float invincibleTime,bool leftflag)
{
    model = buffer;
	this->positionY = height;
	this->angle = angle;
	this->type = type;
	this->leftflag = leftflag;
	this->lifeTimer = lifeTimer;
	this->damage = damage;
	this->invincibleTime = invincibleTime;
	this->dissolveSpeed = dissolveSpeed;
	//this->rotationangle = Player::Instance().GetAngle().y;
	position = Player::Instance().GetPosition();
	direction = { Camera::Instance().GetFront().x,0, Camera::Instance().GetFront().z};
	position.y += height;
	UpdateTransform();
	Player& player = Player::Instance();
	if (type == Type::ChangeWp) {
		uvStatus.y = 0.2;
		if (this->positionY == 1) {
			light = new Light(position, 0, 1, 1, 1);
			LightManager::Instance().Register(light);
		}
		
	}
	if (type == Type::Straight) {
		uvStatus.y = 0.0;
		speed = 60;
		//light = new Light(position, 0, 1, 1, 1);
		//LightManager::Instance().Register(light);
		if (player.GetLockOnflag()) {
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&player.GetLockEnemyPos());
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMStoreFloat3(&direction, N);
		}
		radius = 3.0;
		ParticleSystem::Instance().ImpactEffect({ position.x,position.y,position.z },direction, int(EffectTexAll::EfTexAll::BlueThader), 80, { NULL,NULL,2,1 });
	}
	if (type == Type::Column) {
		direction.x = sinf(player.GetAngle().y);
		direction.z = cosf(player.GetAngle().y);
		se->Play(false,SE);
	}
	if (type == Type::Rotate) {
		se->Play(true, SE);
	}
	if (type == Type::Sword) {
		float angle = DirectX::XMConvertToRadians(rand() % 360);
		float length = rand() % 30;
		DirectX::XMFLOAT3 Vec;
		Vec.x = sinf(angle);
		Vec.z = cosf(angle);
		position.x += Vec.x * length;
		position.y -= 10;
		position.z += Vec.z * length;
		speed = 5;
		Lenght = player.GetAngle().y;
	}
	if (type == Type::Beem) {
		model =std::make_unique<Model>(".\\resources\\Cube.fbx", true);
		model->ModelSerialize(".\\resources\\Cube.fbx");
		model->ModelRegister(".\\resources\\Cube.fbx");
		direction.x = sinf(player.GetAngle().y);
		direction.z = cosf(player.GetAngle().y);
		if (leftflag) {//右側か左側か
			position.x = position.x - direction.z * Lenght;
			position.z = position.z + direction.x * Lenght;
		}
		else {
			radius = 0.5;
			position.x = position.x + direction.z * Lenght;
			position.z = position.z - direction.x * Lenght;
		}
		if (player.GetLockOnflag()) {
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&player.GetLockEnemyPos());
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMStoreFloat3(&direction, N);
		}
		halfLife = lifeTimer * 0.5;
		scale.x = scale.y = 0.1;
		scale.z = 0;
		UpdateTransform();
		model->UpdateBufferData(transform);
	}
	uvStatus.w = texType;
	uvStatus.z = 0.3;
}
//turret用発射
void ProjectileStraite::TurretLaunch(std::shared_ptr <Model> buffer, float height, float dissolveSpeed, DirectX::XMFLOAT3 turretPosition, DirectX::XMFLOAT3 targetPosition, float angle, int type, int texType, float lifeTimer, float damage, float invincibleTime)
{
	model = buffer;
	this->positionY = height;
	this->angle = angle;
	this->type = type;
	this->lifeTimer = lifeTimer;
	this->damage = damage;
	this->invincibleTime = invincibleTime;
	this->dissolveSpeed = dissolveSpeed;
	this->position = turretPosition;
	position.y += height;
	UpdateTransform();
	if (type == Type::Beem) {
		model = std::make_unique<Model>(".\\resources\\Cube.fbx", true);
		model->ModelSerialize(".\\resources\\Cube.fbx");
		model->ModelRegister(".\\resources\\Cube.fbx");

		DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&targetPosition);
		DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
		DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
		DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
		DirectX::XMStoreFloat3(&direction, N);

		halfLife = lifeTimer * 0.6;
		scale.x = scale.y = 0.2;
		scale.z = 0;
		UpdateTransform();
		model->UpdateBufferData(transform);
	}
	uvStatus.w = texType;
	uvStatus.z = 0.3;
}