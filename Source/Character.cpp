#include"Character.h"
#include"StageManager.h"
#include"Mathf.h"
#include"Base.h"

//行列更新処理
void Character::UpdateTransform(int axisType,int lengthType)
{
	const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
		{ -1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},		//0:RHS Y-UP　右手系
		{  1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},		//1:LHS Y-UP　左手系
		{ -1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1},		//2:RHS Z-UP　右手系
		{  1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},		//3:LHS Z-UP　左手系
	};

	const float scale_factor[] = { 0.01f,1.0f };

	DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[axisType]) * DirectX::XMMatrixScaling(scale_factor[lengthType],scale_factor[lengthType],scale_factor[lengthType]) };
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//回転行列を作成
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	//DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	//DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	//DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	//DirectX::XMMATRIX R = Y * X * Z;
	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	//3つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = C * S * R * T;
	//計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

// ダメージを与える
bool Character::ApplyDamage(int damage, float invincibleTime, bool DamageFlag)
{
	// ダメージが0の場合は健康状態を変更する必要がない
	if (damage <= 0)
		return false;

	// 死亡している場合は健康状態を変更しない
	if (health <= 0)return false;
	saveDamage = 0;
	if (invincibleTimer <= 0) {
		// ダメージ処理
		health -= damage;
		saveDamage = damage;
		invincibleTimer = invincibleTime;
		damageFlag = DamageFlag;
	}
	else {
		return false;
	}

	// 死亡通知
	if (health <= 0)
	{
		OnDead();
	}
	// ダメージ通知
	else {
		OnDamaged();
	}

	// 健康状態が変更した場合はtrueを返す
	return true;
}
void Character::SetInvincibleTime(float invincibleTime) {
	invincibleTimer = invincibleTime;
}

// 衝撃を与える
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	// 速力に力を加える
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;

}

// ノックバック
void Character::KnockBack(Character& taisyou, DirectX::XMFLOAT3 mypos, DirectX::XMFLOAT3 targetpos, float power, float powerY)
{
	DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&mypos);
	DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&targetpos);
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(B, A);
	DirectX::XMVECTOR Nvec = DirectX::XMVector3Normalize(Vec);
	Nvec = DirectX::XMVectorScale(Nvec, power / taisyou.GetMass());
	DirectX::XMFLOAT3 inpulse;
	DirectX::XMStoreFloat3(&inpulse, Nvec);
	inpulse.y = powerY / taisyou.GetMass();
	taisyou.AddImpulse(inpulse);
}

//移動処理
void Character::MoveInput(float vx, float vz, float speed)
{
	// 移動方向ベクトルを設定
	moveVecX = vx;
	moveVecZ = vz;
	// 最大速度処理
	maxMoveSpeed = speed;
}

//旋回処理
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	float rotateSpeed = speed * elapsedTime;

	//進行ベクトルがゼロベクトルの場合は処理する必要なし
	float Length = sqrtf((vx * vx) + (vz * vz));
	if (Length == 0.0f) return;

	//進行ベクトルを単位ベクトル化
	vx /= Length;
	vz /= Length;

	//自身の回転値から前方向を求める
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
//垂直旋回処理
void Character::VerticalTurn(float elapsedTime, float vx, float vz, float vy, float speed)
{
	float rotateSpeed = speed * elapsedTime;

	//進行ベクトルがゼロベクトルの場合は処理する必要なし
	float Length = sqrtf((vy * vy));
	if (Length == 0.0f) return;

	//進行ベクトルを単位ベクトル化
	vx /= Length;
	vz /= Length;
	//vy /= Length;

	//自身の回転値から前方向を求める
	float frontX = cosf(angle.x)*sinf(angle.y);
	float frontY = sinf(angle.x);
	float frontZ = cosf(angle.x)*cosf(angle.y);
	float a = 1.0 - (vy * vy);
	// 回転角を求めるため、２つの単位ベクトルの内積を計算する
	float dot = -(frontY * vy) + a;

	// 内積値は-1.0~1.0で表現されており、２つの単位ベクトルの角度が
	// 小さいほど1.0に近づくという性質を利用して回転速度を調整する
	float rot = 1.0f - dot;
	if (rotateSpeed > rot)rotateSpeed = rot;

	// 2Dの外積値が正の場合か負の場合によって左右判定が行える
	// 左右判定を行うことによって左右回転を選択する
	
	angle.x += rotateSpeed;

}
void Character::JumpInput(float speed)
{
	// 上方向の力を設定
	velocity.y = speed;
	jumpflag = true;
}

//速度処理更新
void Character::UpdateVelocity(float elapsedTime)
{
	// 経過フレーム
	float elapsedFrame = 60.0f * elapsedTime;

	// 垂直速力更新処理
	UpdateVerticalVelocity(elapsedFrame);

	// 水平速力更新処理
	UpdateHorizontalVelocity(elapsedFrame);

	// 垂直移動更新処理
	UpdateVerticalMove(elapsedTime);

	// 水平移動更新処理
	UpdateHorizontalMove(elapsedTime);

}

void Character::UpdateInvincibleTimer(float elapsedTime)
{
	if (invincibleTimer > 0.0f)
	{
		invincibleTimer -= elapsedTime;
	}
	else
	{
		damageFlag = false;
	}
}

// 垂直速力更新処理
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
	// 重力処理
	velocity.y += gravity * elapsedFrame;
}

// 垂直移動更新処理
void Character::UpdateVerticalMove(float elapsedTime)
{
	// 垂直方向の移動量
	float my = velocity.y * elapsedTime;

	slopeRate = 0.0f;

	// キャラクターのY軸方向となる法線ベクトル
	DirectX::XMFLOAT3 normal = { 0,1,0 };

	// 落下中
	if (my < 0.0f)
	{
		// レイの開始位置は足元より少し上
		DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
		// レイの終点位置は移動後の位置
		DirectX::XMFLOAT3 end = { position.x,position.y + my - 0.1f,position.z };
		if (velocity.x == 0 && velocity.z == 0) {
			if (isGround) return;
		}
		 
		// レイキャストによる地面判定
		HitResult hit;
		//if (health > 0) {
			if (StageManager::Instance().RayCast(start, end, hit))
		    //if (position.y < 0.1)
			{
				// 法線ベクトル取得
				normal = hit.normal;

				// 地面に接地している
				position = hit.position;
				//position.y = 0;
				// 傾斜率の計算
				//float teihen = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
				//slopeRate = hit.normal.y / (teihen + hit.normal.y);

				// 着地した
				if (!isGround)
				{
					OnLanding();
					jumpflag = false;
				}
				isGround = true;
				velocity.y = 0.0f;
			}
			else
			{
				// 空中に浮いている
				position.y += my;
				isGround = false;
			}
			if (position.y < deathHight) {
				OnDead();
				health = 0;
				fallDeath = true;
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
		//if (health > 0) {
		//	if (StageManager::Instance().RayCast(start, end, hit))
		//	{
		//		// 地面からの距離が離れすぎていれば上昇停止
		//		if (hit.position.y + altitude < position.y) {
		//			//position.y = hit.position.y + altitude;
		//			my = 0.0f;
		//		}			
		//	}
		//	
		//}
		position.y += my;
		isGround = false;
	}

	// 地面の向きに沿うようにXZ軸回転
	{
		//Y軸が法線ベクトル方向に向くオイラー角回転を算出する
		//angle.x = atan2f(normal.z, normal.y);
		//angle.z = -atan2f(normal.x, normal.y);
		//float anglex = atan2f(normal.z, normal.y);
		//float anglez = -atan2f(normal.x, normal.y);
		//// 線形補完で滑らかに回転する
		//angle.x = Mathf::Lerp(angle.x, anglex, 0.2f);
		//angle.z = Mathf::Lerp(angle.z, anglez, 0.2f);

	}
	
}

// 水平速力更新処理
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	DirectX::XMFLOAT3 outPos;
	if (StageManager::Instance().PillarVS(position, radius, height, outPos)) {
		position = outPos;
	}
	if (Base::Instance().PillarVS(position, radius, height, outPos)) {
		position = outPos;
	}
	// XZ平面の速力を減速する
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// 摩擦力
		float friction = this->friction * elapsedFrame;

		// 摩擦による横方向の減速処理
		if (length > friction)
		{
			//単位ベクトル化
			float vx = velocity.x / length;
			float vz = velocity.z / length;

			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
		}
		// 横方向の速力が摩擦力以下になったので速力を無効化
		else
		{
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}
	// XZ平面の速力を加速する
	if (length <= maxMoveSpeed)
	{
		// 移動ベクトルがゼロベクトルでないなら加速する
		float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		if (moveVecLength > 0.0f)
		{
			//加速力
			float acceleration = this->acceleration * elapsedFrame;
			//移動ベクトルによる加速処理
			velocity.x += moveVecX * acceleration;
			velocity.z += moveVecZ * acceleration;

			//最大速度処理
			float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
			if (length > maxMoveSpeed)
			{
				float vx = velocity.x / length;
				float vz = velocity.z / length;

				velocity.x = vx * maxMoveSpeed;
				velocity.z = vz * maxMoveSpeed;
			}
			// 下り坂でガタガタしないようにする
			if (isGround && slopeRate > 0.0f)
			{
				velocity.y -= slopeRate * elapsedFrame;
			}


		}
	}

	// 移動ベクトルをリセット
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

// 水平移動更新処理
void Character::UpdateHorizontalMove(float elapsedTime)
{
	//// 移動処理
	//position.x += velocity.x * elapsedTime;
	//position.z += velocity.z * elapsedTime;

	// 水平速力量計算
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		// 水平移動値
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;
		// レイの開始位置と終点位置
		DirectX::XMFLOAT3 start = { position.x,position.y + height *0.5f,position.z };
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

	}
}

// ホログラムシェーダー情報初期化
void Character::HologramShaderDataInitialize(float minHeight, float maxHeight)
{
	scanTimer = 0.0f;		// スキャンラインの時間
	scanBorder = 0.0f;		// スキャンラインの描画範囲
	glowTimer = 0.0f;		// グロウラインの時間
	glowBorder = 0.0f;		// グロウラインの描画範囲
	hologramTimer = 0.0f;	// ホログラムラインの時間
	hologramBorder = 0.0f;	// ホログラムラインの描画範囲
	this->minHeight = minHeight;	// 最低点
	this->maxHeight = maxHeight;	// 最高点
	isActiveStart = false;	// ホログラムシェーダー実行中フラグ
}
// ホログラムシェーダー更新処理
bool Character::UpdateHologramShader(float elapsedTime)
{
	// minHeight : 足元位置
	// maxHeight : 頭長位置
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
	// スキャンラインの時間加算(倍速)
	scanTimer += elapsedTime * 2.0f;
	// スキャンラインの時間が許容時間以上なら固定
	if (scanTimer >= permissionTimer)
	{
		scanTimer = permissionTimer;
	}
	// スキャンラインの時間が一定値(0.4f)以上なら
	if (scanTimer >= 0.4f)
	{
		// グロウラインの時間加算(3/4倍速)
		glowTimer += elapsedTime * 0.75f;
		// グロウラインの時間が許容時間以上なら固定
		if (glowTimer >= permissionTimer)
		{
			glowTimer = permissionTimer;
		}
	}
	// グロウラインの時間が一定値(0.4f)以上なら
	if (glowTimer >= permissionTimer)
	{
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

	// ホログラムラインの時間が許容時間以上なら固定
	if (hologramTimer >= permissionTimer)
	{
		return true;
	}
	return false;
}