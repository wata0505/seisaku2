#pragma once

#include "Model.h"
#include "Object.h"



//残像エフェクト
class Afterimage :public Object
{
public:
	Afterimage(ObjectManager* manager);
	~Afterimage() override;

	//更新処理
	void Update(float elapsedTime)override;

	//描画処理
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;

	void TrailRender(RenderContext& rc, SwordTrailShader* shader)override {};

	//モデル描画情報セット
	void Launch(std::vector<SkinnedMeshResouurce::constants> buffer);


private:
	std::shared_ptr<Model> model = { nullptr };
	float speed = 17.0f;
	DirectX::XMFLOAT3 angle;
	DirectX::XMFLOAT4 uvStatus;
	int ModelNo = 0;
	std::vector<SkinnedMeshResouurce::constants> data;
	DirectX::XMFLOAT4	materialColor = { 0.5,0.5,0.5,1 };
};