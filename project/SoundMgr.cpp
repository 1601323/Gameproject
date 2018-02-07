#include <DxLib.h>
#include<iostream>
#include <stdio.h>
#include <string>
#include <map>
#include "Assert.h"
#include "SoundMgr.h"



SoundMgr::SoundMgr()
{
}


SoundMgr::~SoundMgr()
{
}
SoundMgr& SoundMgr::Instance()
{
	static SoundMgr soundMgr;
	return soundMgr;
}

int SoundMgr::SoundIdReturn(const std::string& path,const SCENE_TYPE delScene)
{
	if (soundID.count(path) == 0) {
		soundID[path] = LoadSoundMem(path.c_str());
	}
	delSound[path] = delScene;

	return soundID[path];
}

void SoundMgr::SoundManager(const SCENE_TYPE delScene)
{
	auto itr = delSound.begin();

	while (itr != delSound.end()) {
		if (itr->second == delScene) {
			DeleteSoundMem(soundID[itr->first]);
			itr = delSound.erase(itr);
		}
		else {
			itr++;
		}
	}
}

bool SoundMgr::SoundPlayCheck(int soundHandle)
{
	int ret = CheckSoundMem(soundHandle);
	bool soundPlayFlag = true;
	switch (ret) {
	case 1:
		soundPlayFlag = false;
		break;
	case 0:
		soundPlayFlag = true;
		break;
	case -1:
		ASSERT();
		break;
	default:
		ASSERT();
		break;
	}
	return soundPlayFlag;
}

void SoundMgr::SoundIdAllDelete()
{
	InitSoundMem();
	for (auto itr = soundID.begin(); itr != soundID.end(); itr++) {
		DeleteSoundMem(itr->second);
	}
	soundID.erase(soundID.begin(),soundID.end());
	delSound.erase(delSound.begin(),delSound.end());
}

//再生関連
//SEを再生したいとき
void SoundMgr::SeStart(const std::string path, const SCENE_TYPE delScene)
{
	if (CheckSoundMem(SoundIdReturn(path, delScene)) == 1) {
		StopSoundMem(SoundIdReturn(path, delScene));
	}
	else if (CheckSoundMem(SoundIdReturn(path, delScene)) == 0) {
		PlaySoundMem(SoundIdReturn(path,delScene),DX_PLAYTYPE_BACK,true);
	}
	else {
	//	ASSERT();
	}
}
//BGMの再生開始用
void SoundMgr::BgmStart(const std::string file, const SCENE_TYPE delScene)
{
	int bgm = SoundIdReturn(file,delScene);
	if (CheckSoundMem(bgm) == 1) {
		StopSoundMem(bgm);
	}
	else if (CheckSoundMem(bgm) == 0) {
		PlaySoundMem(bgm,DX_PLAYTYPE_LOOP,true);
	}
	else {
		//ASSERT();
	}
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP, true);

	//音量の調節
	ChangeVolumeSoundMem(120, bgm);
	fadeCnt = 0;
}
//BGMをフェードアウトさせます
void SoundMgr::BgmFadeOut(const std::string file, const SCENE_TYPE delScene)
{
	int bgm = SoundIdReturn(file,delScene);
	if (fadeCnt >= 120) {
		fadeCnt = 120;
	}
	if (CheckSoundMem(bgm) == 1) {
		fadeCnt += 1;
		if (120 - fadeCnt <= 0) {
			StopSoundMem(bgm);
		}
		ChangeVolumeSoundMem(120-fadeCnt,bgm);
	}
}
//BGMを止めます
void SoundMgr::BgmStop(const std::string file, const SCENE_TYPE delScene)
{
	StopSoundMem(SoundIdReturn(file,delScene));
}