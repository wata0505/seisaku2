#include "Graphics.h"
#include "ResourceManager.h"


// ���f�����\�[�X�̓ǂݍ���
std::shared_ptr<SkinnedMeshResouurce> ResourceManager::LoadModelResource(const char* filename, bool leftflag,bool triangulate)
{
	// ���f��������
	ModelMap::iterator it = models.find(filename);
	if (it != models.end())
	{
		// �}�l�[�W���[���Ɏw��t�@�C���p�X�̃��f�������݂��邩���������A���������炻�̃��f����Ԃ�
		if (!it->second.expired())
		{
			return it->second.lock();
		}
	}
	// �V�K���f�����\�[�X�쐬���ǂݍ���
	ID3D11Device* device = Graphics::Instance().GetDevice();
	auto model = std::make_shared<SkinnedMeshResouurce>();
	model->Load(device, filename,0, leftflag,triangulate);

	// �}�b�v�ɓo�^
	//models[filename] = model;

	return model;
}
void ResourceManager::SkinnedMeshRegister(const char* filename, std::shared_ptr<SkinnedMeshResouurce> model, const char* texfilename) {
	ModelMap::iterator it = models.find(filename);
	if (it != models.end())
	{
		// �}�l�[�W���[���Ɏw��t�@�C���p�X�̃��f�������݂��邩���������A���������炻�̃��f����Ԃ�
		if (!it->second.expired())
		{
			return;
		}
	}
	models[filename] = model;
	ID3D11Device* device = Graphics::Instance().GetDevice();
	model->CreateComObjects(device, filename,texfilename);
}