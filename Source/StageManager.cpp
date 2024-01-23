#include "StageManager.h"

// コンストラクタ
StageManager::StageManager() 
{
	obj = std::make_unique<Model>(".\\resources\\Box.fbx", true, false);
	obj->ModelSerialize(".\\resources\\Box.fbx");
	obj->ModelRegister(".\\resources\\Box.fbx", "CyberStage\\Texture\\Emission.png");
}

// 更新処理
void StageManager::Update(float elapsedTime)
{
	for (Stage* stage : stages)
	{
		stage->Update(elapsedTime);
	}
}
// 描画処理
void StageManager::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
	for (Stage* stage : stages)
	{
		stage->Render(deviceContext, shader);
	}
}
// インスタンシング描画処理
void StageManager::InstaningRender(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
	shader->Draw(deviceContext, obj.get(), constants, instangNo);
}

// レイキャスト
bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	bool result = false;
	hit.distance = FLT_MAX;

	for (Stage* stage : stages)
	{
		HitResult hit1;
		if (stage->RayCast(start, end, hit1))
		{
			if (hit.distance > hit1.distance)
			{
				hit = hit1;
				result = true;
			}
		}
	}

	return result;
}

// ステージ登録
void StageManager::Register(Stage* stage)
{
	// インスタンシングモデルなら
	if (stage->GetInstancing()) 
	{
		constants.world[instangNo] = stage->GetTransform();
		instangNo++;
	}
	stages.emplace_back(stage);
}
// ステージ全削除
void StageManager::Clear()
{
	for (Stage* stage : stages)
	{
		delete stage;
		stage = nullptr;
	}
	stages.clear();
	instangNo = 0;
}
