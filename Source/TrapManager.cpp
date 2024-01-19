#include"TrapManager.h"
#include"Collision.h"

#include"Turret.h"
#include"Mine.h"
#include"Totem.h"
#include"Decoy.h"

#include"EnemyManager.h"
#include"Player.h"
#include "Input.h"

#include "StageManager.h"

#include <imgui.h>

TrapManager::TrapManager()
{
	uiTrap[0] = std::make_unique<Sprite>(L".\\resources\\Trap\\Turret.png");
	uiTrap[1] = std::make_unique<Sprite>(L".\\resources\\Trap\\Mine.png");
	uiTrap[2] = std::make_unique<Sprite>(L".\\resources\\Trap\\P.png");
	uiTrap[3] = std::make_unique<Sprite>(L".\\resources\\Trap\\Decoy.png");


	uiHp = std::make_unique<Sprite>(L".\\resources\\HP.png");
	sprite = std::make_unique<Sprite>(L".\\resources\\Trap\\TrapSlot.png");
	spriteNo = std::make_unique<Sprite>(L".\\resources\\Trap\\No.png");
	font = std::make_unique<Sprite>(L".\\resources\\Font\\Number.png");
	trapPoint = 100;
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
	//CollisionTrapVstraps();


	GamePad& gamePad = Input::Instance().GetGamePad();
	//トラップ設置（仮）
	{
		if (Player::Instance().GetHologramTimer() < 1.0f)
		{
			buildFlag = false;
		}
		else
		{
			if (buildFlag == false)
			{
				if (gamePad.GetButtonDown() & GamePad::BTN_LEFT)type--;
				if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)type++;
				if (type >= Trap::TrapType::TrapMax) type = Trap::TrapType::TrapTurret;//上限
				if (type < Trap::TrapType::TrapTurret)type = Trap::TrapType::TrapDecoy;//下限
			}

			if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_DOWN)
			{
				if (buildFlag)
				{
					buildFlag = false;
				}
				else
				{
					buildFlag = true;
					SetTrap();
				}
			}
		}
	}
	if (buildFlag)
	{
		canSetFlag = true;
		Trap* trap = GetTrap(GetTrapCount() - 1);
		//設置するトラップが被っていないか
		int TrapCount = static_cast<int>(traps.size());
		for (int a = 0; a < TrapCount; a++)
		{
			Trap* TrapA = traps.at(a);
			if (a == GetTrapCount() - 1)continue;
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersectSphereVsSphere(
				TrapA->GetPosition(), TrapA->GetRadius(),
				trap->GetPosition(), trap->GetRadius(),
				outPosition)
				)
			{
				//設置できない
				canSetFlag = false;
			}
		}

		//台との判定
		if (trap->GetType() == Trap::TrapType::TrapTurret)
		{						
			if (Player::Instance().GetPosition().y < 5)
			{
				canSetFlag = false;
			}
			else
			{
				// レイの開始位置は足元より少し上
				DirectX::XMFLOAT3 start = { trap->GetPosition().x, trap->GetPosition().y, trap->GetPosition().z };
				// 下げ位置
				float offset = 5.0f;
				float frontX = sinf(Player::Instance().GetAngle().y) * trap->GetRadius();
				float frontZ = cosf(Player::Instance().GetAngle().y) * trap->GetRadius();
				DirectX::XMFLOAT3 frontStart = { trap->GetPosition().x + frontX, trap->GetPosition().y, trap->GetPosition().z + frontZ };
				DirectX::XMFLOAT3 backStart = { trap->GetPosition().x - frontX, trap->GetPosition().y, trap->GetPosition().z - frontZ };
				DirectX::XMFLOAT3 frontEnd = { trap->GetPosition().x + frontX, trap->GetPosition().y - offset, trap->GetPosition().z + frontZ };
				DirectX::XMFLOAT3 backEnd = { trap->GetPosition().x - frontX, trap->GetPosition().y - offset, trap->GetPosition().z - frontZ };
				frontX = sinf(Player::Instance().GetAngle().y + DirectX::XM_PIDIV2) * trap->GetRadius();
				frontZ = cosf(Player::Instance().GetAngle().y + DirectX::XM_PIDIV2) * trap->GetRadius();
				DirectX::XMFLOAT3 rightStart = { trap->GetPosition().x + frontX, trap->GetPosition().y, trap->GetPosition().z + frontZ };
				DirectX::XMFLOAT3 leftStart = { trap->GetPosition().x - frontX, trap->GetPosition().y, trap->GetPosition().z - frontZ };
				DirectX::XMFLOAT3 rightEnd = { trap->GetPosition().x + frontX, trap->GetPosition().y - offset, trap->GetPosition().z + frontZ };
				DirectX::XMFLOAT3 leftEnd = { trap->GetPosition().x - frontX, trap->GetPosition().y - offset, trap->GetPosition().z - frontZ };
				HitResult hit;
#if 0
				// 多少台からはみ出ても置ける
				if (!StageManager::Instance().RayCast(start, frontEnd, hit) || !StageManager::Instance().RayCast(start, backEnd, hit) ||
					!StageManager::Instance().RayCast(start, leftEnd, hit) || !StageManager::Instance().RayCast(start, rightEnd, hit))
#else
				// ほとんど台からはみ出てない限り置ける
				if (!StageManager::Instance().RayCast(frontStart, frontEnd, hit) || !StageManager::Instance().RayCast(backStart, backEnd, hit) ||
					!StageManager::Instance().RayCast(leftStart, leftEnd, hit) || !StageManager::Instance().RayCast(rightStart, rightEnd, hit))
#endif
				{
					canSetFlag = false;
				}
			}
		}
		else
		{			
			if (Player::Instance().GetPosition().y > 5)
			{
				canSetFlag = false;
			}
			else
			{
				// 上げ位置
				float offset = 1.0f;				
				DirectX::XMFLOAT3 start = { Player::Instance().GetPosition().x, Player::Instance().GetPosition().y + offset, Player::Instance().GetPosition().z };
				float frontX = sinf(Player::Instance().GetAngle().y) * trap->GetRadius();
				float frontZ = cosf(Player::Instance().GetAngle().y) * trap->GetRadius();				
				DirectX::XMFLOAT3 frontEnd = { trap->GetPosition().x + frontX, trap->GetPosition().y + offset, trap->GetPosition().z + frontZ };
				frontX = sinf(Player::Instance().GetAngle().y + DirectX::XM_PIDIV2) * trap->GetRadius();
				frontZ = cosf(Player::Instance().GetAngle().y + DirectX::XM_PIDIV2) * trap->GetRadius();				
				DirectX::XMFLOAT3 rightEnd = { trap->GetPosition().x + frontX, trap->GetPosition().y + offset, trap->GetPosition().z + frontZ };
				DirectX::XMFLOAT3 leftEnd = { trap->GetPosition().x - frontX, trap->GetPosition().y + offset, trap->GetPosition().z - frontZ };
				HitResult hit;
				// 台に重なるように置けない
				if (StageManager::Instance().RayCast(start, frontEnd, hit) || StageManager::Instance().RayCast(start, leftEnd, hit) || StageManager::Instance().RayCast(start, rightEnd, hit))
				{
					canSetFlag = false;
				}
			}			
		}
		//ポイントが足りない、着地していない
		if (cost[type] > trapPoint || Player::Instance().IsGround() == false)
		{
			canSetFlag = false;
		}
		
		//位置調整
		if (trap->GetActiveFlag() == false)
		{
			float frontX = sinf(Player::Instance().GetAngle().y);
			float frontZ = cosf(Player::Instance().GetAngle().y);
			float add = 3.5f;
			if (type == Trap::TrapType::TrapTurret)
			{
				trap->SetPosition({ (Player::Instance().GetPosition().x + frontX * add),(Player::Instance().GetPosition().y + 2.5f),(Player::Instance().GetPosition().z + frontZ * add) });
			}
			else
			{
				trap->SetPosition({ (Player::Instance().GetPosition().x + frontX * add),(Player::Instance().GetPosition().y),(Player::Instance().GetPosition().z + frontZ * add) });
			}
			trap->SetAngle(Player::Instance().GetAngle());
		}

		//左切り替え
		if (gamePad.GetButtonDown() & GamePad::BTN_LEFT)
		{
			type--;
			if (type < Trap::TrapType::TrapTurret)type = Trap::TrapType::TrapDecoy;//下限
			trap->Destroy();

			SetTrap();
		}
		//右切り替え
		if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)
		{
			type++;
			if (type >= Trap::TrapType::TrapMax) type = Trap::TrapType::TrapTurret;//上限
			trap->Destroy();
			SetTrap();
		}

		//ポイント消費・アクティブ化
		if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_UP)
		{
			//設置可能である
			if (canSetFlag)
			{
				trapPoint -= cost[type];
				trap->SetActiveFlag(true);
				SetTrap();
			}
		}
	}

	//アクティブにしなかったら削除
	if (buildFlag == false && GetTrapCount() >= 1)
	{
		Trap* trap = GetTrap(GetTrapCount() - 1);
		if (trap->GetActiveFlag() == false)
		{
			trap->Destroy();
		}
	}

	if (canSetFlag)
	{
		hologramColor = { 0.0f, 1.0f, 0.0f };
	}
	else
	{
		hologramColor = { 1.0f, 0.0f, 0.0f };
	}
}

//描画処理
void TrapManager::Render(ID3D11DeviceContext* context, ModelShader* shader)
{
	for (Trap* Trap : traps)
	{
		Trap->Render(context, shader);
	}
}
//描画処理
void TrapManager::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)
{
	for (Trap* Trap : traps)
	{
		Trap->Afterimagerender(immediate_context, shader);
	}
}
//トラップのデバッグプリミティブ描画
void TrapManager::DrawDebugPrimitive()
{
	for (Trap* Trap : traps)
	{
		Trap->DrawDebugPrimitive();
	}
}
// デバッグ情報表示
void TrapManager::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(980, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Trap", nullptr, ImGuiWindowFlags_None))
	{
		for (Trap* trap : traps) {
			// トラップ情報表示
			trap->DrawDebugGUI();
		}
	}
	ImGui::End();
}


//トラップ登録
void TrapManager::Register(Trap* Trap)
{
	traps.emplace_back(Trap);
}

void TrapManager::SetTrap()
{
	float frontX = sinf(Player::Instance().GetAngle().y);
	float frontZ = cosf(Player::Instance().GetAngle().y);
	float add = 3.5f;
	switch (type)
	{
	case Trap::TrapType::TrapTurret:
	{
		//台上
		Turret* turret = new Turret();
		turret->SetPosition({ (Player::Instance().GetPosition().x + frontX * add),(Player::Instance().GetPosition().y + 2.5f),(Player::Instance().GetPosition().z + frontZ * add) });
		Register(turret);
		break;
	}
	case Trap::TrapType::TrapMine:
	{
		//地面
		Mine* mine = new Mine();
		mine->SetPosition({ (Player::Instance().GetPosition().x + frontX * add),(Player::Instance().GetPosition().y),(Player::Instance().GetPosition().z + frontZ * add) });
		Register(mine);
		break;
	}
	case Trap::TrapType::TrapTotem:
	{
		//地面
		Totem* totem = new Totem();
		totem->SetPosition({ (Player::Instance().GetPosition().x + frontX * add),(Player::Instance().GetPosition().y),(Player::Instance().GetPosition().z + frontZ * add) });
		Register(totem);
		break;
	}
	case Trap::TrapType::TrapDecoy:
	{
		Decoy* decoy = new Decoy();
		decoy->SetPosition({ (Player::Instance().GetPosition().x + frontX * add),(Player::Instance().GetPosition().y),(Player::Instance().GetPosition().z + frontZ * add) });
		Register(decoy);
		break;
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




void TrapManager::Sprite2DRender(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader)
{
	// ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);
	// 変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	TrapManager::Instance().Text(shader, dc, std::to_string(trapPoint), 50, 50, 100, 100, 0.0f, 1.0f, 0.0f, 1.0f);

	DirectX::XMFLOAT2 positon = { 1100.0f, 480.0f };
	float size = 80.0f;
	//枠
	sprite->Render(dc,
		positon.x, positon.y, 2.0f * size, 3.0f * size,
		0.0f, 0.0f,
		static_cast<float>(sprite->GetTextureWidth()), static_cast<float>(sprite->GetTextureHeight()),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
	shader->Draw(rc, sprite.get());

	//各トラップ類
	uiTrap[type]->Render(dc,
		positon.x, positon.y, 2.0f * size, 3.0f * size,
		0.0f, 0.0f,
		static_cast<float>(uiTrap[type]->GetTextureWidth()), static_cast<float>(uiTrap[type]->GetTextureHeight()),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
	shader->Draw(rc, uiTrap[type].get());
	TrapManager::Instance().Text(shader, dc, std::to_string(cost[type]), positon.x + 115, positon.y + 137, 22, 22, 1.0f, 0.0f, 0.0f, 1.0f);

	//使用不可マーク表示
	if (!buildFlag && type == Trap::TrapType::TrapTurret && Player::Instance().GetPosition().y < 5 ||
		!buildFlag && !type == Trap::TrapType::TrapTurret && Player::Instance().GetPosition().y > 5 ||
		!canSetFlag && buildFlag)
	{
		spriteNo->Render(dc,
			positon.x, positon.y, 2.0f * size, 3.0f * size,
			0.0f, 0.0f,
			static_cast<float>(spriteNo->GetTextureWidth()), static_cast<float>(spriteNo->GetTextureHeight()),
			0.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		);
		shader->Draw(rc, spriteNo.get());
	}


	//HPゲージ描画
	int TrapCount = static_cast<int>(traps.size());
	for (int i = 0; i < TrapCount; i++)
	{
		Trap* trap = traps.at(i);
		float gaugeThrate = trap->GetHealth() / static_cast<float>(trap->GetMaxHealth());
		if (trap->GetHpRenderFlag() && trap->GetHealth() > 0)
		{
			//設置場所に表示
			DirectX::XMFLOAT3 objPosition = trap->GetPosition();
			//objPosition.y += trap->GetHeight();
			DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&objPosition);

			// ワールド座標からスクリーン座標へ変換
			DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
				WorldPosition,
				viewport.TopLeftX,
				viewport.TopLeftY,
				viewport.Width,
				viewport.Height,
				viewport.MinDepth,
				viewport.MaxDepth,
				Projection,
				View,
				World
			);
			// スクリーン座標
			DirectX::XMFLOAT2 screenPosition;
			DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);
			uiHp->Render(rc.deviceContext,
				screenPosition.x - 200.0f * 0.5f,
				screenPosition.y - 20.0f * 0.5f,
				200.0f * gaugeThrate,
				20.0f,
				0.0f, 0.0f,
				static_cast<float>(uiHp->GetTextureWidth() * gaugeThrate),
				static_cast<float>(uiHp->GetTextureHeight()),
				0.0f,
				3.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f
			);
			shader->Draw(rc, uiHp.get());
		}
	}
}

void TrapManager::Text(SpriteShader* shader, ID3D11DeviceContext* dc, std::string s, float x, float y, float w, float h, float r, float g, float b, float a)
{
	RenderContext rc;
	rc.deviceContext = dc;

	//現在の文字位置（相対位置）
	float carriage = 0;

	//文字数分だけrender()を呼び出す
	for (const char c : s)
	{
		//文字を表示。アスキーコードの位置にある文字位置を切り抜いて表示
		font->Render(dc,
			x + carriage / 2, y, w, h,
			72.0f * (c & 0x0F), 0.0f, 72.0f, 72.0f,
			0.0f,
			r, g, b, a);
		shader->Draw(rc, font.get());
		//文字位置を幅分ずらす
		carriage += w;
	}
}