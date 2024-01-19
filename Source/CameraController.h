#pragma once
#include<DirectXMath.h>

//カメラコントローラー
class CameraController
{
public:
	CameraController() {}
	~CameraController() {}

	//更新処理
	void Update(float elapsedTime);

	void TitleUpdate(float elapsedTime);
	void EntryUpdate(float elapsedTime);
	void GameAfterUpdate(float elapsedTime);

	void Update2(float elapsedTime, DirectX::XMFLOAT3 front,float Length);

	void ShakeUpdate(DirectX::XMFLOAT3& eye,float elapsedTime);

	void DrawDebugGUI();

	//ターゲット位置設定
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }
	void SetTarget2(const DirectX::XMFLOAT3& target) { this->target2 = target; }
	float GetAngleY() { return this->angle.y; }

	void SetHitPow(float pow) { hitPow = pow/100; }
	void SetHitTimer(float timer) { hitTimer = timer/10; }

	void SetHitDir(DirectX::XMFLOAT3 dir) { hitDir = dir; }
	//カメラ距離設定
	void SetRange(float range) { this->range = range; }
	//カメラ最大距離設定
	void SetRangeMax(float rangeMax) { this->rangeMax = rangeMax; }

	void SetCorrectionSpeed(float correctionSpeed) { this->correctionSpeed = correctionSpeed; }

private:
	DirectX::XMFLOAT3	target = { 0,0,0 };
	DirectX::XMFLOAT3	target2 = { 0,0,0 };
	DirectX::XMFLOAT3	angle = { 0,0,0 };
	DirectX::XMFLOAT3	hitDir = { 0,0,0 };
	DirectX::XMFLOAT3   cameraEye = {0,0,0};
	float				rollSpeed = DirectX::XMConvertToRadians(90);
	float				correctionSpeed = 20;
	float				rangeMax = 5.0f;
	float				range = 5.0f;
	float				heightRange = 0.0f;
	float				eyeheight = 0.0f;
	float				maxAngleX = DirectX::XMConvertToRadians(0);
	float				minAngleX = DirectX::XMConvertToRadians(0);
	float				maxAngle = DirectX::XMConvertToRadians(35);
	float				minAngle = DirectX::XMConvertToRadians(-15);
	float               hitPow = 0;
	float               hitTimer = 0;
	bool                shakeflag = true;
	float               correction = 0.03f;
	float               lengthMax = 15;
	float               lengthMin = 1;
	float               lengthCorrection = 0.1f;
	float				entryTimer = 0.0f;
};