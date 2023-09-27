#pragma once
#include "Effect.h"
#include <memory.h>

using namespace DirectX;

class EffectAll
{
public:
	EffectAll();
	~EffectAll();

	// インスタンス取得
	static EffectAll& Instance();
public:
	//std::unique_ptr<Effect>hitEffect;
	//std::unique_ptr<Effect>hit2Effect;
	//std::unique_ptr<Effect>gudEffect;
	//std::unique_ptr<Effect>quickEffect;
	//std::unique_ptr<Effect>boomEffect;
	std::unique_ptr<Effect>swordEffect;
	//std::unique_ptr<Effect>sword2Effect;
	std::unique_ptr<Effect>stingerEffect;
	std::unique_ptr<Effect>stampEffect;
	std::unique_ptr<Effect>fireBallEffect;
	//std::unique_ptr<Effect>jumpEffect;
	//std::unique_ptr<Effect>chagingEffect;
	//std::unique_ptr<Effect>impactEffect;

};