#pragma once
#include "Input.h"

class Player;
class Input;
class HitClass;


class Tutorial
{
public:
	Tutorial();
	~Tutorial();
	void Updata();
	void Draw();
private:
	Player* _player;
	HitClass* _hit;

};

