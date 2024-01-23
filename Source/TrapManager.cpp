#include "TrapManager.h"
#include "Turret.h"
#include "Mine.h"
#include "Totem.h"
#include "Decoy.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "Player.h"
#include "StageManager.h"
#include "AudioAll.h"
#include "Input.h"
#include <imgui.h>

// �R���X�g���N�^
TrapManager::TrapManager()
{
	// �n��
	obj[0] = std::make_unique<Model>(".\\resources\\Trap\\Mine\\Mine.fbx", true);
	obj[0]->ModelSerialize(".\\resources\\Trap\\Mine\\Mine.fbx");
	obj[0]->ModelRegister(".\\resources\\Trap\\Mine\\Mine.fbx");	

	// �g�[�e��
	obj[1] = std::make_unique<Model>(".\\resources\\Trap\\AttacheCase\\AttacheCase.fbx", true);
	obj[1]->ModelSerialize(".\\resources\\Trap\\AttacheCase\\AttacheCase.fbx");
	obj[1]->ModelRegister(".\\resources\\Trap\\AttacheCase\\AttacheCase.fbx", "Texture\\Albedo.png");	

	// �f�R�C
	obj[2] = std::make_unique<Model>(".\\resources\\Trap\\Decoy\\Decoy.fbx", true);
	obj[2]->ModelSerialize(".\\resources\\Trap\\Decoy\\Decoy.fbx");
	obj[2]->ModelRegister(".\\resources\\Trap\\Decoy\\Decoy.fbx", "Texture\\Albedo.png");	

	uiTrap[0] = std::make_unique<Sprite>(L".\\resources\\Trap\\Turret.png");
	uiTrap[1] = std::make_unique<Sprite>(L".\\resources\\Trap\\Mine.png");
	uiTrap[2] = std::make_unique<Sprite>(L".\\resources\\Trap\\P.png");
	uiTrap[3] = std::make_unique<Sprite>(L".\\resources\\Trap\\Decoy.png");


	//uiHp = std::make_unique<Sprite>(L".\\resources\\HP.png");
	sprite = std::make_unique<Sprite>(L".\\resources\\Trap\\TrapSlot.png");
	spriteNo = std::make_unique<Sprite>(L".\\resources\\Trap\\No.png");
	font = std::make_unique<Sprite>(L".\\resources\\Font\\Number.png");
	trapPoint = 100;
}

// �X�V����
void TrapManager::Update(float elapsedTime)
{
	// �X�V����
	for (Trap* trap : traps)
	{
		trap->Update(elapsedTime);
	}

	// �j������
	for (Trap* trap : removes)
	{
		//std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Trap*>::iterator it = std::find(traps.begin(), traps.end(), trap);

		if (it != traps.end())
		{
			traps.erase(it);
		}

		// �g���b�v�̔j������
		delete trap;
	}
	// �j�����X�g���N���A
	removes.clear();

	GamePad& gamePad = Input::Instance().GetGamePad();
	// �g���b�v�ݒu
	{
		// �Q�[���J�n���o���͐ݒu�s��
		if (Player::Instance().GetHologramTimer() < 1.0f) 
		{
			buildFlag = false;
		}
		// �Q�[���J�n���o��
		else 
		{
			if (!buildFlag)
			{
				if (gamePad.GetButtonDown() & GamePad::BTN_LEFT)type--;
				if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)type++;
				if (type >= Trap::TrapType::TrapMax) type = Trap::TrapType::TrapTurret;//���
				if (type < Trap::TrapType::TrapTurret) type = Trap::TrapType::TrapDecoy;//����
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
				outPosition))
			{
				//�ݒu�ł��Ȃ�
				canSetFlag = false;
			}
		}

		// ��Ƃ̔���
		if (trap->GetType() == Trap::TrapType::TrapTurret)
		{						
			if (Player::Instance().GetPosition().y < 5)
			{
				canSetFlag = false;
			}
			else
			{
				// ���C�̊J�n�ʒu�͑�����菭����
				DirectX::XMFLOAT3 start = { trap->GetPosition().x, trap->GetPosition().y, trap->GetPosition().z };
				// �����ʒu
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
				// �����䂩��͂ݏo�Ă��u����
				if (!StageManager::Instance().RayCast(start, frontEnd, hit) || !StageManager::Instance().RayCast(start, backEnd, hit) ||
					!StageManager::Instance().RayCast(start, leftEnd, hit) || !StageManager::Instance().RayCast(start, rightEnd, hit))
#else
				// �قƂ�Ǒ䂩��͂ݏo�ĂȂ�����u����
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
			if (Player::Instance().GetPosition().y > 5.0f)
			{
				canSetFlag = false;
			}
			else
			{
				// �グ�ʒu
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
				// ��ɏd�Ȃ�悤�ɒu���Ȃ�
				if (StageManager::Instance().RayCast(start, frontEnd, hit) || StageManager::Instance().RayCast(start, leftEnd, hit) || StageManager::Instance().RayCast(start, rightEnd, hit))
				{
					canSetFlag = false;
				}
			}			
		}
		//�|�C���g������Ȃ��A���n���Ă��Ȃ�
		if (cost[type] > trapPoint || Player::Instance().IsGround() == false)
		{
			canSetFlag = false;
		}
		
		//�ʒu����
		if (!trap->GetIsActive())
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
				trap->SetIsActive(true);
				SetTrap();
				AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Establishment)->Stop();
				AudioAll::Instance().GetMusic((int)AudioAll::AudioMusic::Establishment)->Play(false, 0.3f);
			}
		}
	}

	//�A�N�e�B�u�ɂ��Ȃ�������폜
	if (!buildFlag && GetTrapCount() >= 1)
	{
		Trap* trap = GetTrap(GetTrapCount() - 1);
		if (!trap->GetIsActive())
		{
			trap->Destroy();
		}
	}

	if (canSetFlag)
	{
		hologramColor = 2.0f;
	}
	else
	{
		hologramColor = 1.0f;
	}
}

// �`�揈��
void TrapManager::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
	for (Trap* Trap : traps)
	{
		Trap->Render(deviceContext, shader);
	}
}
// �C���X�^���V���O�`�揈��
void TrapManager::InstancingRender(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
	for (int index = 0; index < 3; index++)
	{
		float yUp = 0.0f;
		if (index == 0)
		{
			yUp = 1.0f;
		}
		//shader->Draw(deviceContext, obj[index].get(), constants[index], instangNo[index], { 0.0f, 0.0f, 0.0f, yUp });
	}	
}
// �c���G�t�F�N�g�`�揈��
void TrapManager::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, ModelShader* shader)
{
	for (Trap* Trap : traps)
	{
		Trap->Afterimagerender(deviceContext, shader);
	}
}
// �f�o�b�O�v���~�e�B�u�`�揈��
void TrapManager::DrawDebugPrimitive()
{
	for (Trap* Trap : traps)
	{
		Trap->DrawDebugPrimitive();
	}
}
// �f�o�b�O���\��
void TrapManager::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(980, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Trap", nullptr, ImGuiWindowFlags_None))
	{
		for (Trap* trap : traps) 
		{
			// �g���b�v���\��
			trap->DrawDebugGUI();
		}
	}
	ImGui::End();
}

// �g���b�v�o�^
void TrapManager::Register(Trap* Trap)
{
	// �C���X�^���V���O���f���Ȃ�
	if (Trap->GetInstancing())
	{
#if 0
		switch (Trap->GetType())
		{
		case Trap::TrapType::TrapMine:		// �n��
			constants[0].world[instangNo[0]] = Trap->GetTransform();
			instangNo[0]++;
			break;
		case Trap::TrapType::TrapTotem:		// �g�[�e��
			constants[1].world[instangNo[1]] = Trap->GetTransform();
			instangNo[1]++;
			break;
		case Trap::TrapType::TrapDecoy:		// �f�R�C
			constants[2].world[instangNo[2]] = Trap->GetTransform();
			instangNo[2]++;
			break;
		}
#endif
		constants[Trap->GetType() - 1].world[instangNo[Trap->GetType() - 1]] = Trap->GetTransform();
		instangNo[Trap->GetType() - 1]++;
	}

	traps.emplace_back(Trap);
}
// �g���b�v�폜
void TrapManager::Remove(Trap* Trap)
{
	//�j�����X�g�ɒǉ�
	removes.emplace_back(Trap);
}
// �g���b�v�S�폜
void TrapManager::Clear()
{
	for (Trap* Trap : traps)
	{
		delete Trap;
	}
	traps.clear();
	buildFlag = false;
	canSetFlag = false;
	type = Trap::TrapType::TrapTurret;
	instangNo[0] = 0;
	instangNo[1] = 0;
	instangNo[2] = 0;
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

	TrapManager::Instance().Text(shader, dc, std::to_string(trapPoint), 165, 630, 50, 50, 0.0f, 1.0f, 0.0f, 1.0f);

	DirectX::XMFLOAT2 positon = { 1100.0f, 480.0f };
	float size = 80.0f;
	//�g
	sprite->Render(dc,
		positon.x, positon.y, 2.0f * size, 3.0f * size,
		0.0f, 0.0f,
		static_cast<float>(sprite->GetTextureWidth()), static_cast<float>(sprite->GetTextureHeight()),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
	shader->Draw(rc, sprite.get());

	//�e�g���b�v��
	uiTrap[type]->Render(dc,
		positon.x, positon.y, 2.0f * size, 3.0f * size,
		0.0f, 0.0f,
		static_cast<float>(uiTrap[type]->GetTextureWidth()), static_cast<float>(uiTrap[type]->GetTextureHeight()),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
	shader->Draw(rc, uiTrap[type].get());
	TrapManager::Instance().Text(shader, dc, std::to_string(cost[type]), positon.x + 115, positon.y + 137, 22, 22, 1.0f, 0.0f, 0.0f, 1.0f);

	//�g�p�s�}�[�N�\��
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

#if 0
	// HP�Q�[�W�`��
	int TrapCount = static_cast<int>(traps.size());
	for (int i = 0; i < TrapCount; i++)
	{
		Trap* trap = traps.at(i);
		float gaugeThrate = trap->GetHealth() / static_cast<float>(trap->GetMaxHealth());
		if (trap->GetHpRenderFlag() && trap->GetHealth() > 0)
		{
			// �ݒu�ꏊ�ɕ\��
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
#endif
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