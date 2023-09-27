#pragma once
#include "Scene.h"

//ローディングシーン
class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
	~SceneLoading() {}
	// 初期化
	void Initialize()override;

	// 終了化
	void Finalize()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render()override;

private:
	//ローディングスレッド
	static void LoadingThread(SceneLoading* scene);

private:
	float angle = 0.0f;
	Scene* nextScene = nullptr;
	//ヒント最大数
	static const int tipsMax{ 2 };
	//ヒント番号
	int tipsNo = 0;
	//ヒント表示時間
	float tipsTimer = 0;
	//ヒント
	std::unique_ptr<Sprite> tips[tipsMax];
	std::shared_ptr<Sprite> sprite[2];
	//操作説明
	std::unique_ptr<Sprite> sousasetumei[2];
	
};