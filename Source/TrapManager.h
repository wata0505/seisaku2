#pragma once

#include<vector>
#include"Trap.h"

//トラップマネージャー
class TrapManager
{
	TrapManager();
	~TrapManager() {}

public:
	//唯一のインスタンス取得
	static TrapManager& Instance()
	{
		static TrapManager instance;
		return instance;
	}

	//トラップ登録
	void Register(Trap* Trap);

	void SetTrap();

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, ModelShader* shader);

	void Text(SpriteShader* shader, ID3D11DeviceContext* dc, std::string s, float x, float y, float w, float h, float r, float g, float b, float a);
	void Sprite2DRender(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);

	//トラップ全削除
	void Clear();

	//トラップ削除
	void Remove(Trap* Trap);

	//デバッグプリミティブ描画
	void DrawDebugPrimitive();

	//トラップ数取得
	int GetTrapCount()const { return static_cast<int>(traps.size()); }

	//トラップ取得
	Trap* GetTrap(int index) { return traps.at(index); }


	float GetTrapPoint() { return trapPoint; }
	void  SetTrapPoint(int point) { this->trapPoint = point; }


private:
	//トラップ同士の衝突処理
	void CollisionTrapVstraps();

	//衝突処理
	bool CollisionVsPlayer();
private:
	enum TrapType
	{
		TrapTurret,
		TrapMine,
		TrapTotem,
		TrapDecoy,

		TrapMax,
	};
	int type = 0;
	
	int cost[TrapMax] = { 1,2,3,4 };
	int trapPoint = 0;

	std::vector<Trap*> traps;
	std::vector<Trap*> removes;
	std::unique_ptr<Sprite> font = {};
};