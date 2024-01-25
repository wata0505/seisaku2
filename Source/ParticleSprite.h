#pragma once
//#include "Model.h"
//#include "Object.h"
//#include "ParticleSpriteShader.h"
#include "Shader.h"
struct PVConstants
{
	DirectX::XMFLOAT3 Position;     // 座標値
	DirectX::XMFLOAT3 Velocity;     // 速度
	DirectX::XMFLOAT4 ParticleSize; // サイズ
	float             Speed;        // 加速度
	int               type;         // パーティクルタイプ
	float             Timer;       

};

class ParticleSprite
{
public:
	ParticleSprite(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir, int spritetype, int movetype, int textype, int Max, float lifetimer, float lenght = 0,bool lifeflag = true,float speed = 0.05,float size = 0.005, DirectX::XMFLOAT4 materialColor = { 1.0,0.0,0.0,1 });
	~ParticleSprite();

	//更新処理
	void Update(float elapsedTime);


	//発射
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, int movetype, int spritetype, float lifeTimer, float speed, DirectX::XMFLOAT4 materialColor = { 1,0,0,1 });

	//パーティクル動きタイプ
	enum  MoveSpriteType
	{
		Slashing,          //斬撃
		Diffusion,		   //拡散
		Convergence,	   //収束
		Slash,			   //斬撃ヒット
		Expansion,		   //拡大
		WeponConvergence,  //武器収束
		Corruption,		   //煙
		Chile			   //塵
	};
	//パーティクルモデルタイプ
	enum  SpriteType
	{
		ParticleSoft,  //丸
		ParticleLine,  //線
		ParticleImpact,//衝撃
		ParticleWind,  //風
		ParticleSquare,//四角
		ParticleBurst,//バースト
		ParticleTriangle,//四角,
		ParticleHart,//四角,


		ParticleMax
	};
	
private:
	//武器収束初期設定
	HRESULT CreateWeponConvergenceResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
	//拡散初期設定
	HRESULT CreateDiffusionResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir,float speed, float lenght,float size);

	HRESULT CreateSlashingResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
	//収束初期設定
	HRESULT CreateConvergenceResource(DirectX::XMFLOAT3 pos);
	//斬撃エフェクト初期設定
	HRESULT CreateSlashResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
	//拡大初期設定
	HRESULT CreateExpansionResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir,float leght);
	//煙初期設定
	HRESULT CreateCorruptionResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
	//塵初期設定
	HRESULT CreateChileResource(DirectX::XMFLOAT3 pos);
	//リソース生成
	HRESULT CreateResource(PVConstants* posVertex);

	// 破棄
	void Destroy();

private:
	
	float lifeTimer = { 0 };
	float speed[5] = { 15.0f,7,7,0,0 };
	bool lifeflag = false;

	int type = 0;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> gPBuffer[2] = { NULL, NULL }; // バッファ リソース
	// 定数バッファフォーマット
	Microsoft::WRL::ComPtr<ID3D11Buffer> verBufferDrawFrom;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  gPSRV[2] = { NULL, NULL }; // シェーダ リソース ビュー
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> gPUAV[2] = { NULL, NULL }; // アンオーダード アクセス ビュー

	DirectX::XMFLOAT3 uvStatus = { 0,0,0 };
	DirectX::XMFLOAT3 up = { 0,0,0 };
	DirectX::XMFLOAT3 down = { 0,0,0 };
	DirectX::XMFLOAT4 materialColor = { 1,0,0,1 };
	int chainA = 0;//バッファーの切り替え
	int chainB = 1;//バッファーの切り替え

	int spriteType = 0;
	int texType = 0;
	int moveType = 0;
	int max = 0;
	float lenght = 0;
	//深度ステンシル
	int depth = 0;
};