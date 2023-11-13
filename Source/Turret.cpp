#include"Turret.h"
#include "Graphics.h"
Turret::Turret()
{
	

	

	model = std::make_unique<Model>(".\\resources\\Sentorygun\\Taret_GUN.fbx", true);
	model->AppendAnimations(".\\resources\\Sentorygun\\GUN_motionBullet.fbx", 0);
	
	model->ModelSerialize(".\\resources\\Sentorygun\\Taret_GUN.fbx");
	model->ModelRegister(".\\resources\\Sentorygun\\Taret_GUN.fbx", "Taret_BODY.fbm\\TARET_GUN_BaseColor.png");
	
	model2 = std::make_unique<Model>(".\\resources\\Sentorygun\\Taret.fbx", true);
	model2->ModelSerialize(".\\resources\\Sentorygun\\Taret.fbx");
	model2->ModelRegister(".\\resources\\Sentorygun\\Taret.fbx", "Taret.fbm\\TARET_BODY_BaseColor.png");

	UpdateTransform(0, 0);
	UpdateTransform2(0, 0);

	model->UpdateBufferDara(transform);
	model2->UpdateBufferDara(transform2);

	renderdata = model->GetBufferData();
	renderdata2 = model2->GetBufferData();


	radius = 1.0f;
	notAttackRange=3.0f;

	scale.x = scale.y = scale.z = 80.0f;
	TransitionIdleState();
}
Turret::~Turret()
{

}

void Turret::Update(float elapsedTime)
{
	// ステート毎の更新処理
	switch (state)
	{
	case State::Idle:
		UpdateIdleState(elapsedTime);
		break;
	case State::Attack:
		UpdateAttackState(elapsedTime);
		break;
	}


	//SearchEnemy(territoryRange,attackRange);
	//TurnToTarget(elapsedTime, turnSpeed);


	UpdateTransform(0, 0);
	UpdateTransform2(0, 0);
	
	//アニメーション更新
	model->UpdateAnimation(elapsedTime, "root_x");
	model->UpdateSubAnimation(elapsedTime, "root_x");
	
	model->UpdateBufferDara(transform);
	model2->UpdateBufferDara(transform2);

	//モデル描画情報受け渡し
	renderdata = model->GetBufferData();
	renderdata2 = model2->GetBufferData();

}

void Turret::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	shader->Draw(dc, model.get());
	shader->Draw(dc, model2.get());
	
}

void Turret::DrawDebugPrimitive()
{
	// 基底クラスのデバッグプリミティブ描画
	Trap::DrawDebugPrimitive();

	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	//衝突判定用のデバック円柱を描画
	debugRenderer->DrawCylinder({ position.x,position.y -position.y,position.z } , radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

	//衝突判定用のデバック円柱を描画
	debugRenderer->DrawCylinder({ position.x,position.y - position.y,position.z }, notAttackRange, height, DirectX::XMFLOAT4(1, 0, 0, 1));


	// 縄張り範囲をデバッグ円柱描画
	debugRenderer->DrawCylinder({ territoryOrigin.x,territoryOrigin.y - territoryOrigin.y,territoryOrigin.z }, territoryRange, height, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}

//行列更新処理
void Turret::UpdateTransform2(int axisType, int lengthType)
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
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(0, 0, angle.z);
	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y - 1.5f, position.z);
	//3つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = C * S * R * T;
	//計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform2, W);
}


// 待機ステートへ遷移
void Turret::TransitionIdleState()
{
	state = State::Idle;
	model->PlayAnimation(-1, false);
}

// 待機ステート更新処理
void Turret::UpdateIdleState(float elapsedTime)
{
	// プレイヤー索敵
	if (SearchEnemy(territoryRange, notAttackRange))
	{
		// 見つかったら追跡ステートへ遷移
		TransitionAttackState();
	}
}

// 攻撃ステートへ遷移
void Turret::TransitionAttackState()
{
	state = State::Attack;
	model->PlayAnimation(static_cast<int>(TurretAnimation::Attack), true);
}

// 攻撃ステート更新処理
void Turret::UpdateAttackState(float elapsedTime)
{
	TurnToTarget(elapsedTime, turnSpeed);
	// 攻撃アニメーションが終わって範囲内に敵がいなければ
	if (SearchEnemy(territoryRange, notAttackRange) == false)
	{
		//待機ステートへ
		TransitionIdleState();
	}
}
