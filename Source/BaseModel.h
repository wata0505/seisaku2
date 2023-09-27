#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "SkinnedMesh.h"
#include "Shader.h"

class BaseModel
{
public:
	BaseModel(const char* filename, bool triangulate);
	virtual	~BaseModel() {}

	// デバッグ情報表示
	void DrawDebugGUI();

	// リソース取得
	const SkinnedMeshResouurce* GetResource() const { return resource.get(); }

	// 静的モデルか判定
	bool IsStaticModel() const { return staticFlag; }
	void SetStaticModelFlag(bool staticFlag) { this->staticFlag = staticFlag; }

public:
	// シェーダーへの設定用バッファ情報取得
	virtual BufferData GetBufferData(const SkinnedMeshResouurce::Mesh& mesh) const = 0;

protected:
	std::shared_ptr<SkinnedMeshResouurce>	resource;
	bool							staticFlag = false;	//	静的モデルフラグ
};
