#pragma once
#include "Model.h"
#include "Object.h"
#include "LightManager.h"
#include "AudioSource.h"
//タイプ取得
// 0   直進
// 1   回転
// 2   列
// 3   剣位置
// 4   ステンガ―
// 5   武器交換
enum  Type
{
	Straight,
	Rotate,
	Column,
	Sword,
	Stinger,
	ChangeWp,
	Beem,
	NULLType,
};

//直進弾丸
class ProjectileStraite :public Object
{
public:
	ProjectileStraite(ObjectManager* manager);
	~ProjectileStraite() override;

	//更新処理
	void Update(float elapsedTime)override;


	//描画処理
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;

	void TrailRender(RenderContext& rc, SwordTrailShader* shader)override;
	void SetScale(DirectX::XMFLOAT3 scale) { this->scale = scale; };
	//発射
	void Launch(std::shared_ptr <Model> buffer,float height, float dissolveSpeed, float angle, int type,int texType, float lifeTimer, float damage, float invincibleTime, bool leftflag = false);
	void TurretLaunch(std::shared_ptr <Model> buffer, float height, float dissolveSpeed, DirectX::XMFLOAT3 turretPosition, DirectX::XMFLOAT3 targetPosition, float angle, int type, int texType, float lifeTimer, float damage, float invincibleTime);
private:
	void StraightUpdate(float elapsedTime);

	void RotateUpdate(float elapsedTime);

	void ColumnUpdate(float elapsedTime);

	void SwordUpdate(float elapsedTime);

	void StingerUpdate(float elapsedTime);

	void ChangeWpUpdate(float elapsedTime);

	void BeemUpdate(float elapsedTime);
private:
	std::shared_ptr<Model> model = { nullptr };
	std::shared_ptr<SwordTrail> Trail;
	DirectX::XMFLOAT4 uvStatus = {0,1,0,0};
	float speed = 34.0f;
	float rotatespeed = 4.0f;
	float changeWpSpeed = 10.0;
	float dissolveSpeed = 3.0f;
	float positionY = 0.0f;
	float angle = 0;
	float Lenght = 1.0;
	float rotationangle = 6.0;
	float dissolveTimer = 0;
	//地面判定
	float under = 0.5;
	//左右判定
	bool leftflag = false;
	bool effectflag = false;
	std::vector<SkinnedMeshResouurce::constants> data;
	DirectX::XMFLOAT4	materialColor = { 1.0,1.0,1.0,1 };
	Light* light;
	std::unique_ptr<AudioSource> se;
};