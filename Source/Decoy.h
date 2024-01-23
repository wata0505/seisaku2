#pragma once

#include "Trap.h"

class Decoy : public Trap
{
public:
	Decoy();
	~Decoy() {}

public:
	// �X�V����
	void Update(float elapsedTime) override;
	// �`�揈��
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader) override;
	// �c���G�t�F�N�g�`�揈��
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, ModelShader* shader) override {};
	// �f�o�b�O�v���~�e�B�u�`�揈��
	void DrawDebugPrimitive() override;
	// �f�o�b�O���\��
	void DrawDebugGUI() override;

private:
	std::unique_ptr<Model> model = nullptr;	// ���f��
};