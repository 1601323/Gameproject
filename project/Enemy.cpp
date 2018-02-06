#include <DxLib.h>
#include "Geometry.h"
#include "GameMain.h"
#include "ImageMgr.h"
#include "Enemy.h"
#include "Object.h"
#include "HitClass.h"
#include "Player.h"
#include "Rope.h"


Enemy::Enemy()
{
	_commonData._level = ALERT_LEVEL_1;
	_state = EM_ST_NONE;
	//vigiImage = ImageMgr::Instance().ImageIdReturn("image/view.png",SCENE_RESULT);
	LoadDivGraph("image/view3.png",5,5,1,300,300,vigiImage);
}


Enemy::~Enemy()
{
}

Rect& Enemy::GetRect()
{
	return _emRect;
}

ENEMY_TYPE& Enemy::GetType()
{
	return _emType;
}

//Œ»İ‚ÌŒx‰úƒŒƒxƒ‹‚ğ•Ô‚·
ENEMY_ALERT Enemy::AlertLevel()
{
	return _commonData._level;
}

ENEMY_STATE& Enemy::GetState()
{
	return _state;
}