#pragma once
#include <list>
#include "Object.h"
#include "Input.h"

#define ROPE_SPEED (6.5f)        //�ړ��X�s�[�h
#define ROPE_THETA (45.f)        //���[�v�̎΂߂̊p�x�E����ver(circle�̂�)
#define ROPE_THETA2 (135.f)      //���[�v�̎΂߂̊p�x������ver(circle�̂�)
#define STRAIGHT_RAD (180.f)     //��������Rad
#define ROPE_RANGE (2.f)         //circle�p�̔��a
#define ROPE_RECT_W (20)         //rect�p�̕�
#define ROPE_RECT_H (20)         //rect�p�̍���
#define ROPE_OMEGA (1.0f)        //�����������邱�Ƃ�1��œ���rect circle��pos��ς���܂�
#define ROPE_LENGTH_MAX (45)     //���[�v�̍ő咷 
#define ROPE_MODEL_NUM (450)     //���[�v�̃��f���̍��W�𒲐����邽�ߒl
#define WAIT_TIMER (30)          //���[�v���B��̑҂�����

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

	bool RopeTurnFlag;                      //���[�v�����Ԃ��pflag
	bool dirFlag;                           //���[�v�̌��������߂�flag
	bool padFlag;                           //�Q�[���p�b�h���h�����Ă��邩�ǂ���������flag
	bool tongueHitTurn;                     //�オ�I�u�W�F�N�g�ɓ����������ǂ���������flag

	int    modelhandle;                     //���[�v�̃��f���n���h��
	float  AnimTotalTime;                   //�A�j���[�V�����̃g�[�^���^�C��
	float  AnimNowTime;                     //�A�j���[�V�����̌��݃^�C��
	int    AnimAttachIndex;                 //�A�j���[�V������
	int timerWait;                          //�҂�����

	int mentenanceNum_X;                    //���f���̕`��̂��߂ɉ��Œu���Ă��钲���p�̒lX
	int mentenanceNum_Y;                    //���f���̕`��̂��߂ɉ��Œu���Ă��钲���p�̒lY
	int RopeHitModelNumY;

	float range;                            //�T�[�N�����a
	float RopeAngle_Y;                      //�\���p���f���p�xY
	float RopeAngle_Z;                      //�\���p���f���p�xZ

	void RopeInit(void);                    //�����֐�
	void DrawRopeRect(void);                //�`�揈��
	void SetRope(void);                     //���[�v�p
	void DirChange(void);                   //������ς���֐�
	void CheckPad(void);                    //�Q�[���p�b�h�`�F�b�N�֐�
	Position2 SetVec(void);                 //�X�s�[�h�ݒ�֐�
	float SetRopeRad(void);                 //���[�v�̔��ˊp�x�����߂�֐�
	void SetRopeRadForDrawZ(void);          //���f���\���̂��߂̒����֐�
	void ModelManager(void);                //���f���֌W�̊֐�

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
	Circle _RopeCircle2;
	Circle _RopeCircle3;
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
