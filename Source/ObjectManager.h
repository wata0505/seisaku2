#pragma once

#include<vector>
#include<set>
#include"Object.h"

//弾丸マネージャー
class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	// 更新処理
	virtual void Update(float elapsedTime);

	// 描画処理
	virtual void Render(ID3D11DeviceContext* dc, ModelShader* shader);

	// 描画処理
	virtual void TrailRender(RenderContext& rc, SwordTrailShader* shader);

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	// オブジェクト登録
	void Register(Object* Object);

	//　オブジェクト全削除
	void Clear();

	// オブジェクト数取得
	int GetObjectCount() const { return static_cast<int>(Objects.size()); }

	// オブジェクト取得
	Object* GetObject(int index) { return Objects.at(index); }

	// オブジェクト削除
	void Remove(Object* Object);



private:
	std::vector<Object*> Objects;
	std::set<Object*> removes;

};