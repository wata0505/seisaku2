#pragma once

#include<DirectXMath.h>
#include"Model.h"

//ヒット結果
struct HitResult
{
	DirectX::XMFLOAT3 position = { 0,0,0 };// レイとポリゴンの交点
	DirectX::XMFLOAT3 normal = { 0,0,0 };  // 衝突したポリゴンの法線ベクトル
	float			  distance = 0.0f;	   // レイの始点から交点までの距離
	int				  materialIndex = -1;  // 衝突したポリゴンのマテリアル番号
	DirectX::XMFLOAT3	rotation = { 0,0,0 };	// 回転量

};

//コリジョン
class Collision
{
public:
	//球と球の交差判定
	static bool IntersectSphereVsSphere(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		DirectX::XMFLOAT3& outPositionB
	);

	//円柱と円柱の交差判定
	static bool IntersectCylinderVsCylinder(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		float heightA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		float heightB,
		DirectX::XMFLOAT3& outPositionB
	);

	// 球と円柱の交差判定
	static bool IntersectSphereVsCylinder(
		const DirectX::XMFLOAT3& spherePosition,
		float sphereRadius,
		const DirectX::XMFLOAT3& cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3& outCylinderPosition
	);
	// カプセルと球の交差判定
	static bool IntersectCapsuleVsSphere(
		const DirectX::XMFLOAT3& capPosition,
		const DirectX::XMFLOAT3& capEndPosition,
		float capRadius,
		const DirectX::XMFLOAT3& spherePosition,
		float sphereRadius
	);
	// 扇形の範囲の交差判定
	static bool IntersectFanVsSphere(
		const DirectX::XMFLOAT3& fanPosition,
		const DirectX::XMFLOAT3& fanDir,
		float rangeDegree,
		float fanLength,
		const DirectX::XMFLOAT3& objPosition,
		float objRadius
	);
	static bool IntersectRayVsCylinder(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const DirectX::XMFLOAT3& cylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3& outCylinderPosition
	);
	// レイとモデルの交差判定
	static bool IntersectRayVsModel(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const Model* model,
		HitResult& result,
		const DirectX::XMFLOAT4X4& world
	);

	// ブロックと円柱の交差判定
	static bool IntersectCubeVsCylinder(
		const DirectX::XMFLOAT3& CubePosition,
		float CubeHalf,
		const DirectX::XMFLOAT3& CylinderPosition,
		float cylinderRadius,
		float cylinderHeight,
		DirectX::XMFLOAT3& outCylinderPosition
	);
};