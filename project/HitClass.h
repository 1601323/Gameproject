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

	static bool IsHit(Rect& rcA, Rect& rcB);		//Rect���m�̂����蔻��
	static bool IsHit(Rect& rc,Position2& pos);		//Rect�Ɠ_�̂����蔻��
	static bool IsHit(Circle& cirA,Circle& cirB);	//�~�Ɖ~�̂����蔻��
	static bool IsHit(Rect& rc,Circle& cir);		//Rect�Ɖ~�̂����蔻��
	static bool IsHit(Circle& cir, Position2& pos);	//�~�Ɠ_�̂����蔻��
	bool GimmickHit(Object& act);					//Gimmick��Rect�̂����蔻��
	bool GimmickHit(Circle& cir);					//Gimmick��Circle�̂����蔻��
	bool GimmickHit(Position2& pos);				//gimmick��pos�̂����蔻��
	bool EnemyHit(Object& act);						//Enemy��Rect�̂����蔻��
	bool EnemyHit(Circle& cir);						//Enemy��Circle�Ƃ̂����蔻��
	bool EnemyHit(Position2& pos);					//Enemy��pos�Ƃ̂����蔻��

	bool TutorialHit(Object& act);                  //tutorial��Rect�̂����蔻��

	bool GimmickEnter(Object& act);

	//�G�̎���p�̂����蔻��ɂ���
	bool EnemyViewing(ENEMY_DATA& em,Rect& rc);
	//�Ƃ肠�����������Ă���M�~�b�N�̎�ނ�Ԃ��܂�
	GIMMICK_TYPE GimmickHitType(Position2& pos);
	GIMMICK_TYPE GimmickHitType(Object& act);
	GIMMICK_TYPE GimmickHitType(Circle& cir);
};

