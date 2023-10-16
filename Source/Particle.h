#pragma once
#include "Model.h"
#include "Object.h"
#include "ParticleShader.h"
#include "LightManager.h"

enum  ParticleType
{
	Lance,
	Barrier,
	BarrierEnemy,
	Rubble,
	Boom,
	Impact
};

//直進弾丸
class Particle
{
private:
	
public:
	enum ParticleModel
	{
		Lance,
		Rock1,
		Rock2,
		Rock3,
		Ball,

		Max
	};

	struct MoveConstants
	{
		DirectX::XMFLOAT3 position;			// ワールド行列
		DirectX::XMFLOAT3 direction;	// マテリアルカラ
		DirectX::XMFLOAT3 directionUp;
		DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		float             speed;
	};
public:
	Particle();
	~Particle();

	//更新処理
	void Update(float elapsedTime);


	//描画処理
	void Render(ID3D11DeviceContext* dc, ParticleShader* shader);


	DirectX::XMFLOAT4 GetMaterialColor() { return materialColor; }
	//パーティクル情報、動き、モデルタイプ、シェーダータイプ、テクスチャタイプ、生存時間、ディゾルブカラー
	void Launch(std::vector<MoveConstants> constant, int type, int modelType,int psType,int texType ,float lifeTimer,DirectX::XMFLOAT4 materialColor = {1,0,0,1});


	int GetModelType() { return modelType; }

	float GetDissolveTimer() { return dissolveTimer; }
private:
	//ハルバート特殊
	void LanceUpdate(float elapsedTime);

	void BarrierUpdate(float elapsedTime);

	void BarrierEnemyUpdate(float elapsedTime);
	//岩飛び散る
	void RubbleUpdate(float elapsedTime);
	//爆発
	void BoomUpdate(float elapsedTime);
	//衝撃
	void ImpactUpdate(float elapsedTime);

	void UpdateTransform();

	// 破棄
	void Destroy();

private:
	DirectX::XMFLOAT3  scale = { 1,1,1 };
	DirectX::XMFLOAT3 right = { 0,0,0 };
	DirectX::XMFLOAT3 up = { 0,0,0 };
	DirectX::XMFLOAT3 front = { 0,0,0 };

	float lifeTimer = {0};
	int type = 0;
	int modelType = 0;
	int psType = 0;
	int texType = 0;
	float dissolveTimer = 0;
	float uvTimer = 1;
	float dissolveMax = 1;
	std::vector<MoveConstants> moves;
	ParticleConstants data;
	int particleNo;
	DirectX::XMFLOAT4	materialColor = { 1.0,0.0,0.0,1 };
	Light* light;
	
}; 