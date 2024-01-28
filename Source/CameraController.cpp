#include "CameraController.h"
#include "Camera.h"
#include "Player.h"
#include "StageManager.h"
#include "Input.h"
#include "Mathf.h"
#include "Calculation.h"
#include <imgui.h>

// �X�V����
void CameraController::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	// �J�����̉�]���x
	float speed = rollSpeed * elapsedTime;
	// �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
	angle.x += ay * speed;
	angle.y += ax * speed;

	// X���̃J������]�𐧌�
	if (angle.x > maxAngle)
	{
		angle.x = maxAngle;
	}
	if (angle.x < minAngle)
	{
		angle.x = minAngle;
	}
	// Y���̉�]�l��-3.14~3.14�Ɏ��܂�悤�ɂ���
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}


	// �J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	float guard = 2.4f;
	if (Player::Instance().IsGround())
	{
		guard = -Player::Instance().GetPosition().y;
	}
	float height = target.y + guard;

	// �����萔
	float rangemax = rangeMax;
	// �G���������ォ����
	if (height > NULL)
	{
		if (height > 10)
		{
			height = 10;
		}
		// ������X���ŏ��l����
		rangemax += height;
	}
	else
	{
		//angle.x += elapsedTime;
		minAngle += DirectX::XMConvertToRadians(height * 4);
		maxAngle = minAngle;
	}

	// ��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);
	range = Mathf::Lerp(range, rangemax, 0.03f);
	// �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - (front.x * range);
	eye.y = target.y - (front.y * range);
	eye.z = target.z - (front.z * range);

	// 0�ɐ��`�⊮
	heightRange = Mathf::Lerp(heightRange, 0, correction);
	target.y += heightRange;
	// 0�ɐ��`�⊮
	eyeheight = Mathf::Lerp(eyeheight, 0, correction);
	eye.y += eyeheight;
	DirectX::XMFLOAT3 start = { target.x,target.y,target.z };
	// ���C�̏I�_�ʒu�͈ړ���̈ʒu
	DirectX::XMFLOAT3 end = { eye.x,eye.y,eye.z };

	// ���C�L���X�g�ɂ��n�ʔ���
	HitResult hit;
	//if (health > 0) {
	//if (StageManager::Instance().RayCast(start, end, hit))
	//{
	//	eye = hit.position;
	//}
	// �ŏ��l�Ő��`�⊮
	cameraEye.x = Mathf::Lerp(cameraEye.x, eye.x, correction * correctionSpeed);
	// �ŏ��l�Ő��`�⊮
	cameraEye.y = Mathf::Lerp(cameraEye.y, eye.y, correction * correctionSpeed);
	// �ŏ��l�Ő��`�⊮
	cameraEye.z = Mathf::Lerp(cameraEye.z, eye.z, correction * correctionSpeed);

	// ��ʐU���X�V����
	ShakeUpdate(cameraEye, elapsedTime);

	//�J�����Ɏ��_�𒍎��_��ݒ�
	Camera::Instance().SetLookAt(cameraEye, target, DirectX::XMFLOAT3(0, 1, 0));
}
// ���b�N�I���X�V����
void CameraController::LockOnUpdate(float elapsedTime, DirectX::XMFLOAT3 front, float Length)
{
	// �������E�l����
	if (Length > lengthMax) 
	{
		Length = lengthMax;
	}
	if (Length < lengthMin) 
	{
		Length = lengthMax;
	}

	// �J�����̉�]���x
	float speed = rollSpeed * elapsedTime;

	float length = sqrtf((front.x * front.x) + (front.z * front.z));
	if (length == 0.0f) return;

	// �i�s�x�N�g����P�ʃx�N�g����
	front.x /= length;
	front.z /= length;

	// ���g�̉�]�l����O���������߂�
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// ��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = (frontX * front.x) + (frontZ * front.z);

	// ���ϒl��-1.0~1.0�ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
	// �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;
	if (speed > rot)speed = rot;

	// ���E������s�����߂�2�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = (frontZ * front.x) - (frontX * front.z);

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
	// ���E������s�����Ƃɂ���č��E��]��I������
	if (cross < 0.0f)
	{
		angle.y -= speed;
	}
	else
	{
		angle.y += speed;
	}

	// ��������
	float height = target.y - target2.y;
	// ������X���ő�l����
	float maxAngle = maxAngleX + DirectX::XMConvertToRadians(lengthMax - Length);
	float minAngle = minAngleX;
	// �����萔
	float rangemax = rangeMax;
	// �G���������ォ����
	if (height < NULL) 
	{
		// ������X���ŏ��l����
		minAngle += DirectX::XMConvertToRadians(height);
		rangemax -= height;
	}
	else
	{
		minAngle += DirectX::XMConvertToRadians(height * 4);
		maxAngle = minAngle;
	}
	// �ŏ��l�Ő��`�⊮
	angle.x = Mathf::Lerp(angle.x, minAngle, correction);
	// �������ő�l�Ő��`�⊮
	range = Mathf::Lerp(range, rangemax, correction);
	// X���̃J������]�𐧌�
	if (angle.x > maxAngle)
	{
		angle.x = maxAngle;
	}
	//if (angle.x < minAngle)
	//{
	//	angle.x = minAngle;
	//}

	// Y���̉�]�l��-3.14~3.14�Ɏ��܂�悤�ɂ���
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	// �J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// ��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front2;
	DirectX::XMStoreFloat3(&front2, Front);
	// �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - (front2.x * range);
	eye.y = target.y - (front2.y * range);
	eye.z = target.z - (front2.z * range);

	DirectX::XMFLOAT3 dir = Vector3::Subset(target2, eye);
	dir = Vector3::Normalize(dir);
	// �G�ƃJ�����̔����̈ʒu�������擾
	dir.y = (dir.y * range) * 0.5f;
	// �����̍�������`�⊮
	heightRange = Mathf::Lerp(heightRange, dir.y, correction);
	// �����_�̍����ɑ��
	target.y += heightRange;
	// �J�����̍����𔼕��̍����Ƌ����Ŕ���
	float eyehightMax = heightRange + (Length * lengthCorrection);

	if (height < NULL) 
	{
		if (Length > 1) 
		{
			// �G�Ƃ̍��፷�Ƌ����ŕ⊮�l������
			eyehightMax = heightRange + (Length * lengthCorrection) + (height / (Length * lengthCorrection));
		}
	}
	// ���`�⊮�ŃJ�����̏オ�蕝������
	eyeheight = Mathf::Lerp(eyeheight, eyehightMax, correction);
	// �������
	eye.y += eyeheight;
	// �J���������ɗ��������Ȃ��悤�␳
	DirectX::XMFLOAT3 start = { eye.x,eye.y + 100,eye.z };
	// ���C�̏I�_�ʒu�͈ړ���̈ʒu
	DirectX::XMFLOAT3 end = { eye.x,eye.y - 1,eye.z };

	// ���C�L���X�g�ɂ��n�ʔ���
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
	// �ŏ��l�Ő��`�⊮
	cameraEye.x = Mathf::Lerp(cameraEye.x, eye.x, correction * 20);
	// �ŏ��l�Ő��`�⊮
	cameraEye.y = Mathf::Lerp(cameraEye.y, eye.y, correction * 5);
	// �ŏ��l�Ő��`�⊮
	cameraEye.z = Mathf::Lerp(cameraEye.z, eye.z, correction * 20);
	
	// ��ʐU���X�V����
	ShakeUpdate(cameraEye, elapsedTime);

	// �J�����Ɏ��_�𒍎��_��ݒ�
	Camera::Instance().SetLookAt(cameraEye, target, DirectX::XMFLOAT3(0, 1, 0));
}
// ��ʐU���X�V����
void CameraController::ShakeUpdate(DirectX::XMFLOAT3& eye, float elapsedTime)
{
	// �h�ꎞ�Ԍ���
	shakeTimer -= elapsedTime;
	// �h�ꎞ�Ԃ��c���Ă����
	if (shakeTimer > 0.0f)
	{
		// �t���O�ɂ���ĉ�ʐU�������ω�
		if (isShake)
		{
			eye = Vector3::PosDir(eye, hitDir, -hitPow);
		}
		else
		{
			eye = Vector3::PosDir(eye, hitDir, hitPow);
		}
		// �t���O�t�]
		isShake = !isShake;
	}
}
// �^�C�g���X�V����
void CameraController::TitleUpdate(float elapsedTime)
{
	// ���@�ɑ΂��Đ���(-Z����)�Ɍ���
	angle.y = DirectX::XM_PI;

	// �J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// ��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);
	// �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - (front.x * range);
	eye.y = target.y - (front.y * range);
	eye.z = target.z - (front.z * range);

	// �J�����Ɏ��_�𒍎��_��ݒ�
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}
// �Q�[���J�n�X�V����
void CameraController::EntryUpdate(float elapsedTime)
{
	// �v���C���[���z���O�����J�n�Ȃ�
	if (Player::Instance().GetScanTimer() <= 0.0f)
	{
		entryTimer = 0.0f;	// �Q�[���J�n���o���`�⊮���ԃ��Z�b�g
	}

	// �Q�[���J�n���o���`�⊮���ԉ��Z
	entryTimer += elapsedTime * 0.5f;
	// �Q�[���J�n���o���`�⊮���Ԃ����l(1.0f)�ȏ�Ȃ�
	if (entryTimer >= 1.0f)
	{
		entryTimer = 1.0f;	// �Q�[���J�n���o���`�⊮���ԌŒ�
	}

	// �v���C���[�̃z���O�����ɍ��킹�đ���->���Ɋ|����X����]
	angle.x = Mathf::Lerp(-XM_1DIVPI, XM_1DIV2PI, Player::Instance().GetHologramTimer());
	// �v���C���[�̎������`�⊮�ŉ��
	angle.y = Mathf::Lerp(0.0f, -DirectX::XM_2PI, entryTimer);

	// �J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// ��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);
	range = Mathf::Lerp(range, rangeMax, correction);
	// �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - (front.x * range);
	eye.y = target.y - (front.y * range);
	eye.z = target.z - (front.z * range);

	//�ŏ��l�Ő��`�⊮
	cameraEye.x = Mathf::Lerp(cameraEye.x, eye.x, correction * correctionSpeed);
	//�ŏ��l�Ő��`�⊮
	cameraEye.y = Mathf::Lerp(cameraEye.y, eye.y, correction * correctionSpeed);
	//�ŏ��l�Ő��`�⊮
	cameraEye.z = Mathf::Lerp(cameraEye.z, eye.z, correction * correctionSpeed);

	// �J�����Ɏ��_�𒍎��_��ݒ�
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}
// �Q�[���I���X�V����
void CameraController::GameAfterUpdate(float elapsedTime)
{
	angle.x = 0.0f;
	angle.y = DirectX::XM_PI;

	// �J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// ��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);
	range = Mathf::Lerp(range, rangeMax, correction);
	// �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - (front.x * range);
	eye.y = target.y - (front.y * range);
	eye.z = target.z - (front.z * range);

	// ��ʐU���X�V����
	ShakeUpdate(eye, elapsedTime);

	//�J�����Ɏ��_�𒍎��_��ݒ�
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

// �f�o�b�O���\��
void CameraController::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("CameraController", nullptr, ImGuiWindowFlags_None))
	{
		// �g�����X�t�H�[��
		if (ImGui::CollapsingHeader("camera", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::InputFloat3("Eye", &cameraEye.x);
			ImGui::InputFloat3("Target", &target.x);
			// ��]
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
