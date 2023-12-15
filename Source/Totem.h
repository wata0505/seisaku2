#pragma once
#include<DirectXMath.h>
#include "shader.h"
#include "Model.h"
#include "Trap.h"
class Totem :public Trap
{
public:
	Totem();
	~Totem();
public:
	//更新処理
	void Update(float elapsed_time)override;
	// 描画処理
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;
	//残像エフェクト描画
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)override;
	// デバッグプリミティブ描画
	void DrawDebugPrimitive()override;
	// デバッグ情報表示
	void DrawDebugGUI() override;

	void CollisionVsEnemies();

private:

	std::unique_ptr<Model> model = nullptr;
	//描画情報格納
	std::vector<SkinnedMeshResouurce::constants> renderdata;


};