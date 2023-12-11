#pragma once
#pragma once
#include "Model.h"
#include "Object.h"
#include "EffectAll.h"
#include "LightManager.h"
//タイプ取得
// 0   直進
// 1   回転
// 2   列
enum  FireBallType
{
	FirBallStraight,
	FirBallNULLType,
};

//直進弾丸
class FireBall :public Object
{
public:
	FireBall(ObjectManager* manager,float size = 0.3f);
	~FireBall() override;

	//更新処理
	void Update(float elapsedTime)override;


	//描画処理
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;

	void TrailRender(RenderContext& rc, SwordTrailShader* shader)override {};

	//発射
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float angle);
private:
	void StraightUpdate(float elapsedTime);

	
private:
	Effekseer::Handle handle;
	float speed = 20.0f;
	float lifeTimer = 4.0f;
	float angle = 0;
	Light* light;
};