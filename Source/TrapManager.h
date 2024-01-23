#pragma once

#include <vector>
#include "Trap.h"

class TrapManager
{
public:
	TrapManager();
	~TrapManager() {}

	// �C���X�^���X�擾
	static TrapManager& Instance()
	{
		static TrapManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader);
	// �C���X�^���V���O�`�揈��
	void InstancingRender(ID3D11DeviceContext* deviceContext, ModelShader* shader);
	// �c���G�t�F�N�g�`�揈��
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, ModelShader* shader);
	// �f�o�b�O�v���~�e�B�u�`�揈��
	void DrawDebugPrimitive();
	// �f�o�b�O���\��
	void DrawDebugGUI();

	// �g���b�v�o�^
	void Register(Trap* Trap);
	// �g���b�v�폜
	void Remove(Trap* Trap);
	// �g���b�v�S�폜
	void Clear();

	void SetTrap();

	void Text(SpriteShader* shader, ID3D11DeviceContext* dc, std::string s, float x, float y, float w, float h, float r, float g, float b, float a);
	void Sprite2DRender(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);	

public:
	// �g���b�v�擾
	Trap* GetTrap(int index) { return traps.at(index); }

	// �z���O�����F�擾
	float GetHologramColor() { return hologramColor; }

	// �g���b�v���擾
	int GetTrapCount() const { return static_cast<int>(traps.size()); }

	// �����|�C���g�ݒ�Ǝ擾
	void SetTrapPoint(int point) { this->trapPoint = point; }
	int GetTrapPoint() { return trapPoint; }

private:
	std::unique_ptr<Sprite> font = {};
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Sprite> spriteNo;
	std::unique_ptr<Sprite> uiHp;
	std::unique_ptr<Sprite> uiTrap[Trap::TrapType::TrapMax];
	std::unique_ptr<Model>	obj[3] = { nullptr };
	std::vector<Trap*>		traps;
	std::vector<Trap*>		removes;
	InstancingConstants		constants[3];
	float					hologramColor = 1;								// �z���O�����F
	bool					buildFlag = false;
	bool					canSetFlag = false;
	int						type = Trap::TrapType::TrapTurret;
	int						cost[Trap::TrapType::TrapMax] = { 1,2,3,4 };
	int						trapPoint = 0;									// �����|�C���g
	int						instangNo[3] = { 0 };
};