#include "CameraController.h"
#include "Camera.h"
#include "Player.h"
#include "StageManager.h"
#include "Input.h"
#include "Mathf.h"
#include "Calculation.h"
#include <imgui.h>

// 更新処理
void CameraController::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	// カメラの回転速度
	float speed = rollSpeed * elapsedTime;
	// スティックの入力値に合わせてX軸とY軸を回転
	angle.x += ay * speed;
	angle.y += ax * speed;

	// X軸のカメラ回転を制限
	if (angle.x > maxAngle)
	{
		angle.x = maxAngle;
	}
	if (angle.x < minAngle)
	{
		angle.x = minAngle;
	}
	// Y軸の回転値を-3.14~3.14に収まるようにする
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}


	// カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	float guard = 2.4f;
	if (Player::Instance().IsGround())
	{
		guard = -Player::Instance().GetPosition().y;
	}
	float height = target.y + guard;

	// 距離定数
	float rangemax = rangeMax;
	// 敵が自分より上か判定
	if (height > NULL)
	{
		if (height > 10)
		{
			height = 10;
		}
		// 高さでX軸最小値決定
		rangemax += height;
	}
	else
	{
		//angle.x += elapsedTime;
		minAngle += DirectX::XMConvertToRadians(height * 4);
		maxAngle = minAngle;
	}

	// 回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);
	range = Mathf::Lerp(range, rangemax, 0.03f);
	// 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - (front.x * range);
	eye.y = target.y - (front.y * range);
	eye.z = target.z - (front.z * range);

	// 0に線形補完
	heightRange = Mathf::Lerp(heightRange, 0, correction);
	target.y += heightRange;
	// 0に線形補完
	eyeheight = Mathf::Lerp(eyeheight, 0, correction);
	eye.y += eyeheight;
	DirectX::XMFLOAT3 start = { target.x,target.y,target.z };
	// レイの終点位置は移動後の位置
	DirectX::XMFLOAT3 end = { eye.x,eye.y,eye.z };

	// レイキャストによる地面判定
	HitResult hit;
	//if (health > 0) {
	//if (StageManager::Instance().RayCast(start, end, hit))
	//{
	//	eye = hit.position;
	//}
	// 最小値で線形補完
	cameraEye.x = Mathf::Lerp(cameraEye.x, eye.x, correction * correctionSpeed);
	// 最小値で線形補完
	cameraEye.y = Mathf::Lerp(cameraEye.y, eye.y, correction * correctionSpeed);
	// 最小値で線形補完
	cameraEye.z = Mathf::Lerp(cameraEye.z, eye.z, correction * correctionSpeed);

	// 画面振動更新処理
	ShakeUpdate(cameraEye, elapsedTime);

	//カメラに視点を注視点を設定
	Camera::Instance().SetLookAt(cameraEye, target, DirectX::XMFLOAT3(0, 1, 0));
}
// ロックオン更新処理
void CameraController::LockOnUpdate(float elapsedTime, DirectX::XMFLOAT3 front, float Length)
{
	// 距離限界値判定
	if (Length > lengthMax) 
	{
		Length = lengthMax;
	}
	if (Length < lengthMin) 
	{
		Length = lengthMax;
	}

	// カメラの回転速度
	float speed = rollSpeed * elapsedTime;

	float length = sqrtf((front.x * front.x) + (front.z * front.z));
	if (length == 0.0f) return;

	// 進行ベクトルを単位ベクトル化
	front.x /= length;
	front.z /= length;

	// 自身の回転値から前方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// 回転角を求めるため、２つの単位ベクトルの内積を計算する
	float dot = (frontX * front.x) + (frontZ * front.z);

	// 内積値は-1.0~1.0で表現されており、２つの単位ベクトルの角度が
	// 小さいほど1.0に近づくという性質を利用して回転速度を調整する
	float rot = 1.0f - dot;
	if (speed > rot)speed = rot;

	// 左右判定を行うために2つの単位ベクトルの外積を計算する
	float cross = (frontZ * front.x) - (frontX * front.z);

	// 2Dの外積値が正の場合か負の場合によって左右判定が行える
	// 左右判定を行うことによって左右回転を選択する
	if (cross < 0.0f)
	{
		angle.y -= speed;
	}
	else
	{
		angle.y += speed;
	}

	// 高さ判定
	float height = target.y - target2.y;
	// 高さでX軸最大値決定
	float maxAngle = maxAngleX + DirectX::XMConvertToRadians(lengthMax - Length);
	float minAngle = minAngleX;
	// 距離定数
	float rangemax = rangeMax;
	// 敵が自分より上か判定
	if (height < NULL) 
	{
		// 高さでX軸最小値決定
		minAngle += DirectX::XMConvertToRadians(height);
		rangemax -= height;
	}
	else
	{
		minAngle += DirectX::XMConvertToRadians(height * 4);
		maxAngle = minAngle;
	}
	// 最小値で線形補完
	angle.x = Mathf::Lerp(angle.x, minAngle, correction);
	// 距離を最大値で線形補完
	range = Mathf::Lerp(range, rangemax, correction);
	// X軸のカメラ回転を制限
	if (angle.x > maxAngle)
	{
		angle.x = maxAngle;
	}
	//if (angle.x < minAngle)
	//{
	//	angle.x = minAngle;
	//}

	// Y軸の回転値を-3.14~3.14に収まるようにする
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	// カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// 回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front2;
	DirectX::XMStoreFloat3(&front2, Front);
	// 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - (front2.x * range);
	eye.y = target.y - (front2.y * range);
	eye.z = target.z - (front2.z * range);

	DirectX::XMFLOAT3 dir = Vector3::Subset(target2, eye);
	dir = Vector3::Normalize(dir);
	// 敵とカメラの半分の位置を高さ取得
	dir.y = (dir.y * range) * 0.5f;
	// 半分の高さを線形補完
	heightRange = Mathf::Lerp(heightRange, dir.y, correction);
	// 注視点の高さに代入
	target.y += heightRange;
	// カメラの高さを半分の高さと距離で判定
	float eyehightMax = heightRange + (Length * lengthCorrection);

	if (height < NULL) 
	{
		if (Length > 1) 
		{
			// 敵との高低差と距離で補完値を決定
			eyehightMax = heightRange + (Length * lengthCorrection) + (height / (Length * lengthCorrection));
		}
	}
	// 線形補完でカメラの上がり幅を決定
	eyeheight = Mathf::Lerp(eyeheight, eyehightMax, correction);
	// 高さ代入
	eye.y += eyeheight;
	// カメラが下に落ちすぎないよう補正
	DirectX::XMFLOAT3 start = { eye.x,eye.y + 100,eye.z };
	// レイの終点位置は移動後の位置
	DirectX::XMFLOAT3 end = { eye.x,eye.y - 1,eye.z };

	// レイキャストによる地面判定
	//HitResult hit;
	// //if (health > 0) {
	// if (StageManager::Instance().RayCast(start, end, hit))
	// {
	// 	eye.y = hit.position.y + 1;
	// }
	//if (eye.y < 0)
	// {
	// 	eye.y = 1;
	// }
	// 最小値で線形補完
	cameraEye.x = Mathf::Lerp(cameraEye.x, eye.x, correction * 20);
	// 最小値で線形補完
	cameraEye.y = Mathf::Lerp(cameraEye.y, eye.y, correction * 5);
	// 最小値で線形補完
	cameraEye.z = Mathf::Lerp(cameraEye.z, eye.z, correction * 20);
	
	// 画面振動更新処理
	ShakeUpdate(cameraEye, elapsedTime);

	// カメラに視点を注視点を設定
	Camera::Instance().SetLookAt(cameraEye, target, DirectX::XMFLOAT3(0, 1, 0));
}
// 画面振動更新処理
void CameraController::ShakeUpdate(DirectX::XMFLOAT3& eye, float elapsedTime)
{
	// 揺れ時間減少
	shakeTimer -= elapsedTime;
	// 揺れ時間が残っていれば
	if (shakeTimer > 0.0f)
	{
		// フラグによって画面振動方向変化
		if (isShake)
		{
			eye = Vector3::PosDir(eye, hitDir, -hitPow);
		}
		else
		{
			eye = Vector3::PosDir(eye, hitDir, hitPow);
		}
		// フラグ逆転
		isShake = !isShake;
	}
}
// タイトル更新処理
void CameraController::TitleUpdate(float elapsedTime)
{
	// 自機に対して正面(-Z方向)に向く
	angle.y = DirectX::XM_PI;

	// カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// 回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);
	// 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - (front.x * range);
	eye.y = target.y - (front.y * range);
	eye.z = target.z - (front.z * range);

	// カメラに視点を注視点を設定
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}
// ゲーム開始更新処理
void CameraController::EntryUpdate(float elapsedTime)
{
	// プレイヤーがホログラム開始なら
	if (Player::Instance().GetScanTimer() <= 0.0f)
	{
		entryTimer = 0.0f;	// ゲーム開始演出線形補完時間リセット
	}

	// ゲーム開始演出線形補完時間加算
	entryTimer += elapsedTime * 0.5f;
	// ゲーム開始演出線形補完時間が一定値(1.0f)以上なら
	if (entryTimer >= 1.0f)
	{
		entryTimer = 1.0f;	// ゲーム開始演出線形補完時間固定
	}

	// プレイヤーのホログラムに合わせて足元->頭に掛けてX軸回転
	angle.x = Mathf::Lerp(-XM_1DIVPI, XM_1DIV2PI, Player::Instance().GetHologramTimer());
	// プレイヤーの周りを線形補完で回る
	angle.y = Mathf::Lerp(0.0f, -DirectX::XM_2PI, entryTimer);

	// カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// 回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);
	range = Mathf::Lerp(range, rangeMax, correction);
	// 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - (front.x * range);
	eye.y = target.y - (front.y * range);
	eye.z = target.z - (front.z * range);

	//最小値で線形補完
	cameraEye.x = Mathf::Lerp(cameraEye.x, eye.x, correction * correctionSpeed);
	//最小値で線形補完
	cameraEye.y = Mathf::Lerp(cameraEye.y, eye.y, correction * correctionSpeed);
	//最小値で線形補完
	cameraEye.z = Mathf::Lerp(cameraEye.z, eye.z, correction * correctionSpeed);

	// カメラに視点を注視点を設定
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}
// ゲーム終了更新処理
void CameraController::GameAfterUpdate(float elapsedTime)
{
	angle.x = 0.0f;
	angle.y = DirectX::XM_PI;

	// カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// 回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);
	range = Mathf::Lerp(range, rangeMax, correction);
	// 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - (front.x * range);
	eye.y = target.y - (front.y * range);
	eye.z = target.z - (front.z * range);

	// 画面振動更新処理
	ShakeUpdate(eye, elapsedTime);

	//カメラに視点を注視点を設定
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

// デバッグ情報表示
void CameraController::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("CameraController", nullptr, ImGuiWindowFlags_None))
	{
		// トランスフォーム
		if (ImGui::CollapsingHeader("camera", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputFloat3("Eye", &cameraEye.x);
			ImGui::InputFloat3("Target", &target.x);
			// 回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::InputFloat3("Angle", &angle.x);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
		}		
	}
	ImGui::End();
}
