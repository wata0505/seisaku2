#pragma once
#include "Scene.h"

//���[�f�B���O�V�[��
class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
	~SceneLoading() {}
	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render()override;

private:
	//���[�f�B���O�X���b�h
	static void LoadingThread(SceneLoading* scene);

private:
	float angle = 0.0f;
	Scene* nextScene = nullptr;
	//�q���g�ő吔
	static const int tipsMax{ 2 };
	//�q���g�ԍ�
	int tipsNo = 0;
	//�q���g�\������
	float tipsTimer = 0;
	//�q���g
	std::unique_ptr<Sprite> tips[tipsMax];
	std::shared_ptr<Sprite> sprite[2];
	//�������
	std::unique_ptr<Sprite> sousasetumei[2];
	
};