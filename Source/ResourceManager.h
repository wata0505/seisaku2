#pragma once

#pragma once

#include <memory>
#include <string>
#include <map>
#include "SkinnedMesh.h"

// ���\�[�X�}�l�[�W���[
class ResourceManager
{
private:
	ResourceManager() {}
	~ResourceManager() {}

public:
	// �B��̃C���X�^���X�擾
	static ResourceManager& Instance()
	{
		static ResourceManager Instance;
		return Instance;
	}

	// ���f�����\�[�X�̓ǂݍ���
	std::shared_ptr<SkinnedMeshResouurce>LoadModelResource(const char* filename, bool leftflag,bool triangulate);
	//���f���o�^
	void SkinnedMeshRegister(const char* filename, std::shared_ptr<SkinnedMeshResouurce> model, const char* texfilename);
private:
	using ModelMap = std::map<std::string, std::weak_ptr<SkinnedMeshResouurce>>;

	ModelMap models;
};