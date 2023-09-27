#include "EffectTexAll.h"

// インスタンス取得
static EffectTexAll* instance = nullptr;

// インスタンス取得
EffectTexAll& EffectTexAll::Instance()
{
	return *instance;

}
EffectTexAll::EffectTexAll() {
	instance = this;
	efTex[static_cast<int>(EfTexAll::Dfelat)] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\dummy.png");
	efTex[static_cast<int>(EfTexAll::BlackThunder)] = std::make_unique<Sprite>(L".\\resources\\Textures\\aka.png");
	efTex[static_cast<int>(EfTexAll::Thunder)] = std::make_unique<Sprite>(L".\\resources\\Textures\\tex.jpg");
	efTex[static_cast<int>(EfTexAll::Sumi)] = std::make_unique<Sprite>(L".\\resources\\Textures\\sumi.png");
	efTex[static_cast<int>(EfTexAll::Bock)] = std::make_unique<Sprite>(L".\\resources\\Textures\\honn.png");
	efTex[static_cast<int>(EfTexAll::Distortion)] = std::make_unique<Sprite>(L".\\resources\\Textures\\sampke_23.jpg");
	efTex[static_cast<int>(EfTexAll::Impact)] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Shock_wave1.png");
	efTex[static_cast<int>(EfTexAll::Flame)] = std::make_unique<Sprite>(L".\\resources\\Textures\\Fire6.png");
	efTex[static_cast<int>(EfTexAll::BlueFlame)] = std::make_unique<Sprite>(L".\\resources\\Textures\\burFl.jpg");
	efTex[static_cast<int>(EfTexAll::Vortex)] = std::make_unique<Sprite>(L".\\resources\\Textures\\uzu.jpg");
	efTex[static_cast<int>(EfTexAll::BlueThader)] = std::make_unique<Sprite>(L".\\resources\\Textures\\burThader.jpg");
	efTex[static_cast<int>(EfTexAll::Metal)] = std::make_unique<Sprite>(L".\\resources\\Textures\\17.png");
	efTex[static_cast<int>(EfTexAll::Barrier)] = std::make_unique<Sprite>(L".\\resources\\Textures\\baria2.png");
	efTex[static_cast<int>(EfTexAll::BarrierEnemy)] = std::make_unique<Sprite>(L".\\resources\\Textures\\baria3.png");
}
EffectTexAll::~EffectTexAll() {
	//for (int i = 0; i < static_cast<int>(EfTexAll::Max); i++) {
	//	delete efTex[i];
	//}
}