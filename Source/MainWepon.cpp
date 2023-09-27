#include"MainWepon.h"
#include"Player.h"
#include"StageManager.h"
#include"ParticleSprite.h"
#include"Calculation.h"
#include"EffectTexAll.h"
#include "AudioAll.h"
#include "UIManager.h"
#include "GaugeUI.h"
static MainWepon* instance = nullptr;
MainWepon::MainWepon() {
	instance = this;
	weapon[WeponType::MainSword] = std::make_unique<Model>(".\\resources\\Weapon\\Sword\\PREF_sword_3.fbx", true);
	weapon[WeponType::MainSword]->ModelSerialize(".\\resources\\Weapon\\Sword\\PREF_sword_3.fbx");
	weapon[WeponType::MainSword]->ModelRegister(".\\resources\\Weapon\\Sword\\PREF_sword_3.fbx");
	weapon[WeponType::Axe] = std::make_unique<Model>(".\\resources\\Weapon\\Bardiche\\PREF_bardiche3.fbx", true);
	weapon[WeponType::Axe]->ModelSerialize(".\\resources\\Weapon\\Bardiche\\PREF_bardiche3.fbx");
	weapon[WeponType::Axe]->ModelRegister(".\\resources\\Weapon\\Bardiche\\PREF_bardiche3.fbx");
	weapon[WeponType::Halberd] = std::make_unique<Model>(".\\resources\\Weapon\\Halberd\\PREF_halberd_3.fbx", true);
	weapon[WeponType::Halberd]->ModelSerialize(".\\resources\\Weapon\\Halberd\\PREF_halberd_3.fbx");
	weapon[WeponType::Halberd]->ModelRegister(".\\resources\\Weapon\\Halberd\\PREF_halberd_3.fbx");
	weapon[WeponType::Shield] = std::make_unique<Model>(".\\resources\\Weapon\\Shield\\kiteshield2.fbx", true);
	weapon[WeponType::Shield]->ModelSerialize(".\\resources\\Weapon\\Shield\\kiteshield2.fbx");
	weapon[WeponType::Shield]->ModelRegister(".\\resources\\Weapon\\Shield\\kiteshield2.fbx");
	//weapon[WeponType::Spear] = std::make_unique<Model>(".\\resources\\Weapon\\Spear\\PREF_spear.fbx", true);
	//weapon[WeponType::Spear]->ModelSerialize(".\\resources\\Weapon\\Spear\\PREF_spear.fbx");
	//weapon[WeponType::Spear]->ModelRegister(".\\resources\\Weapon\\Spear\\PREF_spear.fbx");
	weapon[WeponType::SubSword] = std::make_unique<Model>(".\\resources\\Weapon\\Zweihander\\PREF_zweihander2.fbx", true);
	weapon[WeponType::SubSword]->ModelSerialize(".\\resources\\Weapon\\Zweihander\\PREF_zweihander2.fbx");
	weapon[WeponType::SubSword]->ModelRegister(".\\resources\\Weapon\\Zweihander\\PREF_zweihander2.fbx");

    scale.x = scale.y = scale.z = 1.5f;
    radius = 0.5f;
	UpdateTransform();
	for (int i = 0; i < 5; i++) {
		weapon[i]->UpdateBufferDara(transform);
	}

	weponUI[MainSwordUI] = std::make_unique<Sprite>(L".\\resources\\UI\\SW.PNG");
	weponUI[AxeUI] = std::make_unique<Sprite>(L".\\resources\\UI\\Axe2.PNG");
	weponUI[HalberdUI] = std::make_unique<Sprite>(L".\\resources\\UI\\HB.PNG");
	weponUI[ShieldUI] = std::make_unique<Sprite>(L".\\resources\\UI\\SD.PNG");
	weponUI[SubSwordUI] = std::make_unique<Sprite>(L".\\resources\\UI\\EX.PNG");
	weponUI[WeponWAKU] = std::make_unique<Sprite>(L".\\resources\\UI\\weponwaku.PNG");
	weponUI[MSHpUI] = std::make_unique<Sprite>();
	weponUI[AXHpUI] = std::make_unique<Sprite>();
	weponUI[HBHpUI] = std::make_unique<Sprite>();
	weponUI[SDHpUI] = std::make_unique<Sprite>();
	weponUI[SSHpUI] = std::make_unique<Sprite>();

	UIManager& uiManager = UIManager::Instance();
	const float gaugeWidth = 65.0f;
	const float gaugeHeight = 5.0f;
	for (int i = 0; i < Shield; i++) {
		uiWeponHp[i] = new GaugeUI(L".\\resources\\HP.png",100 + 100 * uiWeponWidth[i], 400+uiWeponHeight[i], gaugeWidth, gaugeHeight, weponLife[i], maxLife[i]);
	}
	uiUseWepon = new BaseUI(L".\\resources\\UI\\weponwaku.PNG", 100,300, gaugeWidth,100,true);
	weponUI[BreakUI] = std::make_unique<Sprite>(L".\\resources\\UI\\break.png");
	for (int i = 0; i < WeponMax; i++) {
		weponLife[i] = maxLife[i];
	}
	weapon[weponType]->UpdateBufferDara(transform);
	data = weapon[weponType]->GetBufferData();
	weapon[WeponType::Shield]->UpdateBufferDara(transform);
	shieldData = weapon[WeponType::Shield]->GetBufferData();
}
MainWepon& MainWepon::Instance()
{
	return *instance;
}
void MainWepon::Update(float elapsedTime)
{
	float speed = this->speed * elapsedTime;
	Player& player = Player::Instance();
	if (weponType == WeponType::MainSword && swordState == SwordState::Nomal) {
		WeponShieldUpdate();
		UpdateTransform();
		weapon[WeponType::Shield]->UpdateBufferDara(transform);
		shieldData = weapon[WeponType::Shield]->GetBufferData();
	}
	switch (swordState)
	{
	case SwordState::Nomal:
		NomalUpdate(elapsedTime);
		break;
	case SwordState::Shift:
		ShiftUpdate(elapsedTime);
		break;
	default:
		//directionUp = {0,1,0,0};

		break;
	}
	if (weponLife[weponType] < 0 && !breakflag[weponType]) {//武器が壊れたら
		breakflag[weponType] = true;
		ParticleSprite* particleSprite = new ParticleSprite(weaponPPoint, weaponPoint, ParticleSprite::ParticleTriangle, ParticleSprite::Diffusion, (int)EffectTexAll::EfTexAll::Distortion, 3000, 3);
		
		AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Break)->Play(false, SE);
		

	}

	//オブジェクト行列を更新
	UpdateTransform();

	WeponLifeUpdate(elapsedTime);

	UpdateUI(elapsedTime);

	weapon[weponType]->UpdateBufferDara(transform);
	data = weapon[weponType]->GetBufferData();
	//モデル行列更新
	//sword->UpdateWorldBufferData(transform);
	if (swordState != SwordState::Shift) {
		AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Sift)->Stop();
	}
	timer++;
}
void MainWepon::NomalUpdate(float elapsedTime) {
	Player& player = Player::Instance();
	transform = player.GetWeponTransform();
	position = player.GetHeadPos();
	direction = player.GetSworddir();
	directionUp = {-transform._21,-transform._22,-transform._23,transform._24};//武器の上の方向
	if (weponType == WeponType::Shield) {//盾なら
		direction = {0,1,0};
		float angle = player.GetAngle().y;
		DirectX::XMFLOAT3 up = { 0,0,0 };
		up.x = sinf(angle);
		up.z = cosf(angle);
		directionUp = { -up.x,0,-up.z,0 };
	}
	//武器の様々な位置情報を更新
	weaponHPoint = position;
	weaponPoint = Vector3::PosDir(position, direction, weponLen[weponType]);
	weaponEPoint = Vector3::PosDir(position, direction, weponELen[weponType]);
	weaponPPoint = Vector3::PosDir(position, direction, weponPLen[weponType]);
	weaponEFPoint = Vector3::PosDir(position, direction, weponEFLen[weponType]);
	position = Vector3::PosDir(position, direction, -weponHead[weponType]);
}
void MainWepon::ShiftUpdate(float elapsedTime) {
	Player& player = Player::Instance();
	AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Sift)->Play(true,SE);
	float speed = this->speed * elapsedTime;
	
	direction = shiftDirection;
	position.x += direction.x * speed;
	position.y += direction.y * speed;
	position.z += direction.z * speed;

	angle += 0.5;
	//武器ごとの回転
	switch (weponType)
	{
	    case WeponType::MainSword:
	    direction.y = -sin(angle);
	    direction.x = (cosf(angle) * direction.x);
	    direction.z = (cosf(angle) * direction.z);
	    break;
	    case WeponType::Axe:
	    	direction.y = -sin(angle);
	    	direction.x = cosf(angle) * direction.x;
	    	direction.z = cosf(angle) * direction.z;
	    	break;
	    case WeponType::SubSword:
	    	direction.x = sinf(angle);
	    	direction.y = 0;
	    	direction.z = cosf(angle);
			break;
		case WeponType::Halberd:
			direction.x = sinf(angle);
			direction.y = 0;
			direction.z = cosf(angle);
	    default:
	    	break;
	}
	//武器の様々な位置情報を更新
	weaponEPoint = Vector3::PosDir(position, direction, weponELen[weponType]);
	weaponEFPoint = Vector3::PosDir(position, direction, weponEFLen[weponType]);
	HitResult hit;
	if (StageManager::Instance().RayCast(position, weaponPoint, hit)) {
		swordState = SwordState::Stop;
	}

}
void MainWepon::WeponShieldUpdate() {
	Player& player = Player::Instance();
	
	position = player.GetShieldHeadPos();
	direction = { 0,1,0 };
	float angle = player.GetAngle().y;
	DirectX::XMFLOAT3 up = { 0,0,0 };
	up.x = sinf(angle);
	up.z = cosf(angle);
	directionUp = { -up.x,0,-up.z,0 };
	


}
void MainWepon::WeponLifeUpdate(float elapsedTime) {
	for (int i = 0; i < 5;i++) {//武器ごと耐久値を回復
		if (weponLife[i] < 0) weponLife[i] = 0;
		if (i == weponType ) continue;
		if (breakflag[i]) {//壊れていたら
			weponLife[i] += elapsedTime * breakTimer;
		}
		else
		{
			weponLife[i] += elapsedTime * weponRepsir[i];
		}
		if (weponLife[i] > maxLife[i]) {
			weponLife[i] = maxLife[i];
			breakflag[i] = false;
		}
		
	}
	if (!renderflag || breakflag[weponType]) {//描画されないか壊れていたらディゾルブで消す
		uvStatus.x += 2.5* elapsedTime;
		if (uvStatus.x > 1) {
			uvStatus.x = 1;
		}
	}
	else
	{
		uvStatus.x -= 2.5 * elapsedTime;//ディゾルブで出現
		if (uvStatus.x < 0) {
			uvStatus.x = 0; 
		}
	}
	Player& player = Player::Instance();
	//シールドディゾルブ
	if (!player.GetShildFlag() || !renderflag || breakflag[weponType]) {

		uvShiedStatus.x += 3.5 * elapsedTime;
		if (uvShiedStatus.x > 1) {
			uvShiedStatus.x = 1;
		}
	}
	else
	{
		uvShiedStatus.x -= 3.5 * elapsedTime;
		if (uvShiedStatus.x < 0) {
			uvShiedStatus.x = 0;
		}
	}
}
void MainWepon::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
	if (uvStatus.x != 1 ) {
	shader->Draw(dc, weapon[weponType].get(), data,uvStatus);
	if(weponType == WeponType::MainSword && uvShiedStatus.x != 1 && swordState == SwordState::Nomal)shader->Draw(dc, weapon[WeponType::Shield].get(), shieldData, uvShiedStatus);
	}
}
void MainWepon::UpdateTransform()
{
	DirectX::XMVECTOR Front, Up, Right;

	//前ベクトルを算出
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);

	//仮の上ベクトルを算出
	Up = DirectX::XMLoadFloat4(&directionUp);
	Up = DirectX::XMVector3Normalize(Up);

	//右ベクトルを算出
	Right = DirectX::XMVector3Cross(Up, Front);

	//上ベクトルを算出
	Up = DirectX::XMVector3Cross(Front, Right);

	//計算結果を取り出し
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
	transform._31 = (front.x) * 0.01 * scale.z;
	transform._32 = (front.y) * 0.01 * scale.z;
	transform._33 = (front.z) * 0.01 * scale.z;
	transform._34 = 0.0f;
	transform._41 = position.x;
	transform._42 = position.y;
	transform._43 = position.z;
	transform._44 = 1.0f;

	//発射方向
	this->direction = front;
}
void MainWepon::UpdateUI(float elapsedTime) {
	//武器の耐久値UI更新
	for (int i = 0; i < Shield; i++) {
		uiWeponHp[i]->SetGauge(weponLife[i]);
	}
	uiUseWepon->SetUIPos(100 + 100 * uiWeponWidth[weponType], 300.0f + uiWeponHeight[weponType]);
}
bool MainWepon::GetBroken() {
	//壊れかけか
	if (weponLife[weponType] / maxLife[weponType] < brokenLife) {
		return true;
	}
	return false;
}

void MainWepon::Exhaustion(int life) {
	weponLife[weponType] -= life;
	if (GetBroken()) {//壊れかけなら粒子をだす
		ParticleSprite* particleSprite = new ParticleSprite(weaponPPoint, weaponPoint, ParticleSprite::ParticleTriangle, ParticleSprite::Corruption,(int)EffectTexAll::EfTexAll::Distortion, 200, NULL,NULL,false);
	}
}
