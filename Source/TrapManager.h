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
	//残像エフェクト描画
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader);
	//デバッグプリミティブ描画
	void DrawDebugPrimitive();
	// デバッグ情報表示
	void DrawDebugGUI();

	void Text(SpriteShader* shader, ID3D11DeviceContext* dc, std::string s, float x, float y, float w, float h, float r, float g, float b, float a);
	void Sprite2DRender(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);

	//トラップ全削除
	void Clear();

	//トラップ削除
	void Remove(Trap* Trap);

	//トラップ数取得
	int GetTrapCount()const { return static_cast<int>(traps.size()); }

	//トラップ取得
	Trap* GetTrap(int index) { return traps.at(index); }


	float GetTrapPoint() { return trapPoint; }
	void  SetTrapPoint(int point) { this->trapPoint = point; }

	bool GetBuidFlag() { return buildFlag; }

	DirectX::XMFLOAT4 GetHologramColor() { return hologramColor; }

private:
	//トラップ同士の衝突処理
	void CollisionTrapVstraps();

	//衝突処理
	bool CollisionVsPlayer();

private:

	int type = Trap::TrapType::TrapTurret;

	int cost[Trap::TrapType::TrapMax] = { 1,2,3,4 };
	int trapPoint = 0;


	std::vector<Trap*> traps;
	std::vector<Trap*> removes;
	std::unique_ptr<Sprite> font = {};

	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Sprite> spriteNo;
	std::unique_ptr<Sprite> uiHp;
	std::unique_ptr<Sprite> uiTrap[Trap::TrapType::TrapMax];


	bool buildFlag = false;
	bool canSetFlag = false;

	DirectX::XMFLOAT4 hologramColor = { 0.0f, 1.0f, 0.0f, 1.0f }; // ホログラム色
};