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
//あたり判定です。GetRectを持っている奴らなら呼び出して使えるはず
//例
//　HitClass* _hit = new HitClass();
// _hit->IsHit(GetRect(),判定したい相手->GetRect());
bool HitClass::IsHit(Rect& rcA, Rect& rcB) 
{
	return !(rcA.Right() <rcB.Left()  ||
			 rcB.Right() <rcA.Left() ||
		   	 rcA.Bottom()<rcB.Top()  ||
			 rcB.Bottom()<rcA.Top());
}
//あたり判定　Rectと点Position2
bool HitClass::IsHit(Rect& rc, Position2& pos)
{
	return !(rc.Right() < pos.x ||
			 rc.Left()  > pos.x ||
			 rc.Top()   > pos.y ||
			 rc.Bottom()< pos.y);
}
//あたり判定　円と円
bool HitClass::IsHit(Circle& cirA,Circle& cirB)
{
	if (pow((cirA.pos.x - cirB.pos.x), 2) + pow((cirA.pos.y - cirB.pos.y), 2) <= pow((cirA.r + cirB.r), 2)) {
		return true;
	}
	return false;
}
//あたり判定　Rectと円
bool HitClass::IsHit(Rect& rc, Circle& cir)
{
	Position2 verpos[4];	//それぞれの頂点を代入しておく
	//中心点が矩形内であれば絶対trueをかえす
	if (rc.Left()  < cir.pos.x && cir.pos.x < rc.Right() &&
		rc.Top()   < cir.pos.y && cir.pos.y < rc.Bottom()) {
		return true;
	}
	//円が矩形内に入っていたらtrueを返す
	if (rc.Right() > (cir.pos.x - cir.r) && rc.Left() < (cir.pos.x + cir.r) &&
		rc.Top() < (cir.pos.y + cir.r) && rc.Bottom() > (cir.pos.y - cir.r)) {
		//このままであれば矩形同士のあたり判定と変わらなくなるので判定を行う
		verpos[0] = { rc.Left(),rc.Top() };		//左上
		verpos[1] = { rc.Right(),rc.Top() };	//右上
		verpos[2] = { rc.Left(), rc.Bottom() };	//左下
		verpos[3] = { rc.Right(),rc.Bottom() };	//右下
		for (int f = 0; f < 4; f++) {
			if (pow((cir.pos.x - verpos[f].x), 2) + pow((cir.pos.y - verpos[f].y), 2) <= pow(cir.r,2)) {
				return true;
			}
		}
		//左右の辺とぶつかっているとき
		if (rc.Top()<cir.pos.y && cir.pos.y<rc.Bottom() && (rc.Right() >(cir.pos.x - cir.r) || rc.Left() < (cir.pos.x + cir.r))) {
			return true;
		}
		//上下の辺とぶつかっているとき
		if (rc.Right() > cir.pos.x && rc.Left() < cir.pos.x && (rc.Top() < (cir.pos.y + cir.r) || rc.Bottom() > (cir.pos.y - cir.r))) {
			return true;
		}
		return false;
	}
	return false;
}
//あたり判定　円と点
bool HitClass::IsHit(Circle& cir,Position2& pos) 
{
	if (pow((cir.pos.x - pos.x), 2) + pow((cir.pos.y - pos.y), 2) <= pow(cir.r, 2)) {
		return true;
	}
	return false;
}
//ギミックとRectとのあたり判定
bool HitClass::GimmickHit(Object& act)
{
	//通常のあたり判定
	for (auto& gimmick : _fac->GimmickList()) {
		if (IsHit( gimmick->GetRect(),act.GetRect())) {
			return true;
		}
	}
	return false;
}
//GimmickとCircleのあたり判定
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
//仮としてドアに入るときのあたり判定を記述
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
//敵とのあたり判定
bool HitClass::EnemyHit(Object& act)
{
	for (auto& enemy : _emFac->EnemyList()) {
			//怯み状態であれば当たっていない判定（ﾌﾟﾚｲﾔｰに対してだけなので調整の必要あり）
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
//敵の視界について
//ENEMY_DATAで必要な引数を入れてください
//　lookAngle	視野角を指定。
//	lookDir		減殺向いている方向
//	lookRange	視界の広さ。いわゆる目の位置。Circleなどでeyeを作っていれてください
bool HitClass::EnemyViewing(ENEMY_DATA& em, Rect&rc)
{
	float upAngle = AngleRad(360.f - (em.lookAngle *0.5f));		//上方向の角度
	float downAngle = AngleRad((em.lookAngle *0.5));			//下方向の角度
	//視野角の円周に入っていたら判定を始める
	if (IsHit(rc, em.lookRange)) {
		if (em.lookDir == DIR_RIGHT) {
			if (rc.Right() < em.lookRange.pos.x) {
				return false;			//右を向いているとき、判定相手が中心より左にいたら判定しない
			}
			upAngle = AngleRad(360.f - (em.lookAngle*0.5f));		//上方向の角度
			downAngle = AngleRad((em.lookAngle *0.5f));			//下方向の角度
		}
		else if (em.lookDir == DIR_LEFT) {
			if (rc.Left() > em.lookRange.pos.x) {
				return false;		//左を向いているとき、判定相手が中心より右にいたら判定しない
			}
			upAngle   = AngleRad(180.f + (em.lookAngle *0.5f));		//上方向の角度
			downAngle = AngleRad(180.f - (em.lookAngle *0.5f));			//下方向の角度
		}
		else {
			ASSERT();
		}
		Vector2 vector[2];
		Position2 intersection[2];
		//上角度について
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
		//下角度について
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
		//もし視界よりも大きいものを判定するとき
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

//gimmickの種類を返すために暫定的に用意しています
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