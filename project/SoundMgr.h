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
	int fadeCnt;
public:
	~SoundMgr();
	static SoundMgr& Instance();
	void SoundManager(const SCENE_TYPE delScene);

	bool SoundPlayCheck(int soundHandle);
	int SoundIdReturn(const std::string& path, const SCENE_TYPE delScene);
	void SoundIdAllDelete();

	void SeStart(const std::string path,const SCENE_TYPE delScene);
	void BgmStart(const std::string file, const SCENE_TYPE delScene);
	void BgmFadeOut(const std::string file, const SCENE_TYPE delScene);
};

