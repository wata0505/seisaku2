#include"Particle.h"
#include"ParticleManager.h"
#include"Player.h"
#include"Calculation.h"
#include"EnemyManager.h"
// コンストラクタ
Particle::Particle()//基底クラスのコンストラクタ呼び出し
{
	
	//表示サイズを調整
	scale.x = scale.y = scale.z = 2.5f;
	//scale.x = scale.y = scale.z = 1.0f;
}

// デストラクタ
Particle::~Particle()
{

}

//更新処理
void Particle::Update(float elapsedTime)
{
	//寿命処理
	
		lifeTimer -= elapsedTime;
		if (lifeTimer < 0)
		{
			if(dissolveTimer > dissolveMax){
			//自分を削除
			  Destroy();
			}
			dissolveTimer += elapsedTime;
		}
		//移動
		switch (type)
		{
		case ParticleType::Lance:
			LanceUpdate(elapsedTime);
			break;
		case ParticleType::Barrier :
			BarrierUpdate(elapsedTime);
			break;
		case ParticleType::BarrierEnemy:
			BarrierEnemyUpdate(elapsedTime);
			break;
		case ParticleType::Rubble:
			RubbleUpdate(elapsedTime);
			break;
		case ParticleType::Rubble2:
			Rubble2Update(elapsedTime);
			break;
		case ParticleType::Boom:
			BoomUpdate(elapsedTime);
			break;
		case ParticleType::Impact:
			ImpactUpdate(elapsedTime);
			break;
		}
	
	
	//Trail->Update();
	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	uvTimer += elapsedTime;

}
void Particle::LanceUpdate(float elapsedTime) {
	//ハルバート特殊
	for (MoveConstants& move : moves) {
		float speed = move.speed * elapsedTime;
		if (lifeTimer > 1.0) {//生存時間１以下なら止まる
			move.position.x += move.direction.x * speed;
			move.position.y += move.direction.y * speed;
			move.position.z += move.direction.z * speed;
		}
		DirectX::XMFLOAT3 up = { 0,0,0 };
		move.directionUp.x = -move.direction.z;
		move.directionUp.y = 0;
		move.directionUp.z = move.direction.x;
	}
	
}
void Particle::BarrierUpdate(float elapsedTime) {
	//float speed = this->speed[type] * elapsedTime;
	for (MoveConstants& move : moves) {
		move.position = Player::Instance().GetEfPos();
	}
	if (lifeTimer < 1) {
		//position.x += direction.x * speed;
		//position.y += direction.y * speed;
		//position.z += direction.z * speed;
	}
	//this->speed[type] -= elapsedTime;
	//materialColor.w -= elapsedTime*2;
	light->SetPos(moves.at(0).position, 10, materialColor.x, materialColor.y, materialColor.z);

}
void Particle::BarrierEnemyUpdate(float elapsedTime) {
	scale.x = scale.y = scale.z = 2.0f;
	for (MoveConstants& move : moves) {
		Enemy* enemy = EnemyManager::Instance().GetEnemyFromId(move.speed);
		if (enemy) {
			move.position = enemy->GetEfPos();
			lifeTimer = 1;
			if (!enemy->GetBarrierFlag())lifeTimer = 0;
			if (enemy->GetDamageFlag()) {
				materialColor.w = 0.5;
			}
			else
			{
				materialColor.w = 1.0;
			}
		}
		else
		{
			lifeTimer = 0;
		}
		
	}

}

void Particle::RubbleUpdate(float elapsedTime) {
	//岩拡散
	for (MoveConstants& move : moves) {
		float speed = move.speed * elapsedTime;

		move.position.x += move.direction.x * speed;
		move.position.y += move.direction.y * speed;
		move.position.z += move.direction.z * speed;

		DirectX::XMFLOAT3 up = { 0,0,0 };
		move.directionUp.x = -move.direction.z;
		move.directionUp.y = 0;
		move.directionUp.z = move.direction.x;
		
		move.direction.y += -elapsedTime;
		//move.speed -= elapsedTime;
	}
}
void Particle::Rubble2Update(float elapsedTime) {
	//岩拡散
	for (MoveConstants& move : moves) {
		float speed = move.speed * elapsedTime;
		if (move.position.y > -2.5) return;
		move.position.x += move.direction.x * speed;
		move.position.y += move.direction.y * speed;
		move.position.z += move.direction.z * speed;

		DirectX::XMFLOAT3 up = { 0,0,0 };
		move.directionUp.x = -move.direction.z;
		move.directionUp.y = 0;
		move.directionUp.z = move.direction.x;
	    
		move.direction.y += -elapsedTime;
		//move.speed -= elapsedTime;
	}
}
void Particle::BoomUpdate(float elapsedTime) {
	//爆発
	scale.x += elapsedTime * moves.at(NULL).speed;
	scale.y += elapsedTime * moves.at(NULL).speed;
	scale.z += elapsedTime * moves.at(NULL).speed;
	//ライト
	//light->SetPos(moves.at(0).position, 5 * moves.at(NULL).speed, materialColor.x, materialColor.y, materialColor.z);
}
void Particle::ImpactUpdate(float elapsedTime) {
	//爆発
	scale.x = scale.y = scale.z = 5.0f;
	for (MoveConstants& move : moves) {
		float speed = move.speed * elapsedTime;

		move.position.x += move.direction.x * speed;
		move.position.y += move.direction.y * speed;
		move.position.z += move.direction.z * speed;

	}
	//ライト
	//light->SetPos(moves.at(0).position, 5 * moves.at(NULL).speed, materialColor.x, materialColor.y, materialColor.z);
}
void Particle::Destroy()
{
	light->Destroy();
	ParticleManager::Instance().Remove(this);
}


//発射
void Particle::Launch(std::vector<MoveConstants> constants, int type, int modelType, int psType, int texType, float lifeTimer, DirectX::XMFLOAT4 materialColor)
{
	moves = constants;
	this->type = type;
	this->lifeTimer = lifeTimer;
	this->materialColor = materialColor;
	this->modelType = modelType;
	this->psType = psType;
	this->texType = texType;

	if (type == ParticleType::Boom) {
		scale.x = scale.y = scale.z = 0.0;
		//ライト設定
		//light = new Light(moves.at(0).position, 0, 1, 1, 1);
		//LightManager::Instance().Register(light);
	}
	if (type == ParticleType::Lance || type == ParticleType::Barrier) {
		//ライト設定
		light = new Light(moves.at(0).position, 0, 1, 1, 1);
		LightManager::Instance().Register(light);
		light->SetPos(Player::Instance().GetPosition(), 10, materialColor.x, materialColor.y, materialColor.z);
	}
}

void Particle::Render(ID3D11DeviceContext* dc, ParticleShader* shader) {
	ParticleManager& partic = ParticleManager::Instance();
	shader->Draw(dc,partic.GetModel(modelType).get(),data,particleNo,psType,texType);
}

// 行列更新処理
void Particle::UpdateTransform()
{
	//DirectX::XMVECTOR Front, Up, Right;
	particleNo= 0;
	//前ベクトルを算出
	for (MoveConstants& move : moves) {
		front = move.direction;
		front = Vector3::Normalize(front);
		//仮の上ベクトルを算出
		up = move.directionUp;
		up = Vector3::Normalize(up);
		//右ベクトルを算出
		right = Vector3::Cross(up, front);
		//上ベクトルを算出
		up = Vector3::Cross(front, right);
		//算出した軸ベクトルから行列を作成
		move.transform._11 = right.x * 0.01 * scale.x;
		move.transform._12 = right.y * 0.01 * scale.x;
		move.transform._13 = right.z * 0.01 * scale.x;
		move.transform._14 = 0.0f;
		move.transform._21 = up.x * 0.01 * scale.y;
		move.transform._22 = up.y * 0.01 * scale.y;
		move.transform._23 = up.z * 0.01 * scale.y;
		move.transform._24 = 0.0f;
		move.transform._31 = (front.x) * 0.01 * scale.z;
		move.transform._32 = (front.y) * 0.01 * scale.z;
		move.transform._33 = (front.z) * 0.01 * scale.z;
		move.transform._34 = 0.0f;
		move.transform._41 = move.position.x;
		move.transform._42 = move.position.y;
		move.transform._43 = move.position.z;
		move.transform._44 = 1.0f;
		//描画用配列に計算結果を入力していく
		ParticleManager::Instance().GetModel(modelType)->UpdateWorldBufferData(move.transform);
		data.world[particleNo] = ParticleManager::Instance().GetModel(modelType)->GetBufferTransform();
		data.materialColor[particleNo] = materialColor;//ディゾルブカラー
		data.dissolve[particleNo].x = dissolveTimer;//ディゾルブ状態
		data.dissolve[particleNo].y = uvTimer;//UVアニメーション
		particleNo++;
		//発射方向
		move.direction = front;
	}
}