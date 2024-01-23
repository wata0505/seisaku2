#include "StageManager.h"

// �R���X�g���N�^
StageManager::StageManager() 
{
	obj = std::make_unique<Model>(".\\resources\\Box.fbx", true, false);
	obj->ModelSerialize(".\\resources\\Box.fbx");
	obj->ModelRegister(".\\resources\\Box.fbx", "CyberStage\\Texture\\Emission.png");
}

// �X�V����
void StageManager::Update(float elapsedTime)
{
	for (Stage* stage : stages)
	{
		stage->Update(elapsedTime);
	}
}
// �`�揈��
void StageManager::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
	for (Stage* stage : stages)
	{
		stage->Render(deviceContext, shader);
	}
}
// �C���X�^���V���O�`�揈��
void StageManager::InstaningRender(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
	shader->Draw(deviceContext, obj.get(), constants, instangNo);
}

// ���C�L���X�g
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

// �X�e�[�W�o�^
void StageManager::Register(Stage* stage)
{
	// �C���X�^���V���O���f���Ȃ�
	if (stage->GetInstancing()) 
	{
		constants.world[instangNo] = stage->GetTransform();
		instangNo++;
	}
	stages.emplace_back(stage);
}
// �X�e�[�W�S�폜
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
