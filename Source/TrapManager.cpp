#include"TrapManager.h"
#include"Collision.h"

#include"Turret.h"
#include"Mine.h"
#include"Totem.h"
#include"Decoy.h"


#include"Player.h"
#include "Input.h"

TrapManager::TrapManager()
{
	font = std::make_unique<Sprite>(L".\\resources\\Font\\Number.png");
}

//更新処理
void TrapManager::Update(float elapsedTime)
{
	for (Trap* trap : traps)
	{
		trap->Update(elapsedTime);
	}
	//破棄処理
	for (Trap* trap : removes)
	{
		//std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Trap*>::iterator it = std::find(traps.begin(), traps.end(), trap);

		if (it != traps.end())
		{
			traps.erase(it);
		}

		//トラップの破棄処理
		delete trap;
	}
	//破棄リストをクリア
	removes.clear();

	//トラップ同士の衝突処理
	CollisionTrapVstraps();

	//トラップ設置（仮）
	//{
	//	GamePad& gamePad = Input::Instance().GetGamePad();
	//	if (gamePad.GetButtonDown() & GamePad::BTN_LEFT) type--;
	//	if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT) type++;
	//	if (type >= TrapType::TrapMax) type--;//上限
	//	if (type < 0)type++;//下限
	//
	//	if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_UP && CollisionVsPlayer() == false)
	//	{
	//		SetTrap();
	//	}
	//	if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_DOWN)
	//	{
	//		trapPoint++;
	//	}
	//}
}

//描画処理
void TrapManager::Render(ID3D11DeviceContext* context, ModelShader* shader)
{
	for (Trap* Trap : traps)
	{
		Trap->Render(context, shader);
	}
}

//トラップ登録
void TrapManager::Register(Trap* Trap)
{
	traps.emplace_back(Trap);
}

void TrapManager::SetTrap()
{
	if (cost[type] <= trapPoint)
	{
		trapPoint -= cost[type];

		switch (type)
		{
		case TrapType::TrapTurret:
		{
			Turret* turret = new Turret();
			turret->SetPosition({ Player::Instance().GetPosition().x,Player::Instance().GetPosition().y +1.5f,Player::Instance().GetPosition().z});
			turret->SetTerritory(turret->GetPosition(), 10.0f);
			Register(turret);
			break;
		}
		case TrapType::TrapMine:
		{
			Mine* mine = new Mine();
			mine->SetPosition(Player::Instance().GetPosition());
			mine->SetTerritory(mine->GetPosition(), 10.0f);
			Register(mine);
			break;
		}
		case TrapType::TrapTotem:
		{
			Totem* totem = new Totem();
			totem->SetPosition(Player::Instance().GetPosition());
			totem->SetTerritory(totem->GetPosition(), 10.0f);
			Register(totem);
			break;
		}
		case TrapType::TrapDecoy:
		{
			Decoy* decoy = new Decoy();
			decoy->SetPosition(Player::Instance().GetPosition());
			decoy->SetTerritory(decoy->GetPosition(), 10.0f);
			Register(decoy);
			break;
		}
		}
	}
	

	
}

//トラップ全削除
void TrapManager::Clear()
{
	for (Trap* Trap : traps)
	{
		delete Trap;
	}
	traps.clear();
}

//トラップ削除
void TrapManager::Remove(Trap* Trap)
{
	//破棄リストに追加
	removes.emplace_back(Trap);
}


//トラップのデバッグプリミティブ描画
void TrapManager::DrawDebugPrimitive()
{
	for (Trap* Trap : traps)
	{
		Trap->DrawDebugPrimitive();
	}
}

//トラップ同士の衝突処理
void TrapManager::CollisionTrapVstraps()
{
	int TrapCount = static_cast<int>(traps.size());
	for (int a = 0; a < TrapCount; a++)
	{
		//基準となる方の敵
		Trap* TrapA = traps.at(a);

		for (int b = 0; b < TrapCount; b++)
		{
			if (a == b)continue;
			//位置修正される側の敵
			Trap* TrapB = traps.at(b);
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsSphere(
				TrapA->GetPosition(), TrapA->GetRadius(),
				TrapB->GetPosition(), TrapB->GetRadius(),
				outPosition)
				)
			{
				TrapB->SetPosition(outPosition);
			}
		}
	}
}

bool TrapManager::CollisionVsPlayer()
{
	int TrapCount = static_cast<int>(traps.size());
	for (int a = 0; a < TrapCount; a++)
	{
		Trap* TrapA = traps.at(a);
		//衝突処理
		DirectX::XMFLOAT3 outPosition;

		if (Collision::IntersectCylinderVsCylinder(
			TrapA->GetPosition(), TrapA->GetRadius(), TrapA->GetHeight(),
			Player::Instance().GetPosition(), Player::Instance().GetRadius(), Player::Instance().GetHeight(),
			outPosition))
		{
			return true;
		}
	}
	return false;
}

void TrapManager::Sprite2DRender(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader) {

	TrapManager::Instance().Text(shader, dc, std::to_string(cost[type]), 10, 10, 100, 100, 0.0f, 1.0f, 0.0f, 1.0f);
	TrapManager::Instance().Text(shader, dc, std::to_string(trapPoint), 100, 10, 100, 100, 0.0f, 0.0f, 1.0f, 1.0f);

}

void TrapManager::Text(SpriteShader* shader, ID3D11DeviceContext* dc, std::string s, float x, float y, float w, float h, float r, float g, float b, float a)
{
	RenderContext rc;
	rc.deviceContext = dc;

	//現在の文字位置（相対位置）
	float carriage = 0;
	const float blend_factor[4] = { 1,1,1,1 };

	//文字数分だけrender()を呼び出す
	for (const char c : s)
	{
		//文字を表示。アスキーコードの位置にある文字位置を切り抜いて表示
		font->Render(dc,
			x + carriage, y, w, h,
			72.0f * (c & 0x0F), 0.0f, 72.0f, 72.0f,
			0.0f,
			r, g, b, a);
		shader->Draw(rc, font.get());
		//文字位置を幅分ずらす
		carriage += w;
	}
}