#pragma once
#include<DirectXMath.h>
#include "shader.h"
#include "Model.h"
#include "Trap.h"
class Totem :public Trap
{
public:
	Totem();
	~Totem();
public:
	//�X�V����
	void Update(float elapsed_time)override;
	// �`�揈��
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;
	//�c���G�t�F�N�g�`��
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)override;
	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive()override;
	// �f�o�b�O���\��
	void DrawDebugGUI() override;

	void CollisionVsEnemies();

private:

	std::unique_ptr<Model> model = nullptr;
	//�`����i�[
	std::vector<SkinnedMeshResouurce::constants> renderdata;


};