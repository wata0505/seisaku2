#pragma once
#include "Shader.h"
#include "GaugeUI.h"
#include "BaseUI.h"
enum  SwordState
{
	Nomal,
	Shift,
	Stop,
};
enum WeponType
{
	MainSword,
	Axe,
	Halberd,
	SubSword,
	Shield,

	WeponMax
};

class MainWepon
{
public:
	MainWepon();
	 ~MainWepon() {}

	 static MainWepon& Instance();

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
    void Render(ID3D11DeviceContext* dc, ModelShader* shader);

	// �ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// �����擾
	const DirectX::XMFLOAT3& GetDirection() const { return direction; }

	// �X�P�[���擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// ���a����
	const float GetRadius() const { return radius; }

	void SetRadius(float radius) { this->radius  = radius; }

	void SetSwordState(int type) { this->swordState = type; }
	//�^�C�v�擾
	const int GetSwordState() const { return swordState; }

	// ���G����
	const float GetInvincibleTime() const { return InvincibleTime; }

	//�_���[�W
	const int GetDamage() const { return damage; }

	void SetDamage(int damage) { this->damage = damage; }

	const int GetMpChargeRate() const { return mpChargeRate; }

	void SetMpChargeRate(int mpChargeRate) { this->mpChargeRate = mpChargeRate; }

	void SetDirection(DirectX::XMFLOAT3 d) { direction = d; };
	//����̓��������
	void SetShiftDirection(DirectX::XMFLOAT3 d) { shiftDirection = d; };

	void SetA(float d) { angle = d; };

	void SetWeponType(int type) { this->weponType = type; }
	//�^�C�v�擾
	const int GetWeponType() const { return weponType; }

	std::shared_ptr<Model> GetWeponModel(int t) { return weapon[t]; }

	// �ʒu�擾
	DirectX::XMFLOAT3 GetWeaponPoint() const { return weaponPoint; }

	// �����擾
	DirectX::XMFLOAT3 GetWeaponEPoint() const { return weaponEPoint; }
	DirectX::XMFLOAT3 GetWeaponEFPoint() const { return weaponEFPoint; }

	DirectX::XMFLOAT3 GetWeaponPPoint() const { return weaponPPoint; }

	DirectX::XMFLOAT3 GetWeaponHPoint() const { return weaponHPoint; }

	void SetRnderflag(bool flag) { renderflag = flag; }

	bool GetRnderflag() { return renderflag; }
	//�f�B�]���u���
	float GetDisslve() { return uvStatus.x; }
	//����̏���
	void Exhaustion(int life);
	//���̕��킪���Ă��邩
	bool GetBreakFlag(int type) { return breakflag[type]; }

	int GetWeponLife(int type) { return weponLife[type]; }

	float GetMaxWeponLife(int type) { return maxLife[type]; }

	void SetWeponLife(int type, int life) { weponLife[type] = life; }
	//��ꂩ����
	bool GetBroken();
	//UI�p���v�Z
	int GetUIWidth(int type) { return uiWeponWidth[type]; };
	//UI�p�c�v�Z
	int GetUIHeight(int type) { return uiWeponHeight[type]; };

	DirectX::XMFLOAT3 GetUp() { return up; }
	DirectX::XMFLOAT3 GetRight() { return right; }
	DirectX::XMFLOAT3 GetFront() { return front; }
	void DrawUI(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);
protected:
	// �s��X�V����
	void UpdateTransform();
private:
	//�ʏ���
	void NomalUpdate(float elapsedTime);
	//�V�t�g���
	void ShiftUpdate(float elapsedTime);

	void WeponLifeUpdate(float elapsedTime);

	void UpdateUI(float elapsedTime);

	void WeponShieldUpdate();
private:
	enum WeponUI
	{
		MainSwordUI,
		AxeUI,
		HalberdUI,
		ShieldUI,
		SubSwordUI,
		WeponWAKU,
		MSHpUI,
		AXHpUI,
		HBHpUI,
		SDHpUI,
		SSHpUI,
		BreakUI,

		Max
	};
	//std::unique_ptr<Sprite> weponUI[Max] = {};
	DirectX::XMFLOAT3  position = { 0,0,0 };
	DirectX::XMFLOAT3  direction = { 0,0,1 };
	DirectX::XMFLOAT3  shiftDirection = { 0,0,0 };
	DirectX::XMFLOAT4  directionUp = { 0,1,0,0 };
	//UI�X�e�[�^�X x = �f�B�]���u���
	DirectX::XMFLOAT4  uvStatus = { 0,0,0,0 };
	//UI�V�[���h�X�e�[�^�X x = �f�B�]���u���
	DirectX::XMFLOAT4  uvShiedStatus = { 0,0,0,0 };
	DirectX::XMFLOAT3  scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	std::shared_ptr<Model> weapon[10];
	DirectX::XMFLOAT3  weaponPoint = {0,0,0};//����̐�
	DirectX::XMFLOAT3  weaponEPoint = { 0,0,0 };//�G�t�F�N�g�̉���
	DirectX::XMFLOAT3  weaponEFPoint = { 0,0,0 };//�G�t�F�N�g�̏㑤
	DirectX::XMFLOAT3  weaponPPoint = { 0,0,0 };//����̕�
	DirectX::XMFLOAT3  weaponHPoint = { 0,0,0 };//����̎茳
	DirectX::XMFLOAT3 right = { 0,0,0 };
	DirectX::XMFLOAT3 up = {0,0,0};
	DirectX::XMFLOAT3 front = {0,0,0};
	float radius = 1.0f;
	int mpChargeRate = 1;
	float weponHead[WeponMax] = { 0, 0.4f, 0.5f, 0, 0 };//���킲�Ƃ̎茳
	float weponLen[WeponMax] = { 1.5f, 1.6f, 2, 2, 0.5f };//���킲�Ƃ̐�
	float weponEFLen[WeponMax] = { 2.5f, 2.6f, 3, 4, 0.5f };//���킲�ƃG�t�F�N�g�̏㑤
	float weponELen[WeponMax] = { 0, 1, 1, 0.5f, 0 };//���킲�ƃG�t�F�N�g�̉���
	float weponPLen[WeponMax] = { -0.2f, -0.8f, -1, -0.5f, -1 };//���킲�Ƃ̕�



	int weponLife[WeponMax] = {0,0,0,0,0};//���킲�Ƃ�HP
	float weponRepsir[WeponMax] = { 1.8f, 1.7f, 1.6f, 1.5f, 1.4f };//���킲�Ƃ̏C�����x
	float maxLife[WeponMax] = {30,40,38,25,5};//���킲�Ƃ̍ő�HP
	float breakTimer = 1;//��ꂽ�C�����x
	int lifeAverage[WeponMax] = {1,1,1,1,1}; // ���킲�Ƃ�HP�Q�[�W
	float lifeSpeed = 0.5; // HP�Q�[�W�̍X�V���x
	bool breakflag[WeponMax] = {false,false,false,false,false};//���킲�Ƃ̉�ꂽ�t���O
	float brokenLife = 0.3f;//���Ă����銄��
	int swordState = 0;//����̏��
	int weponType = 0;
	int uiWeponWidth[WeponMax] = {0,1,-1,0,0};
	int uiWeponHeight[WeponMax] = {0,100,100,200,0};
	GaugeUI* uiWeponHp[Shield];
	BaseUI* uiUseWepon;
	float speed = 40;
	int damage = 1;
	float InvincibleTime = 1.0;
	int timer = 0;
	float angle = 0;
	DirectX::XMFLOAT3 a = {0,0,0};
	bool renderflag = true;
	std::vector<SkinnedMeshResouurce::constants> data;
	std::vector<SkinnedMeshResouurce::constants> shieldData;
};