#include "StageObj.h"
//#include "Camera.h"

// コンストラクタ
StageObj::StageObj(DirectX::XMFLOAT3 position, float angleY, float angleZ)
{
	obj = std::make_unique<Model>(".\\resources\\Box.fbx", true, false);
	obj->ModelSerialize(".\\resources\\Box.fbx");
	obj->ModelRegister(".\\resources\\Box.fbx", "CyberStage\\Texture\\Emission.png");
	
	angle.y = DirectX::XMConvertToRadians(angleY);	// Y軸角度設定
	angle.z = DirectX::XMConvertToRadians(angleZ);	// Z軸角度設定
	this->position = position;						// 位置設定
	scale.x = scale.y = scale.z = 10.0f;			// サイズ設定
	UpdateTransform();								// 行列更新処理
	raycastTransform = transform;					// レイキャスト用行列設定
	obj->UpdateBufferData(transform);				// 描画情報更新処理
	transform = obj->GetBufferTransform();			// インスタンシング化の描画情報行列取得
	instancing = true;								// インスタンシング化
}

// 更新処理
void StageObj::Update(float elapseTime)
{
#if 0
	isRender = Collision::IntersectFanVsSphere(
		Camera::Instance().GetEye(),
		Camera::Instance().GetFront(),
		Camera::Instance().GetFovY(),
		Camera::Instance().GetFarZ(),
		position,
		radius);
#endif
}
// 描画処理
void StageObj::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{

}

// レイキャスト
bool StageObj::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayVsModel(start, end, obj.get(), hit, raycastTransform);
}

// 行列更新処理
void StageObj::UpdateTransform()
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