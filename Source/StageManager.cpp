#include "StageManager.h"

StageManager::StageManager() {
	obj = std::make_unique<Model>(".\\resources\\cube4.fbx", true, false);
	obj->ModelSerialize(".\\resources\\cube4.fbx");
	//model->ModelCreate(".\\resources\\ExampleStage\\ExampleStage.fbx");
	obj->ModelRegister(".\\resources\\cube4.fbx", "Stag\\CyberStage5.fbm\\Emission.png");
}

void StageManager::Update(float elapsedTime)
{
	for (Stage* stage : stages)
	{
		stage->Update(elapsedTime);
	}

}

void StageManager::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	for (Stage* stage : stages)
	{
		stage->Render(dc, shader);
	}
}
void StageManager::InstaningRender(ID3D11DeviceContext* dc, ModelShader* shader)
{
	shader->Draw(dc,obj.get(),constants, instangNo);
}

void StageManager::Register(Stage* stage)
{
	if (stage->GetInstsncing()) {
		
		constants.world[instangNo] = stage->GetTransform();
		instangNo++;
	}
	stages.emplace_back(stage);
}

void StageManager::clear()
{
	for (Stage* stage : stages)
	{
		delete stage;
		stage = nullptr;
	}
	stages.clear();
	instangNo = 0;
}

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

bool StageManager::PillarVS(const DirectX::XMFLOAT3& pos, const float radius, const float height, DirectX::XMFLOAT3& outPos)
{
	bool result = false;

	for (Stage* stage : stages)
	{
		DirectX::XMFLOAT3 outPos1;
		if (stage->PillarVS(pos, radius, height,outPos1))
		{
				outPos= outPos1;
				result = true;
			
		}
	}

	return result;
}