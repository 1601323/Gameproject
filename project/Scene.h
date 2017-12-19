#pragma once
#include "Input.h"

#include "GameMain.h"

struct KeyState;

class Scene
{
public:
	Scene();
	virtual~Scene();
	virtual void Updata(Input* input) = 0;
	virtual SCENE_TYPE GetScene() = 0;
};


