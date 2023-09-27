#include"Calculation.h"
#include"Camera.h"
#include"Graphics.h"
DirectX::XMFLOAT3 Vector3::Normalize(DirectX::XMFLOAT3 v1)
{
	if (v1.x == 0 && v1.y == 0 && v1.z == 0)return v1;
	//DirectX::XMVector3 N = DirectX::XMLoadFloat3(&v1);
	float len = sqrtf(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
	v1.x /=len;
	v1.y /= len;
	v1.z /= len;
	//DirectX::XMVector33Normalize(N);
	//DirectX::XMStoreFloat3(&v1, N);
	return v1;
}

float Vector3::Dot(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2) {

	return(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}
float Vector3::Lenght(DirectX::XMFLOAT3 v1) {

	return sqrtf(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
}
DirectX::XMFLOAT3 Vector3::Subset(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2) {

	return{ v1.x - v2.x, v1.y - v2.y,v1.z - v2.z };
}

DirectX::XMFLOAT3 Vector3::Cross(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2) {

	return{ v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,v1.x * v2.y - v1.y * v2.x };
}

DirectX::XMFLOAT3 Vector3::Multip(DirectX::XMFLOAT3 v1, float v2) {

	return{ v1.x * v2, v1.y * v2, v1.z * v2 };
}
DirectX::XMFLOAT3 Vector3::Add(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2) {

	return{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}
DirectX::XMFLOAT3 Vector3::PosDir(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2, float Lengle) {
	v2 = Multip(v2, Lengle);
	v1 = Add(v1, v2);	
	return v1;
}
DirectX::XMFLOAT2 Vector3::C3Dor2D(DirectX::XMFLOAT3 pos) {
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	Camera& camera = Camera::Instance();
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	dc->RSGetViewports(&numViewports, &viewport);
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&camera.GetView());
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&camera.GetProjection());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
	
	DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&pos);

	//Player& player = Player::Instance();
	//DirectX::XMFLOAT3 eye = PosDir(player.GetPosition(), player.GetAttackDir(), -10);
	//DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	//
	//DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&player.GetPosition());
	//DirectX::XMVECTOR Up = { 0,1,0 };
	//DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	////ビューを逆行列化し、ワールド行列に戻す
	//ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	//dc->RSGetViewports(&numViewports, &viewport);
	//DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&camera.GetProjection());
	//DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
	//
	//
	//DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&pos);

	// ワールド座標からスクリーン座標へ
	DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
		WorldPosition,
		viewport.TopLeftX,
		viewport.TopLeftY,
		viewport.Width,
		viewport.Height,
		viewport.MinDepth,
		viewport.MaxDepth,
		Projection,
		View,
		World
	);

	//スクリーン座標
	DirectX::XMFLOAT2 screenPosition;
	DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);
	return screenPosition;
}
bool Vector3::Probability(int num) {
	if (rand() % num == 0) {
		return true;
	}
	return false;
}

bool Vector3::FrameInput(int timer,int frameIndex) {
	if (timer % frameIndex == 0) {
		return true;
	}
	return false;
}

DirectX::XMFLOAT2 Vector2::Normalize(DirectX::XMFLOAT2 v1)
{
	if (v1.x == 0 && v1.y == 0)return v1;
	//DirectX::XMVector2 N = DirectX::XMLoadFloat3(&v1);
	float len = sqrtf(v1.x * v1.x + v1.y * v1.y);
	v1.x /= len;
	v1.y /= len;
	//DirectX::XMVector23Normalize(N);
	//DirectX::XMStoreFloat3(&v1, N);
	return v1;
}

float Vector2::Dot(DirectX::XMFLOAT2 v1, DirectX::XMFLOAT2 v2) {

	return(v1.x * v2.x + v1.y * v2.y );
}
DirectX::XMFLOAT2 Vector2::Subset(DirectX::XMFLOAT2 v1, DirectX::XMFLOAT2 v2) {

	return{ v1.x - v2.x, v1.y - v2.y};
}


DirectX::XMFLOAT2 Vector2::Multip(DirectX::XMFLOAT2 v1, float v2) {

	return{ v1.x * v2, v1.y * v2};
}
DirectX::XMFLOAT2 Vector2::Add(DirectX::XMFLOAT2 v1, DirectX::XMFLOAT2 v2) {

	return{ v1.x + v2.x, v1.y + v2.y};
}
