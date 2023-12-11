#pragma once

#include<vector>
#include"Trap.h"

//�g���b�v�}�l�[�W���[
class TrapManager
{
	TrapManager();
	~TrapManager() {}

public:
	//�B��̃C���X�^���X�擾
	static TrapManager& Instance()
	{
		static TrapManager instance;
		return instance;
	}

	//�g���b�v�o�^
	void Register(Trap* Trap);

	void SetTrap();

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, ModelShader* shader);

	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader);

	void Text(SpriteShader* shader, ID3D11DeviceContext* dc, std::string s, float x, float y, float w, float h, float r, float g, float b, float a);
	void Sprite2DRender(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);

	//�g���b�v�S�폜
	void Clear();

	//�g���b�v�폜
	void Remove(Trap* Trap);

	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	//�g���b�v���擾
	int GetTrapCount()const { return static_cast<int>(traps.size()); }

	//�g���b�v�擾
	Trap* GetTrap(int index) { return traps.at(index); }


	float GetTrapPoint() { return trapPoint; }
	void  SetTrapPoint(int point) { this->trapPoint = point; }


private:
	//�g���b�v���m�̏Փˏ���
	void CollisionTrapVstraps();

	//�Փˏ���
	bool CollisionVsPlayer();

private:

	int type = Trap::TrapType::TrapTurret;

	int cost[Trap::TrapType::TrapMax] = { 1,2,3,4 };
	int trapPoint = 0;


	std::vector<Trap*> traps;
	std::vector<Trap*> removes;
	std::unique_ptr<Sprite> font = {};

	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Sprite> spriteNo;
	std::unique_ptr<Sprite> uiHp;
	std::unique_ptr<Sprite> uiTrap[Trap::TrapType::TrapMax];


	bool buildFlag = false;
};