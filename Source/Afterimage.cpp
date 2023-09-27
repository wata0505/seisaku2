#include "Afterimage.h"
#include"EffectTexAll.h"
// �R���X�g���N�^
Afterimage::Afterimage(ObjectManager* manager)
	:Object(manager)//���N���X�̃R���X�g���N�^�Ăяo��
{
	model = std::make_unique<Model>(".\\resources\\Knight Errant\\Knight_Errant4.fbx", true);
	
	
	
	radius = 0.0f;
	lifeTimer = 0.5f;
	uvStatus.w = (int)EffectTexAll::EfTexAll::Metal;
}

// �f�X�g���N�^
Afterimage::~Afterimage()
{
	
}

//�X�V����
void Afterimage::Update(float elapsedTime)
{
	//��������
	lifeTimer -= elapsedTime;
	if (lifeTimer < 0)
	{
		//�������폜
		Destroy();
	}
	//�ړ�
	uvStatus.x += elapsedTime;//uv�A�j���[�V����
}

//�`�揈��
void Afterimage::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	shader->Draw(dc, model.get(),data,uvStatus,materialColor);
}

//����
void Afterimage::Launch(std::vector<SkinnedMeshResouurce::constants> buffer)
{
	data = buffer;

}

