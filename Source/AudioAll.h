#pragma once
#include"AudioSource.h"
#define SE 0.1
#define BGM 0.01
#define DOWNSE 0.5
class AudioAll
{
public:
	enum   AudioMusic
	{
		Title,
		Bgm,
		BgmBoss,
		Ketei,
		Wolk,
		Run,
		Slash1,
		Slash2,
		Swing1,
		Sift,
		boom1,
		boom2,
		SlashFly,
		Rock,
		Chage,
		Change,
		Break,
		Strike,
		Hit,
		Gud,
		DragonRor,
		DragonFry,
		DragonFB1,
		DragonFB2,
		DragonFBE,
		DragonWolk,
		Down,
		Roll,
		Jump,
		Lund,
		Max
	};
public:
	AudioAll();
	~AudioAll();

	// インスタンス取得
	static AudioAll& Instance();
	AudioSource* GetMusic(int index) { return audio[index].get(); };

	//Sprite* GetSprite(int index) { return efTex[index]; };
private:

	std::unique_ptr<AudioSource> audio[static_cast<int>(AudioMusic::Max)];

	//Sprite* efTex[static_cast<int>(EfTexAll::Max)];

};