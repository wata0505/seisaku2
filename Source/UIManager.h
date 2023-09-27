#pragma once

#include<vector>
#include<set>
#include"UI.h"

//弾丸マネージャー
class UIManager
{
public:
	UIManager() {};
	~UIManager() {};

	// 唯一のインスタンス取得
	static UIManager& Instance()
	{
		static UIManager instance;
		return instance;
	}
	// 更新処理
	virtual void Update(float elapsedTime);

	// 描画処理
	virtual void Render(RenderContext& rc, SpriteShader* shader);

	// オブジェクト登録
	void Register(UI* ui);

	//　オブジェクト全削除
	void Clear();

	// オブジェクト数取得
	int GetUICount() const { return static_cast<int>(uis.size()); }

	// オブジェクト取得
	UI* GetUI(int index) { return uis.at(index); }

	// オブジェクト削除
	void Remove(UI* ui);



private:
	std::vector<UI*> uis;
	std::set<UI*> removes;

};