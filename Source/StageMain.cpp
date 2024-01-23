#include "StageMain.h"

// コンストラクタ
StageMain::StageMain()
{
	model = std::make_unique<Model>(".\\resources\\CyberStage\\CyberStage.fbx", true, false);
	model->ModelSerialize(".\\resources\\CyberStage\\CyberStage.fbx");
	model->ModelRegister(".\\resources\\CyberStage\\CyberStage.fbx", "Texture\\HoneycombStructure.png");

	position.y = -2.5f;						// Y軸位置設定
	scale.x = scale.y = scale.z = 100.0f;	// サイズ設定
	UpdateTransform();						// 行列更新処理
    model->UpdateBufferData(transform);		// 描画情報更新処理
}

// 更新処理
void StageMain::Update(float elapseTime)
{
	// 経過時間加算
	progressTimer += elapseTime * 4.0f;
	// シェーダー情報調整
	model->ShaderAdjustment(0.0f, progressTimer, 0.0f);
}
// 描画処理
void StageMain::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
	shader->Draw(deviceContext, model.get());
}

// レイキャスト
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, model.get(), hit, transform);
}

// 行列更新処理
void StageMain::UpdateTransform()
{
	const DirectX::XMFLOAT4X4 coordinate_system_transforms[]
	{
		{ -1,0,0,0, 0,1, 0,0, 0,0,1,0, 0,0,0,1 },		//0:RHS Y-UP　右手系
		{  1,0,0,0, 0,1, 0,0, 0,0,1,0, 0,0,0,1 },		//1:LHS Y-UP　左手系
		{ -1,0,0,0, 0,0,-1,0, 0,1,0,0, 0,0,0,1 },		//2:RHS Z-UP　右手系
		{  1,0,0,0, 0,0, 1,0, 0,1,0,0, 0,0,0,1 },		//3:LHS Z-UP　左手系
	};
	
	const float scaleFactor = 0.01f;
	
	DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) };
	// スケール行列作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// 回転行列作成
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;
	// 位置行列作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// 3つの行列を組み合わせ、ワールド行列作成
	DirectX::XMMATRIX W = C * S * R * T;
	// 計算したワールド行列取得
	DirectX::XMStoreFloat4x4(&transform, W);
}