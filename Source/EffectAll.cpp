#include "EffectAll.h"

// インスタンス取得
static EffectAll* instance = nullptr;

// インスタンス取得
EffectAll& EffectAll::Instance()
{
	return *instance;

}
EffectAll::EffectAll() {
	instance = this;
	
	//hitEffect = std::make_unique<Effect>(".\\resources\\Effect\\blood.efk");
	//hit2Effect = std::make_unique<Effect>(".\\resources\\Effect\\hit4.efk");
	//gudEffect = std::make_unique<Effect>(".\\resources\\Effect\\Gud6.efk");
	//quickEffect = std::make_unique<Effect>(".\\resources\\Effect\\skill1.efk");
	//boomEffect = std::make_unique<Effect>(".\\resources\\Effect\\inpact.efk");
	swordEffect = std::make_unique<Effect>(".\\resources\\Effect\\swordEffcet.efk");
	//sword2Effect = std::make_unique<Effect>(".\\resources\\Effect\\Attack2.efk");
	stingerEffect = std::make_unique<Effect>(".\\resources\\Effect\\Stinger2.efk");
	stampEffect = std::make_unique<Effect>(".\\resources\\Effect\\BloodLance.efk");
	fireBallEffect = std::make_unique<Effect>(".\\resources\\Effect\\FireBall.efk");
	//jumpEffect = std::make_unique<Effect>(".\\resources\\Effect\\jump.efk");
	//chagingEffect = std::make_unique<Effect>(".\\resources\\Effect\\chage.efk");
	//impactEffect = std::make_unique<Effect>(".\\resources\\Effect\\inpact11.efk");

}
EffectAll::~EffectAll() {
	

}