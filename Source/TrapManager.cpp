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


	GamePad& gamePad = Input::Instance().GetGamePad();
	//�g���b�v�ݒu�i���j
	{

		if (buildFlag == false)
		{
			if (gamePad.GetButtonDown() & GamePad::BTN_LEFT)type--;
			if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)type++;
			if (type >= Trap::TrapType::TrapMax) type = Trap::TrapType::TrapTurret;//���
			if (type < Trap::TrapType::TrapTurret)type = Trap::TrapType::TrapDecoy;//����
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
	if (buildFlag)
	{
		canSetFlag = true;
		Trap* trap = GetTrap(GetTrapCount() - 1);
		//�ݒu����g���b�v������Ă��Ȃ���
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
				//�ݒu�ł��Ȃ�
				canSetFlag = false;
			}
		}
		//��Ƃ̔���
		if (trap->GetType() == Trap::TrapType::TrapTurret)
		{
			// ���C�̊J�n�ʒu�͑�����菭����
			DirectX::XMFLOAT3 start = { trap->GetPosition().x,trap->GetPosition().y ,trap->GetPosition().z };
			// ���C�̏I�_�ʒu�͈ړ���̈ʒu
			DirectX::XMFLOAT3 end = { trap->GetPosition().x,trap->GetPosition().y - 5 ,trap->GetPosition().z };
			HitResult hit;
			if (Player::Instance().GetPosition().y > 5 && StageManager::Instance().RayCast(start, end, hit) == false)
			{
				canSetFlag = false;
			}
		}
		else
		{
			// ���C�̊J�n�ʒu�͑�����菭����
			DirectX::XMFLOAT3 start = { trap->GetPosition().x,trap->GetPosition().y+10 ,trap->GetPosition().z };
			// ���C�̏I�_�ʒu�͈ړ���̈ʒu
			DirectX::XMFLOAT3 end = { trap->GetPosition().x,trap->GetPosition().y,trap->GetPosition().z };
			HitResult hit;
			if (Player::Instance().GetPosition().y < 5 && StageManager::Instance().RayCast(start, end, hit))
			{
				canSetFlag = false;
			}
		}
		//�|�C���g������Ȃ��A���n���Ă��Ȃ�
		if (cost[type] > trapPoint || Player::Instance().IsGround() == false)
		{
			canSetFlag = false;
		}
		
		//�ʒu����
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
		}

		//���؂�ւ�
		if (gamePad.GetButtonDown() & GamePad::BTN_LEFT)
		{
			type--;
			if (type < Trap::TrapType::TrapTurret)type = Trap::TrapType::TrapDecoy;//����
			trap->Destroy();

			SetTrap();
		}
		//�E�؂�ւ�
		if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)
		{
			type++;
			if (type >= Trap::TrapType::TrapMax) type = Trap::TrapType::TrapTurret;//���
			trap->Destroy();
			SetTrap();
		}

		//�|�C���g����E�A�N�e�B�u��
		if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_UP)
		{
			//�ݒu�\�ł���
			if (canSetFlag)
			{
				trapPoint -= cost[type];
				trap->SetActiveFlag(true);
				SetTrap();
			}
		}
	}

	//�A�N�e�B�u�ɂ��Ȃ�������폜
	if (buildFlag == false && GetTrapCount() >= 1)
	{
		Trap* trap = GetTrap(GetTrapCount() - 1);
		if (trap->GetActiveFlag() == false)
		{
			trap->Destroy();
		}
	}
}

//�`�揈��
void TrapManager::Render(ID3D11DeviceContext* context, ModelShader* shader)
{
	for (Trap* Trap : traps)
	{
		Trap->Render(context, shader);
	}
}
//�`�揈��
void TrapManager::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)
{
	for (Trap* Trap : traps)
	{
		Trap->Afterimagerender(immediate_context, shader);
	}
}



//�g���b�v�o�^
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
		//���
		Turret* turret = new Turret();
		turret->SetPosition({ (Player::Instance().GetPosition().x + frontX * add),(Player::Instance().GetPosition().y + 2.5f),(Player::Instance().GetPosition().z + frontZ * add) });
		Register(turret);
		break;
	}
	case Trap::TrapType::TrapMine:
	{
		//�n��
		Mine* mine = new Mine();
		mine->SetPosition({ (Player::Instance().GetPosition().x + frontX * add),(Player::Instance().GetPosition().y),(Player::Instance().GetPosition().z + frontZ * add) });
		Register(mine);
		break;
	}
	case Trap::TrapType::TrapTotem:
	{
		//�n��
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




void TrapManager::Sprite2DRender(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader)
{
	// �r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);
	// �ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	TrapManager::Instance().Text(shader, dc, std::to_string(trapPoint), 50, 50, 100, 100, 0.0f, 1.0f, 0.0f, 1.0f);

	DirectX::XMFLOAT2 positon = { 1100,480 };
	float size = 80;
	//�g
	sprite->Render(dc,
		positon.x, positon.y, 2 * size, 3 * size,
		0, 0,
		sprite->GetTextureWidth(), sprite->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
	shader->Draw(rc, sprite.get());

	//�e�g���b�v��
	uiTrap[type]->Render(dc,
		positon.x, positon.y, 2 * size, 3 * size,
		0, 0,
		uiTrap[type]->GetTextureWidth(), uiTrap[type]->GetTextureHeight(),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
	shader->Draw(rc, uiTrap[type].get());
	TrapManager::Instance().Text(shader, dc, std::to_string(cost[type]), positon.x + 115, positon.y + 137, 22, 22, 1.0f, 0.0f, 0.0f, 1.0f);

	//�g�p�s�}�[�N�\��
	if (Player::Instance().IsGround() == false ||
		(Player::Instance().GetPosition().y > 5 && type != Trap::TrapType::TrapTurret) ||
		(Player::Instance().GetPosition().y < 5 && type == Trap::TrapType::TrapTurret))
	{
		//���ŁA�^���b�g����Ȃ�
		spriteNo->Render(dc,
			positon.x, positon.y, 2 * size, 3 * size,
			0, 0,
			spriteNo->GetTextureWidth(), spriteNo->GetTextureHeight(),
			0.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		);
		shader->Draw(rc, spriteNo.get());
	}


	//HP�Q�[�W�`��
	int TrapCount = static_cast<int>(traps.size());
	for (int i = 0; i < TrapCount; i++)
	{
		Trap* trap = traps.at(i);
		float gaugeThrate = trap->GetHealth() / static_cast<float>(trap->GetMaxHealth());
		if (trap->GetHpRenderFlag() && trap->GetHealth() > 0)
		{
			//�ݒu�ꏊ�ɕ\��
			DirectX::XMFLOAT3 objPosition = trap->GetPosition();
			//objPosition.y += trap->GetHeight();
			DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&objPosition);

			// ���[���h���W����X�N���[�����W�֕ϊ�
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
			// �X�N���[�����W
			DirectX::XMFLOAT2 screenPosition;
			DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);
			uiHp->Render(rc.deviceContext,
				screenPosition.x - 200 * 0.5,
				screenPosition.y - 20 * 0.5,
				200 * gaugeThrate,
				20,
				0, 0,
				static_cast<float>(uiHp->GetTextureWidth() * gaugeThrate),
				static_cast<float>(uiHp->GetTextureHeight()),
				0,
				3.0f, 1.0f, 1.0f, 1.0f, 0, 0
			);
			shader->Draw(rc, uiHp.get());
		}
	}
}

void TrapManager::Text(SpriteShader* shader, ID3D11DeviceContext* dc, std::string s, float x, float y, float w, float h, float r, float g, float b, float a)
{
	RenderContext rc;
	rc.deviceContext = dc;

	//���݂̕����ʒu�i���Έʒu�j
	float carriage = 0;

	//������������render()���Ăяo��
	for (const char c : s)
	{
		//������\���B�A�X�L�[�R�[�h�̈ʒu�ɂ��镶���ʒu��؂蔲���ĕ\��
		font->Render(dc,
			x + carriage / 2, y, w, h,
			72.0f * (c & 0x0F), 0.0f, 72.0f, 72.0f,
			0.0f,
			r, g, b, a);
		shader->Draw(rc, font.get());
		//�����ʒu�𕝕����炷
		carriage += w;
	}
}