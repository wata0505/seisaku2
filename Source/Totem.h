#pragma once

#include "Trap.h"

class Totem : public Trap
{
public:
	Totem();
	~Totem() {}

public:
	// �X�V����
	void Update(float elapsedTime) override;
	// �`�揈��
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) override;
	// �c���G�t�F�N�g�`�揈��
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, ModelShader* shader) override {}
	// �f�o�b�O�v���~�e�B�u�`�揈��
	void DrawDebugPrimitive() override;
	// �f�o�b�O���\��
	void DrawDebugGUI() override;

	// �G�Ƃ̏Փˏ���
	void CollisionVsEnemies();

private:
	std::unique_ptr<Model> model = nullptr;	// ���f��
	float survivalTime = 0.0f;				// ��������
};