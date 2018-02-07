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

	std::string tmpName;
	SCENE_TYPE tmpScene;
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
	void BgmStop(const std::string file,const SCENE_TYPE delScene);
	void ChangeSound();						//音量を平常に戻します
	void ChangeSound(int volume);			//音量を決められた値にします
	void BgmStop();

	void LastInputBgm(std::string file, SCENE_TYPE delScene);			//最後に読み込んだBGMを記憶させます
};

