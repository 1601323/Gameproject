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

	static bool IsHit(Rect& rcA, Rect& rcB);		//Rect同士のあたり判定
	static bool IsHit(Rect& rc,Position2& pos);		//Rectと点のあたり判定
	static bool IsHit(Circle& cirA,Circle& cirB);	//円と円のあたり判定
	static bool IsHit(Rect& rc,Circle& cir);		//Rectと円のあたり判定
	static bool IsHit(Circle& cir, Position2& pos);	//円と点のあたり判定
	bool GimmickHit(Object& act);					//GimmickとRectのあたり判定
	bool GimmickHit(Circle& cir);					//GimmickとCircleのあたり判定
	bool GimmickHit(Position2& pos);				//gimmickとposのあたり判定
	bool EnemyHit(Object& act);						//EnemyとRectのあたり判定
	bool EnemyHit(Circle& cir);						//EnemyとCircleとのあたり判定
	bool EnemyHit(Position2& pos);					//Enemyとposとのあたり判定

	bool TutorialHit(Object& act);                  //tutorialとRectのあたり判定

	bool GimmickEnter(Object& act);

	//敵の視野角のあたり判定について
	bool EnemyViewing(ENEMY_DATA& em,Rect& rc);
	//とりあえず当たっているギミックの種類を返します
	GIMMICK_TYPE GimmickHitType(Position2& pos);
	GIMMICK_TYPE GimmickHitType(Object& act);
	GIMMICK_TYPE GimmickHitType(Circle& cir);
};

