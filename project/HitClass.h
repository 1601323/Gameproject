#pragma once
#include "Geometry.h"

class GimmickFactory;
class EnemyFactory;
class Tutorial;
class Object;

class HitClass
{
private:
	GimmickFactory* _fac;
	EnemyFactory* _emFac;
	Tutorial* _tuto;
public:
	HitClass(GimmickFactory* f,EnemyFactory* ef, Tutorial* tuto);
	HitClass();
	~HitClass();

	void GetClass(GimmickFactory* f);
	void GetClass(EnemyFactory* ef);
	void GetClass(Tutorial* tuto);

	static bool IsHit(Rect& rcA, Rect& rcB);		//Rect“¯Žm‚Ì‚ ‚½‚è”»’è
	static bool IsHit(Rect& rc,Position2& pos);		//Rect‚Æ“_‚Ì‚ ‚½‚è”»’è
	static bool IsHit(Circle& cirA,Circle& cirB);	//‰~‚Æ‰~‚Ì‚ ‚½‚è”»’è
	static bool IsHit(Rect& rc,Circle& cir);		//Rect‚Æ‰~‚Ì‚ ‚½‚è”»’è
	static bool IsHit(Circle& cir, Position2& pos);	//‰~‚Æ“_‚Ì‚ ‚½‚è”»’è
	bool GimmickHit(Object& act);					//Gimmick‚ÆRect‚Ì‚ ‚½‚è”»’è
	bool GimmickHit(Circle& cir);					//Gimmick‚ÆCircle‚Ì‚ ‚½‚è”»’è
	bool GimmickHit(Position2& pos);				//gimmick‚Æpos‚Ì‚ ‚½‚è”»’è
	bool EnemyHit(Object& act);						//Enemy‚ÆRect‚Ì‚ ‚½‚è”»’è
	bool EnemyHit(Circle& cir);						//Enemy‚ÆCircle‚Æ‚Ì‚ ‚½‚è”»’è
	bool EnemyHit(Position2& pos);					//Enemy‚Æpos‚Æ‚Ì‚ ‚½‚è”»’è

	bool TutorialHit(Object& act);                  //tutorial‚ÆRect‚Ì‚ ‚½‚è”»’è

	bool GimmickEnter(Object& act);

	//“G‚ÌŽ‹–ìŠp‚Ì‚ ‚½‚è”»’è‚É‚Â‚¢‚Ä
	bool EnemyViewing(ENEMY_DATA& em,Rect& rc);
	//‚Æ‚è‚ ‚¦‚¸“–‚½‚Á‚Ä‚¢‚éƒMƒ~ƒbƒN‚ÌŽí—Þ‚ð•Ô‚µ‚Ü‚·
	GIMMICK_TYPE GimmickHitType(Position2& pos);
	GIMMICK_TYPE GimmickHitType(Object& act);
	GIMMICK_TYPE GimmickHitType(Circle& cir);
};

