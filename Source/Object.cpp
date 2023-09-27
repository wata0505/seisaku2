#include "Object.h"
#include"ObjectManager.h"
#include"Graphics.h"

// コンストラクタ
Object::Object(ObjectManager* manager)
	:manager(manager)
{
	// 生成時にマネージャーに登録する
	manager->Register(this);
}

// デバッグプリミティブ描画
void Object::DrawDebugPrimitive()
{
	//DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	//
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

// 破棄
void Object::Destroy()
{
	//if (type == 5)boon->Play(false);
	// マネージャーから自分を解除する
	manager->Remove(this);

}

// 行列更新処理
void Object::UpdateTransform()
{
	DirectX::XMVECTOR Front, Up, Right;

	//前ベクトルを算出
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);

	//仮の上ベクトルを算出
	Up = DirectX::XMLoadFloat3(&directionUp);
	Up = DirectX::XMVector3Normalize(Up);

	//右ベクトルを算出
	Right = DirectX::XMVector3Cross(Up, Front);

	//上ベクトルを算出
	Up = DirectX::XMVector3Cross(Front, Right);

	//計算結果を取り出し
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	//算出した軸ベクトルから行列を作成
	transform._11 = right.x * 0.01*scale.x;
	transform._12 = right.y * 0.01*scale.x;
	transform._13 = right.z * 0.01*scale.x;
	transform._14 = 0.0f;
	transform._21 = up.x * 0.01 * scale.y;
	transform._22 = up.y * 0.01 * scale.y;
	transform._23 = up.z * 0.01 * scale.y;
	transform._24 = 0.0f;
	transform._31 = front.x * 0.01 * scale.z;
	transform._32 = front.y * 0.01 * scale.z;
	transform._33 = front.z * 0.01 * scale.z;
	transform._34 = 0.0f;
	transform._41 = position.x;
	transform._42 = position.y;
	transform._43 = position.z;
	transform._44 = 1.0f;

	//発射方向
	this->direction = front;
}
