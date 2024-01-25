#pragma once
#include"Sprite.h"
#include <memory>
//using namespace DirectX;

class EffectTexAll
{
public:
	enum  EfTexAll
	{
		Dfelat,
		BlackThunder,
		Thunder,
		Sumi,
		Bock,
		Distortion,
		Impact,
		Flame,
		BlueFlame,
		Hart,
		BlueThader,
		Metal,
		Barrier,
		BarrierEnemy,
		Max
	};
public:
	EffectTexAll();
	~EffectTexAll();

	// インスタンス取得
	static EffectTexAll& Instance();
	std::shared_ptr<Sprite> GetSprite(int index) { return efTex[index]; };
private:
	
	std::shared_ptr<Sprite> efTex[static_cast<int>(EfTexAll::Max)];

};