#pragma once
#include<DirectXMath.h>
#include "shader.h"
#include "Model.h"
#include "Trap.h"
#include "ObjectManager.h"
class Mine :public Trap
{
public:
	Mine();
	~Mine();
public:
	//�X�V����
	void Update(float elapsed_time)override;
	// �`�揈��
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;
	//�c���G�t�F�N�g�`��
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)override;
	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive()override;

	void CollisionVsEnemies();

private:

	std::unique_ptr<Model> model = nullptr;
	//�`����i�[
	std::vector<SkinnedMeshResouurce::constants> renderdata;
	//�e�֌W
	ObjectManager objectManager;

};