#pragma once

#pragma once

#include <memory>
#include <string>
#include <map>
#include "SkinnedMesh.h"

// リソースマネージャー
class ResourceManager
{
private:
	ResourceManager() {}
	~ResourceManager() {}

public:
	// 唯一のインスタンス取得
	static ResourceManager& Instance()
	{
		static ResourceManager Instance;
		return Instance;
	}

	// モデルリソースの読み込み
	std::shared_ptr<SkinnedMeshResouurce>LoadModelResource(const char* filename, bool leftflag,bool triangulate);
	//モデル登録
	void SkinnedMeshRegister(const char* filename, std::shared_ptr<SkinnedMeshResouurce> model, const char* texfilename);
private:
	using ModelMap = std::map<std::string, std::weak_ptr<SkinnedMeshResouurce>>;

	ModelMap models;
};