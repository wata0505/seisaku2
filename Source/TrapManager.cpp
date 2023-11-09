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

//�X�V����
void TrapManager::Update(float elapsedTime)
{
	for (Trap* trap : traps)
	{
		trap->Update(elapsedTime);
	}
	//�j������
	for (Trap* trap : removes)
	{
		//std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Trap*>::iterator it = std::find(traps.begin(), traps.end(), trap);

		if (it != traps.end())
		{
			traps.erase(it);
		}

		//�g���b�v�̔j������
		delete trap;
	}
	//�j�����X�g���N���A
	removes.clear();

	//�g���b�v���m�̏Փˏ���
	CollisionTrapVstraps();

	//�g���b�v�ݒu�i���j
	//{
	//	GamePad& gamePad = Input::Instance().GetGamePad();
	//	if (gamePad.GetButtonDown() & GamePad::BTN_LEFT) type--;
	//	if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT) type++;
	//	if (type >= TrapType::TrapMax) type--;//���
	//	if (type < 0)type++;//����
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

//�`�揈��
void TrapManager::Render(ID3D11DeviceContext* context, ModelShader* shader)
{
	for (Trap* Trap : traps)
	{
		Trap->Render(context, shader);
	}
}

//�g���b�v�o�^
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

//�g���b�v�S�폜
void TrapManager::Clear()
{
	for (Trap* Trap : traps)
	{
		delete Trap;
	}
	traps.clear();
}

//�g���b�v�폜
void TrapManager::Remove(Trap* Trap)
{
	//�j�����X�g�ɒǉ�
	removes.emplace_back(Trap);
}


//�g���b�v�̃f�o�b�O�v���~�e�B�u�`��
void TrapManager::DrawDebugPrimitive()
{
	for (Trap* Trap : traps)
	{
		Trap->DrawDebugPrimitive();
	}
}

//�g���b�v���m�̏Փˏ���
void TrapManager::CollisionTrapVstraps()
{
	int TrapCount = static_cast<int>(traps.size());
	for (int a = 0; a < TrapCount; a++)
	{
		//��ƂȂ���̓G
		Trap* TrapA = traps.at(a);

		for (int b = 0; b < TrapCount; b++)
		{
			if (a == b)continue;
			//�ʒu�C������鑤�̓G
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
		//�Փˏ���
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

	//���݂̕����ʒu�i���Έʒu�j
	float carriage = 0;
	const float blend_factor[4] = { 1,1,1,1 };

	//������������render()���Ăяo��
	for (const char c : s)
	{
		//������\���B�A�X�L�[�R�[�h�̈ʒu�ɂ��镶���ʒu��؂蔲���ĕ\��
		font->Render(dc,
			x + carriage, y, w, h,
			72.0f * (c & 0x0F), 0.0f, 72.0f, 72.0f,
			0.0f,
			r, g, b, a);
		shader->Draw(rc, font.get());
		//�����ʒu�𕝕����炷
		carriage += w;
	}
}