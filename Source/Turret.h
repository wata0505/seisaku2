#pragma once
#include<DirectXMath.h>
#include "shader.h"
#include "Model.h"
#include "Trap.h"

class Turret:public Trap
{
public:
	Turret();
	~Turret();
public:
	//更新処理
	void Update(float elapsed_time)override;
	// 描画処理
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;
	
	// デバッグプリミティブ描画
	void DrawDebugPrimitive()override;


	
private:
	//行列更新処理
	void UpdateTransform2(int axisType, int lengthType);

	// 待機ステートへ遷移
	void TransitionIdleState();

	// 待機ステート更新処理
	void UpdateIdleState(float elapsedTime);

	// 攻撃ステートへ遷移
	void TransitionAttackState();

	// 攻撃ステート更新処理
	void UpdateAttackState(float elapsedTime);

private:
	enum  TurretAnimation
	{
		Attack,
	};
	// ステート
	enum class State
	{
		Idle,
		Attack,
	};

	State state = State::Idle;

	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<Model> model2 = nullptr;
	//描画情報格納
	std::vector<SkinnedMeshResouurce::constants> renderdata;
	std::vector<SkinnedMeshResouurce::constants> renderdata2;

	DirectX::XMFLOAT4X4		transform2 = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

};