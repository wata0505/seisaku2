#include "Afterimage.h"
#include"EffectTexAll.h"
// コンストラクタ
Afterimage::Afterimage(ObjectManager* manager)
	:Object(manager)//基底クラスのコンストラクタ呼び出し
{
	model = std::make_unique<Model>(".\\resources\\Knight Errant\\Knight_Errant4.fbx", true);
	
	
	
	radius = 0.0f;
	lifeTimer = 0.5f;
	uvStatus.w = (int)EffectTexAll::EfTexAll::Metal;
}

// デストラクタ
Afterimage::~Afterimage()
{
	
}

//更新処理
void Afterimage::Update(float elapsedTime)
{
	//寿命処理
	lifeTimer -= elapsedTime;
	if (lifeTimer < 0)
	{
		//自分を削除
		Destroy();
	}
	//移動
	uvStatus.x += elapsedTime;//uvアニメーション
}

//描画処理
void Afterimage::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	shader->Draw(dc, model.get(),data,uvStatus,materialColor);
}

//発射
void Afterimage::Launch(std::vector<SkinnedMeshResouurce::constants> buffer)
{
	data = buffer;

}

