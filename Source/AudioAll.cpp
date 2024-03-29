#include "AudioAll.h"
#include "Audio.h"
// インスタンス取得
static AudioAll* instance = nullptr;

// インスタンス取得
AudioAll& AudioAll::Instance()
{
	return *instance;

}
AudioAll::AudioAll() {
	instance = this;
	audio[static_cast<int>(AudioMusic::Title)] = Audio::Instance().LoadAudioSource("resources\\Audio\\Title.wav");
	audio[static_cast<int>(AudioMusic::Bgm)] = Audio::Instance().LoadAudioSource("resources\\Audio\\Game.wav");
	audio[static_cast<int>(AudioMusic::Ketei)] = Audio::Instance().LoadAudioSource("resources\\Audio\\ketteionn.wav");
	audio[static_cast<int>(AudioMusic::Wolk)] = Audio::Instance().LoadAudioSource("resources\\Audio\\wolk2.wav");
	audio[static_cast<int>(AudioMusic::Run)] = Audio::Instance().LoadAudioSource("resources\\Audio\\run3.wav");
	audio[static_cast<int>(AudioMusic::Slash1)] = Audio::Instance().LoadAudioSource("resources\\Audio\\slah1.wav");
	audio[static_cast<int>(AudioMusic::Slash2)] = Audio::Instance().LoadAudioSource("resources\\Audio\\slah2.wav");
	audio[static_cast<int>(AudioMusic::Swing1)] = Audio::Instance().LoadAudioSource("resources\\Audio\\suing.wav");
	audio[static_cast<int>(AudioMusic::boom1)] = Audio::Instance().LoadAudioSource("resources\\Audio\\boom1.wav");
	audio[static_cast<int>(AudioMusic::boom2)] = Audio::Instance().LoadAudioSource("resources\\Audio\\boom2.wav");
	audio[static_cast<int>(AudioMusic::SlashFly)] = Audio::Instance().LoadAudioSource("resources\\Audio\\hitslash.wav");
	audio[static_cast<int>(AudioMusic::Rock)] = Audio::Instance().LoadAudioSource("resources\\Audio\\iwa.wav");
	audio[static_cast<int>(AudioMusic::Break)] = Audio::Instance().LoadAudioSource("resources\\Audio\\break.wav");
	audio[static_cast<int>(AudioMusic::Hit)] = Audio::Instance().LoadAudioSource("resources\\Audio\\pannti.wav");
	audio[static_cast<int>(AudioMusic::DragonFBE)] = Audio::Instance().LoadAudioSource("resources\\Audio\\flram1.wav");
	audio[static_cast<int>(AudioMusic::Down)] = Audio::Instance().LoadAudioSource("resources\\Audio\\dle.wav");
	audio[static_cast<int>(AudioMusic::Roll)] = Audio::Instance().LoadAudioSource("resources\\Audio\\roll.wav");
	audio[static_cast<int>(AudioMusic::Jump)] = Audio::Instance().LoadAudioSource("resources\\Audio\\jump.wav");
	audio[static_cast<int>(AudioMusic::Lund)] = Audio::Instance().LoadAudioSource("resources\\Audio\\chakuchi.wav");
	audio[static_cast<int>(AudioMusic::Clear)] = Audio::Instance().LoadAudioSource("resources\\Audio\\Clear.wav");
	audio[static_cast<int>(AudioMusic::Lose)] = Audio::Instance().LoadAudioSource("resources\\Audio\\Lose.wav");
	audio[static_cast<int>(AudioMusic::Hologram)] = Audio::Instance().LoadAudioSource("resources\\Audio\\Hologram.wav");
	audio[static_cast<int>(AudioMusic::Shot)] = Audio::Instance().LoadAudioSource("resources\\Audio\\Shot.wav");
	audio[static_cast<int>(AudioMusic::Establishment)] = Audio::Instance().LoadAudioSource("resources\\Audio\\Establishment.wav");
	audio[static_cast<int>(AudioMusic::Text)] = Audio::Instance().LoadAudioSource("resources\\Audio\\Text.wav");

	

	//efTex[static_cast<int>(EfTexAll::Dfelat)] = new Sprite(L".\\resources\\Effect\\Texture\\dummy.png");
	//efTex[static_cast<int>(EfTexAll::BlackThunder)] = new Sprite(L".\\resources\\Textures\\black.png");
	//efTex[static_cast<int>(EfTexAll::Thunder)] = new Sprite(L".\\resources\\Textures\\tex.jpg");
	//efTex[static_cast<int>(EfTexAll::Sumi)] = new Sprite(L".\\resources\\Textures\\sumi.png");
	//efTex[static_cast<int>(EfTexAll::Bock)] = new Sprite(L".\\resources\\Textures\\honn.png");
	//efTex[static_cast<int>(EfTexAll::Distortion)] = new Sprite(L".\\resources\\Textures\\sampke_23.jpg");
	//efTex[static_cast<int>(EfTexAll::Impact)] = new Sprite(L".\\resources\\Effect\\Texture\\Shock_wave1.png");
	//efTex[static_cast<int>(EfTexAll::Flame)] = new Sprite(L".\\resources\\Effect\\Texture\\sampke_23.jpg");
	//efTex[static_cast<int>(EfTexAll::BlueFlame)] = new Sprite(L".\\resources\\Effect\\Texture\\burFl.jpg");
	//efTex[static_cast<int>(EfTexAll::Vortex)] = new Sprite(L".\\resources\\Effect\\Texture\\uzu.jpg");
	//efTex[static_cast<int>(EfTexAll::BlueThader)] = new Sprite(L".\\resources\\Effect\\Texture\\burThader.jpg");



}
AudioAll::~AudioAll() {
	//for (int i = 0; i < static_cast<int>(EfTexAll::Max); i++) {
	//	delete efTex[i];
	//}
}