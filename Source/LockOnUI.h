#pragma once
#include "UI.h"

//ロックオンUIクラス
class LockOnUI : public UI
{
public:
	LockOnUI(const wchar_t* filename);
	~LockOnUI() {};

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	virtual void Render(RenderContext& rc, SpriteShader* shader);

	// 破棄
	void Destroy();

	//UIManager* manager = nullptr;
private:
	float lockonAngle;
};