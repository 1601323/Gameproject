#pragma once
#include <map>
#include "GameMain.h"

class SoundMgr
{
private:
	SoundMgr();
	SoundMgr(const SoundMgr&);

	std::map<std::string, int> soundID;				//サウンドのハンドル
	std::map<std::string, SCENE_TYPE> delSound;		//ハンドルを消すタイミング
public:
	~SoundMgr();
	static SoundMgr& Instance();
	void SoundManager(const SCENE_TYPE delScene);

	bool SoundPlayCheck(int soundHandle);
	int SoundIdReturn(const std::string& path, const SCENE_TYPE delScene);
	void SoundIdAllDelete();

};

