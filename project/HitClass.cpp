#include <DxLib.h>
#include <iostream>
#include <algorithm>
#include <math.h>

#include "Assert.h"
#include "Geometry.h"
#include "GimmickFactory.h"
#include "EnemyFactory.h"
#include "Object.h"
#include "Gimmick.h"
#include "Enemy.h"
#include "HitClass.h"
#include "SensorDoor.h"
#include "Math.h"
HitClass::HitClass(GimmickFactory* f,EnemyFactory* ef)
{
	_fac = f;
	_emFac = ef;
}

HitClass::HitClass() 
{

}
HitClass::~HitClass()
{
}
void HitClass::GetClass(GimmickFactory* f)
{
	_fac = f;
}
void HitClass::GetClass(EnemyFactory* ef)
{
	_emFac = ef;
}
//�����蔻��ł��BGetRect�������Ă���z��Ȃ�Ăяo���Ďg����͂�
//��
//�@HitClass* _hit = new HitClass();
// _hit->IsHit(GetRect(),���肵��������->GetRect());
bool HitClass::IsHit(Rect& rcA, Rect& rcB) 
{
	return !(rcA.Right() <rcB.Left()  ||
			 rcB.Right() <rcA.Left() ||
		   	 rcA.Bottom()<rcB.Top()  ||
			 rcB.Bottom()<rcA.Top());
}
//�����蔻��@Rect�Ɠ_Position2
bool HitClass::IsHit(Rect& rc, Position2& pos)
{
	return !(rc.Right() < pos.x ||
			 rc.Left()  > pos.x ||
			 rc.Top()   > pos.y ||
			 rc.Bottom()< pos.y);
}
//�����蔻��@�~�Ɖ~
bool HitClass::IsHit(Circle& cirA,Circle& cirB)
{
	if (pow((cirA.pos.x - cirB.pos.x), 2) + pow((cirA.pos.y - cirB.pos.y), 2) <= pow((cirA.r + cirB.r), 2)) {
		return true;
	}
	return false;
}
//�����蔻��@Rect�Ɖ~
bool HitClass::IsHit(Rect& rc, Circle& cir)
{
	Position2 verpos[4];	//���ꂼ��̒��_�������Ă���
	//���S�_����`���ł���ΐ��true��������
	if (rc.Left()  < cir.pos.x && cir.pos.x < rc.Right() &&
		rc.Top()   < cir.pos.y && cir.pos.y < rc.Bottom()) {
		return true;
	}
	//�~����`���ɓ����Ă�����true��Ԃ�
	if (rc.Right() > (cir.pos.x - cir.r) && rc.Left() < (cir.pos.x + cir.r) &&
		rc.Top() < (cir.pos.y + cir.r) && rc.Bottom() > (cir.pos.y - cir.r)) {
		//���̂܂܂ł���΋�`���m�̂����蔻��ƕς��Ȃ��Ȃ�̂Ŕ�����s��
		verpos[0] = { rc.Left(),rc.Top() };		//����
		verpos[1] = { rc.Right(),rc.Top() };	//�E��
		verpos[2] = { rc.Left(), rc.Bottom() };	//����
		verpos[3] = { rc.Right(),rc.Bottom() };	//�E��
		for (int f = 0; f < 4; f++) {
			if (pow((cir.pos.x - verpos[f].x), 2) + pow((cir.pos.y - verpos[f].y), 2) <= pow(cir.r,2)) {
				return true;
			}
		}
		//���E�̕ӂƂԂ����Ă���Ƃ�
		if (rc.Top()<cir.pos.y && cir.pos.y<rc.Bottom() && (rc.Right() >(cir.pos.x - cir.r) || rc.Left() < (cir.pos.x + cir.r))) {
			return true;
		}
		//�㉺�̕ӂƂԂ����Ă���Ƃ�
		if (rc.Right() > cir.pos.x && rc.Left() < cir.pos.x && (rc.Top() < (cir.pos.y + cir.r) || rc.Bottom() > (cir.pos.y - cir.r))) {
			return true;
		}
		return false;
	}
	return false;
}
//�����蔻��@�~�Ɠ_
bool HitClass::IsHit(Circle& cir,Position2& pos) 
{
	if (pow((cir.pos.x - pos.x), 2) + pow((cir.pos.y - pos.y), 2) <= pow(cir.r, 2)) {
		return true;
	}
	return false;
}
//�M�~�b�N��Rect�Ƃ̂����蔻��
bool HitClass::GimmickHit(Object& act)
{
	//�ʏ�̂����蔻��
	for (auto& gimmick : _fac->GimmickList()) {
		if (IsHit( gimmick->GetRect(),act.GetRect())) {
			return true;
		}
	}
	return false;
}
//Gimmick��Circle�̂����蔻��
bool HitClass::GimmickHit(Circle& cir)
{
	for (auto& gimmick : _fac->GimmickList()) {
		if (IsHit(gimmick->GetRect(), cir)) {
			if (gimmick->GetType() == GIM_DOOR) {
				return false;
			}
			return true;
		}
	}
	return false;
}
bool HitClass::GimmickHit(Position2& pos)
{
	for (auto& gimmick : _fac->GimmickList()) {
		if (IsHit(gimmick->GetRect(), pos)) {
			return true;
		}
	}
	return false;
}
//���Ƃ��ăh�A�ɓ���Ƃ��̂����蔻����L�q
bool HitClass::GimmickEnter(Object& act)
{
	for (auto& gimmick : _fac->GimmickList()) {
		if (IsHit(gimmick->GetRect(), act.GetRect())) {
			if (gimmick->GetClearData().midFlag == true) {
				if (gimmick->GetRect().Left() <= act.GetRect().Left()
				&&  act.GetRect().Right() <=gimmick->GetRect().Right())
				return true;
			}
		}
	}
	return false;
}
//�G�Ƃ̂����蔻��
bool HitClass::EnemyHit(Object& act)
{
	for (auto& enemy : _emFac->EnemyList()) {
			//���ݏ�Ԃł���Γ������Ă��Ȃ�����i��ڲ԰�ɑ΂��Ă����Ȃ̂Œ����̕K�v����j
		if (enemy->GetState() == EM_ST_FEAR) {
			return false;
		}
		if (IsHit(enemy->GetRect(), act.GetRect())) {
			if (enemy->GetDir() == DIR_LEFT) {
				if (enemy->GetRect().Left() > act.GetRect().Left()) {
					return true;
				}
				return false;
			}
			else if (enemy->GetDir() == DIR_RIGHT) {
				if (enemy->GetRect().Right() < act.GetRect().Right()) {
					return true;
				}
				return false;
			}
			//return true;
		}
	}
	return false;
}
bool HitClass::EnemyHit(Circle& cir)
{
	for (auto& enemy : _emFac->EnemyList()) {
		if (IsHit(enemy->GetRect(), cir)) {
			return true;
		}
	}
	return false;
}
bool HitClass::EnemyHit(Position2& pos)
{
	for (auto& enemy : _emFac->EnemyList()) {
		if (IsHit(enemy->GetRect(), pos)) {
			return true;
		}
	}
	return false;
}
//�G�̎��E�ɂ���
//ENEMY_DATA�ŕK�v�Ȉ��������Ă�������
//�@lookAngle	����p���w��B
//	lookDir		���E�����Ă������
//	lookRange	���E�̍L���B������ڂ̈ʒu�BCircle�Ȃǂ�eye������Ă���Ă�������
bool HitClass::EnemyViewing(ENEMY_DATA& em, Rect&rc)
{
	float upAngle = AngleRad(360.f - (em.lookAngle *0.5f));		//������̊p�x
	float downAngle = AngleRad((em.lookAngle *0.5));			//�������̊p�x
	//����p�̉~���ɓ����Ă����画����n�߂�
	if (IsHit(rc, em.lookRange)) {
		if (em.lookDir == DIR_RIGHT) {
			if (rc.Right() < em.lookRange.pos.x) {
				return false;			//�E�������Ă���Ƃ��A���葊�肪���S��荶�ɂ����画�肵�Ȃ�
			}
			upAngle = AngleRad(360.f - (em.lookAngle*0.5f));		//������̊p�x
			downAngle = AngleRad((em.lookAngle *0.5f));			//�������̊p�x
		}
		else if (em.lookDir == DIR_LEFT) {
			if (rc.Left() > em.lookRange.pos.x) {
				return false;		//���������Ă���Ƃ��A���葊�肪���S���E�ɂ����画�肵�Ȃ�
			}
			upAngle   = AngleRad(180.f + (em.lookAngle *0.5f));		//������̊p�x
			downAngle = AngleRad(180.f - (em.lookAngle *0.5f));			//�������̊p�x
		}
		else {
			ASSERT();
		}
		Vector2 vector[2];
		Position2 intersection[2];
		//��p�x�ɂ���
		vector[0].x = em.lookRange.r*cos(upAngle);
		vector[0].y = em.lookRange.r*sin(upAngle);
		intersection[0] = em.lookRange.pos + vector[0];
		if (rc.Bottom() < em.lookRange.pos.y && intersection[0].y < rc.Bottom() ||
			rc.Top()	< em.lookRange.pos.y && intersection[0].y < rc.Top()) {
			Position2 p ;
			if (em.lookDir == DIR_RIGHT) {
				p = { rc.Right(),rc.Bottom() };
			}
			else if(em.lookDir==DIR_LEFT){
				p = { rc.Left(),rc.Bottom() };
			}
			else {
				ASSERT();
			}
			Vector2 ap = {p.x -em.lookRange.pos.x, p.y-em.lookRange.pos.y};
			Vector2 ab = {intersection[0].x - em.lookRange.pos.x, intersection[0].y - em.lookRange.pos.y};
			float cross = ap.x* ab.y - ab.x*ap.y;
			float result = cross / (sqrt(ab.x*ab.x + ab.y*ab.y)*sqrt(ap.x*ap.x + ap.y*ap.y));
			if (em.lookDir == DIR_RIGHT) {
				if (result <= 0) {
					return true;
				}
			}
			else if (em.lookDir == DIR_LEFT) {
				if (result >= 0) {
					return true;
				}
			}
			else {
				ASSERT();
			}
		}
		//���p�x�ɂ���
		vector[1].x = em.lookRange.r*cos(downAngle);
		vector[1].y = em.lookRange.r*sin(downAngle);
		intersection[1] = em.lookRange.pos + vector[1];
		if (rc.Bottom() < intersection[1].y &&em.lookRange.pos.y < rc.Bottom() ||
			rc.Top()	< intersection[1].y &&em.lookRange.pos.y < rc.Top()) {
			Position2 p ;
			if (em.lookDir == DIR_RIGHT) {
				p = { rc.Right(),rc.Top() };
			}
			else if(em.lookDir == DIR_LEFT){
				p = { rc.Left(),rc.Top() };
			}
			else {
				ASSERT();
			}
			Vector2 ap = { p.x - em.lookRange.pos.x, p.y - em.lookRange.pos.y };
			Vector2 ab = { intersection[1].x - em.lookRange.pos.x, intersection[1].y - em.lookRange.pos.y };
			float cross = ap.x* ab.y - ab.x*ap.y;
			float result = cross / (sqrt(ab.x*ab.x + ab.y*ab.y)*sqrt(ap.x*ap.x + ap.y*ap.y));
			if (em.lookDir == DIR_RIGHT) {
				if (result >= 0) {
					return true;
				}
			}
			else if (em.lookDir == DIR_LEFT) {
				if (result <= 0) {
					return true;
				}
			}
			else {
				ASSERT();
			}
		}
		//�������E�����傫�����̂𔻒肷��Ƃ�
		if (rc.Left() < em.lookRange.pos.x && em.lookRange.pos.x < rc.Right() &&
			rc.Top()  < em.lookRange.pos.y && em.lookRange.pos.y < rc.Bottom() ||
			rc.Left() < intersection[0].x  && intersection[0].x < rc.Right()  &&
			rc.Top()  < intersection[0].y  && intersection[0].y < rc.Bottom()  ||
			rc.Left() < intersection[1].x  && intersection[1].x < rc.Right()  &&
			rc.Top()  < intersection[1].y  && intersection[1].y < rc.Bottom()	) {
			return true;
		}
	}
	return false;
}

//gimmick�̎�ނ�Ԃ����߂Ɏb��I�ɗp�ӂ��Ă��܂�
GIMMICK_TYPE HitClass :: GimmickHitType(Position2& pos)
{
	for (auto& gimmick : _fac->GimmickList()) {
		if (IsHit(gimmick->GetRect(), pos)) {
			return gimmick->GetType();
		}
	}
	return GIM_NONE;
}
GIMMICK_TYPE HitClass::GimmickHitType(Object& act)
{
	for (auto& gimmick : _fac->GimmickList()) {
		if (IsHit(gimmick->GetRect(), act.GetRect())) {
			return gimmick->GetType();
		}
	}
	return GIM_NONE;
}
GIMMICK_TYPE HitClass::GimmickHitType(Circle& cir)
{
	for (auto& gimmick : _fac->GimmickList()) {
		if (IsHit(gimmick->GetRect(), cir)) {
			return  gimmick->GetType();
		}
	}
	return GIM_NONE;
}