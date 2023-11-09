#pragma once
#include<DirectXMath.h>
#include "shader.h"
#include "Model.h"
#include "Trap.h"
class Mine :public Trap
{
public:
	Mine();
	~Mine();
public:
	//更新処理
	void Update(float elapsed_time)override;
	// 描画処理
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive()override;

	void CollisionVsEnemies();

private:

	std::unique_ptr<Model> model = nullptr;
	//描画情報格納
	std::vector<SkinnedMeshResouurce::constants> renderdata;

};