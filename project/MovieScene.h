#pragma once
#include "Scene.h"
class MovieScene :
	public Scene
{
private:
	KEY key;
	KEY lastKey;
	INPUT_INFO inpInfo;
	void(MovieScene::*_updater)(Input* input);

	char* waitMovie;
	bool playFlag;
	void NormalUpdata(Input* input);
	void playMovie();
public:
	MovieScene();
	~MovieScene();
	SCENE_TYPE GetScene();
	void Updata(Input* input);
};

