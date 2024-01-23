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

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
    void Render(ID3D11DeviceContext* dc, ModelShader* shader);

	// 位置取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 方向取得
	const DirectX::XMFLOAT3& GetDirection() const { return direction; }

	// スケール取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// 半径所得
	const float GetRadius() const { return radius; }

	void SetRadius(float radius) { this->radius  = radius; }

	void SetSwordState(int type) { this->swordState = type; }
	//タイプ取得
	const int GetSwordState() const { return swordState; }

	// 無敵時間
	const float GetInvincibleTime() const { return InvincibleTime; }

	//ダメージ
	const int GetDamage() const { return damage; }

	void SetDamage(int damage) { this->damage = damage; }

	const int GetMpChargeRate() const { return mpChargeRate; }

	void SetMpChargeRate(int mpChargeRate) { this->mpChargeRate = mpChargeRate; }

	void SetDirection(DirectX::XMFLOAT3 d) { direction = d; };
	//武器の投げる方向
	void SetShiftDirection(DirectX::XMFLOAT3 d) { shiftDirection = d; };

	void SetA(float d) { angle = d; };

	void SetWeponType(int type) { this->weponType = type; }
	//タイプ取得
	const int GetWeponType() const { return weponType; }

	std::shared_ptr<Model> GetWeponModel(int t) { return weapon[t]; }

	// 位置取得
	DirectX::XMFLOAT3 GetWeaponPoint() const { return weaponPoint; }

	// 方向取得
	DirectX::XMFLOAT3 GetWeaponEPoint() const { return weaponEPoint; }
	DirectX::XMFLOAT3 GetWeaponEFPoint() const { return weaponEFPoint; }

	DirectX::XMFLOAT3 GetWeaponPPoint() const { return weaponPPoint; }

	DirectX::XMFLOAT3 GetWeaponHPoint() const { return weaponHPoint; }

	void SetRnderflag(bool flag) { renderflag = flag; }

	bool GetRnderflag() { return renderflag; }
	//ディゾルブ状態
	float GetDisslve() { return uvStatus.x; }
	//武器の消耗
	void Exhaustion(int life);
	//この武器が壊れているか
	bool GetBreakFlag(int type) { return breakflag[type]; }

	int GetWeponLife(int type) { return weponLife[type]; }

	float GetMaxWeponLife(int type) { return maxLife[type]; }

	void SetWeponLife(int type, int life) { weponLife[type] = life; }
	//壊れかけか
	bool GetBroken();
	//UI用横計算
	int GetUIWidth(int type) { return uiWeponWidth[type]; };
	//UI用縦計算
	int GetUIHeight(int type) { return uiWeponHeight[type]; };

	DirectX::XMFLOAT3 GetUp() { return up; }
	DirectX::XMFLOAT3 GetRight() { return right; }
	DirectX::XMFLOAT3 GetFront() { return front; }
	void DrawUI(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);
protected:
	// 行列更新処理
	void UpdateTransform();
private:
	//通常状態
	void NomalUpdate(float elapsedTime);
	//シフト状態
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
	//UIステータス x = ディゾルブ状態
	DirectX::XMFLOAT4  uvStatus = { 0,0,0,0 };
	//UIシールドステータス x = ディゾルブ状態
	DirectX::XMFLOAT4  uvShiedStatus = { 0,0,0,0 };
	DirectX::XMFLOAT3  scale = { 1,1,1 };
	DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	std::shared_ptr<Model> weapon[10];
	DirectX::XMFLOAT3  weaponPoint = {0,0,0};//武器の先
	DirectX::XMFLOAT3  weaponEPoint = { 0,0,0 };//エフェクトの下側
	DirectX::XMFLOAT3  weaponEFPoint = { 0,0,0 };//エフェクトの上側
	DirectX::XMFLOAT3  weaponPPoint = { 0,0,0 };//武器の柄
	DirectX::XMFLOAT3  weaponHPoint = { 0,0,0 };//武器の手元
	DirectX::XMFLOAT3 right = { 0,0,0 };
	DirectX::XMFLOAT3 up = {0,0,0};
	DirectX::XMFLOAT3 front = {0,0,0};
	float radius = 1.0f;
	int mpChargeRate = 1;
	float weponHead[WeponMax] = { 0, 0.4f, 0.5f, 0, 0 };//武器ごとの手元
	float weponLen[WeponMax] = { 1.5f, 1.6f, 2, 2, 0.5f };//武器ごとの先
	float weponEFLen[WeponMax] = { 2.5f, 2.6f, 3, 4, 0.5f };//武器ごとエフェクトの上側
	float weponELen[WeponMax] = { 0, 1, 1, 0.5f, 0 };//武器ごとエフェクトの下側
	float weponPLen[WeponMax] = { -0.2f, -0.8f, -1, -0.5f, -1 };//武器ごとの柄



	int weponLife[WeponMax] = {0,0,0,0,0};//武器ごとのHP
	float weponRepsir[WeponMax] = { 1.8f, 1.7f, 1.6f, 1.5f, 1.4f };//武器ごとの修復速度
	float maxLife[WeponMax] = {30,40,38,25,5};//武器ごとの最大HP
	float breakTimer = 1;//壊れた修復速度
	int lifeAverage[WeponMax] = {1,1,1,1,1}; // 武器ごとのHPゲージ
	float lifeSpeed = 0.5; // HPゲージの更新速度
	bool breakflag[WeponMax] = {false,false,false,false,false};//武器ごとの壊れたフラグ
	float brokenLife = 0.3f;//壊れてかける割合
	int swordState = 0;//武器の状態
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