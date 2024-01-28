#pragma once

#include <DirectXMath.h>

class CameraController
{
public:
	CameraController() {}
	~CameraController() {}

	// 更新処理
	void Update(float elapsedTime);
	// ロックオン更新処理
	void LockOnUpdate(float elapsedTime, DirectX::XMFLOAT3 front, float Length);
	// 画面振動更新処理
	void ShakeUpdate(DirectX::XMFLOAT3& eye,float elapsedTime);
	// タイトル更新処理
	void TitleUpdate(float elapsedTime);
	// ゲーム開始更新処理
	void EntryUpdate(float elapsedTime);
	// ゲーム終了更新処理
	void GameAfterUpdate(float elapsedTime);

	// デバッグ情報表示
	void DrawDebugGUI();

public:
	// ターゲット位置設定
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }
	void SetTarget2(const DirectX::XMFLOAT3& target) { this->target2 = target; }
	float GetAngleY() { return this->angle.y; }

	void SetHitPow(float pow) { hitPow = pow/100; }
	void SetHitTimer(float timer) { shakeTimer = timer / 10; }

	void SetHitDir(DirectX::XMFLOAT3 dir) { hitDir = dir; }

	// カメラ距離設定
	void SetRange(float range) { this->range = range; }
	// カメラ最大距離設定
	void SetRangeMax(float rangeMax) { this->rangeMax = rangeMax; }

	void SetCorrectionSpeed(float correctionSpeed) { this->correctionSpeed = correctionSpeed; }

private:
	DirectX::XMFLOAT3	target = {};
	DirectX::XMFLOAT3	target2 = {};
	DirectX::XMFLOAT3	angle = {};										// 角度
	DirectX::XMFLOAT3	hitDir = {};
	DirectX::XMFLOAT3   cameraEye = {};
	float				rollSpeed = DirectX::XMConvertToRadians(90);	// 回転速度
	float				correctionSpeed = 20;
	float				range = 5.0f;									// カメラ距離
	float				rangeMax = 5.0f;								// カメラ最大距離
	float				heightRange = 0.0f;
	float				eyeheight = 0.0f;
	float				maxAngleX = DirectX::XMConvertToRadians(0);		// X軸最大角度
	float				minAngleX = DirectX::XMConvertToRadians(0);		// X軸最小角度
	float				maxAngle = DirectX::XMConvertToRadians(35);
	float				minAngle = DirectX::XMConvertToRadians(-15);
	float               hitPow = 0;
	float               shakeTimer = 0;		// 揺れ時間
	float               correction = 0.03f;
	float               lengthMax = 15;
	float               lengthMin = 1;
	float               lengthCorrection = 0.1f;
	float				entryTimer = 0.0f;			// ゲーム開始演出線形補完時間
	bool                isShake = true;		// 画面振動方向フラグ
};