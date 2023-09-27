#include "Graphics.h"
#include "ResourceManager.h"


// モデルリソースの読み込み
std::shared_ptr<SkinnedMeshResouurce> ResourceManager::LoadModelResource(const char* filename, bool leftflag,bool triangulate)
{
	// モデルを検索
	ModelMap::iterator it = models.find(filename);
	if (it != models.end())
	{
		// マネージャー内に指定ファイルパスのモデルが存在するかを検索し、見つかったらそのモデルを返す
		if (!it->second.expired())
		{
			return it->second.lock();
		}
	}
	// 新規モデルリソース作成＆読み込み
	ID3D11Device* device = Graphics::Instance().GetDevice();
	auto model = std::make_shared<SkinnedMeshResouurce>();
	model->Load(device, filename,0, leftflag,triangulate);

	// マップに登録
	//models[filename] = model;

	return model;
}
void ResourceManager::SkinnedMeshRegister(const char* filename, std::shared_ptr<SkinnedMeshResouurce> model, const char* texfilename) {
	ModelMap::iterator it = models.find(filename);
	if (it != models.end())
	{
		// マネージャー内に指定ファイルパスのモデルが存在するかを検索し、見つかったらそのモデルを返す
		if (!it->second.expired())
		{
			return;
		}
	}
	models[filename] = model;
	ID3D11Device* device = Graphics::Instance().GetDevice();
	model->CreateComObjects(device, filename,texfilename);
}