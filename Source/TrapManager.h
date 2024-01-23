#pragma once

#include <vector>
#include "Trap.h"

class TrapManager
{
public:
	TrapManager();
	~TrapManager() {}

	// インスタンス取得
	static TrapManager& Instance()
	{
		static TrapManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11DeviceContext* deviceContext, ModelShader* shader);
	// インスタンシング描画処理
	void InstancingRender(ID3D11DeviceContext* deviceContext, ModelShader* shader);
	// 残像エフェクト描画処理
	void Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, ModelShader* shader);
	// デバッグプリミティブ描画処理
	void DrawDebugPrimitive();
	// デバッグ情報表示
	void DrawDebugGUI();

	// トラップ登録
	void Register(Trap* Trap);
	// トラップ削除
	void Remove(Trap* Trap);
	// トラップ全削除
	void Clear();

	void SetTrap();

	void Text(SpriteShader* shader, ID3D11DeviceContext* dc, std::string s, float x, float y, float w, float h, float r, float g, float b, float a);
	void Sprite2DRender(ID3D11DeviceContext* dc, RenderContext& rc, SpriteShader* shader);	

public:
	// トラップ取得
	Trap* GetTrap(int index) { return traps.at(index); }

	// ホログラム色取得
	float GetHologramColor() { return hologramColor; }

	// トラップ数取得
	int GetTrapCount() const { return static_cast<int>(traps.size()); }

	// 所持ポイント設定と取得
	void SetTrapPoint(int point) { this->trapPoint = point; }
	int GetTrapPoint() { return trapPoint; }

private:
	std::unique_ptr<Sprite> font = {};
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Sprite> spriteNo;
	std::unique_ptr<Sprite> uiHp;
	std::unique_ptr<Sprite> uiTrap[Trap::TrapType::TrapMax];
	std::unique_ptr<Model>	obj[3] = { nullptr };
	std::vector<Trap*>		traps;
	std::vector<Trap*>		removes;
	InstancingConstants		constants[3];
	float					hologramColor = 1;								// ホログラム色
	bool					buildFlag = false;
	bool					canSetFlag = false;
	int						type = Trap::TrapType::TrapTurret;
	int						cost[Trap::TrapType::TrapMax] = { 1,2,3,4 };
	int						trapPoint = 0;									// 所持ポイント
	int						instangNo[3] = { 0 };
};