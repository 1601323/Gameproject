#pragma once
#include "Object.h"
#include "Geometry.h"

class HitClass;
class Player;
class Rope;

class Enemy :
	public Object
{
private:
	HitClass* _hit;
public:
	Enemy();
	~Enemy();
	Rect _emRect;
	ENEMY_TYPE _emType;
	ENEMY_DATA _emData;
	ENEMY_STATE _state;
	EnemyServerData _commonData;
	virtual Rect&GetRect();
	virtual ENEMY_TYPE& GetType();

	virtual void Updata() =0;
	virtual void Draw(Position2 offset) =0 ;
	virtual void EnemyFalter() =0;
	virtual ENEMY_STATE& GetState();
	virtual void SetInitPos() = 0;
	ENEMY_ALERT AlertLevel();				//Œ»İ‚ÌŒx‰úƒŒƒxƒ‹‚ğ•Ô‚·
};

