#pragma once
#include <list>
#include "Object.h"
#include "Input.h"

#define ROPE_SPEED (15)          //�ړ��X�s�[�h
#define ROPE_THETA (45.f)        //���[�v�̎΂߂̊p�x�E����ver
#define ROPE_THETA2 (135.f)      //���[�v�̎΂߂̊p�x������ver
#define STRAIGHT_RAD (180.f)     //��������Rad
#define ROPE_RANGE (5.f)         //circle�p�̔��a
#define ROPE_RECT_W (20)         //rect�p�̕�
#define ROPE_RECT_H (20)         //rect�p�̍���
#define ROPE_OMEGA (2.f)         //�����������邱�Ƃ�1��œ���rect circle��pos��ς���܂�
#define ROPE_LENGTH_MAX (20)     //���[�v�̍ő咷
#define WAIT_TIME (0)           //�L�т�������̎�������

class Input;
class Player;
class HitClass;
class MapCtl;
class ModelMgr;

struct Rect;
struct Circle;
struct INPUT_INFO;
struct L_STICK;
struct R_STICK;

enum SENSING_VALUE;

union KEY;

enum ROPE_DIR
{
	ROPE_DIR_NON,        //�L�΂��Ă��Ȃ����
	ROPE_DIR_UPPER,      //�΂ߏ�ɐL�΂��Ă�����
	ROPE_DIR_STRAIGHT,   //�܂������L�΂��Ă�����
	ROPE_DIR_LOWER,      //�΂߉��ɐL�΂��Ă�����
};

enum ROPE_CURVE_DIR
{
	ROPE_DIR_CURVE_NON,  //�L�΂��Ă��Ȃ����
	ROPE_DIR_CURVE_UP,   //�΂ߏ�ɋȐ���`���Ȃ���L�΂��Ă�����
	ROPE_DIR_CURVE_LOW   //�΂߉��ɋȐ���`���Ȃ���L�΂��Ă�����
};

class Rope : public Object
{
private:

	ModelMgr* _modelmgr;

	Position2 RotationPos;    //�������ɂ��邭���pos
	bool RopeTurnFlag;
	bool dirFlag;
	bool padFlag;
	int time;

	int modelhandle;
	float  AnimTotalTime;
	float  AnimNowTime;
	int    AnimAttachIndex;

	float range;
	float theta;
	float omega;
	float rote;

	void RopeInit(void);
	void DrawRopeRect(void);
	void SetRope(void);
	void SetCurveRope(void);
	void DirChange(void);
	void ThetaSet(void);
	void CheckPad(void);
	void DrawLineSet(Position2 startpos,Position2 endpos,int color);
	Position2 SetVec(void);

	float SetRopeRad(void);
	float SetRopeRadCurve(void);

public:
	Rope();
	Rope(Player* player);
	~Rope();

	std::list<int> ropeinfo;
	std::list<int>::iterator itr = ropeinfo.begin();
	std::list<int>::iterator last = ropeinfo.end();

	Position2 _rope[ROPE_LENGTH_MAX];

	ROPE_STATE _state;
	ROPE_DIR _ropeDir;
	ROPE_CURVE_DIR _curveDir;
	SENSING_VALUE _minValue;

	KEY _key;
	KEY _lastkey;
	INPUT_INFO _inputinfo;
	Rect _RopeRect;
	Circle _RopeCircle;
	Position2 _HitPos;
	Position2 _vec;
	Position2 _tmpOffset;			//�`��p�̾�Ēl

	Player* _player;
	MapCtl* _mapctl;
	HitClass* _hit;

	void Updata(Input* input,Position2 offset);
	
	void(Rope::*_states)(Input* input);
	void Ready(Input* input);
	void SelectDir(Input* input);
	void Extending(Input* input);
	void Extended(Input* input);
	void Shrinking(Input* input);
	void GetClass(HitClass* h);		//�N���X���󂯎�邽�߂̊֐�

	ROPE_DIR RopeDirSet(void);
	ROPE_DIR RopeDirSetPad(Input* input);
	ROPE_CURVE_DIR CurveRopeDirSet(void);
	Rect& GetRect(void);
	Circle& GetCircle(void);
	ROPE_STATE GetRopeState(void);
	Position2 GetReturnPos(void);
	Position2 GetRopeVec(void);

};
