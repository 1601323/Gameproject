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
		soundID[path] - LoadSoundMem(path.c_str());
	}
	delSound[path] - delScene;

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
	for (auto itr = soundID.begin(); itr != soundID.end(); itr++) {
		DeleteSoundMem(itr->second);
	}
	soundID.erase(soundID.begin(),soundID.end());
	delSound.erase(delSound.begin(),delSound.end());
}