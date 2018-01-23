#pragma once
#include <DxLib.h>
#include <math.h>
#include <cmath>

#define SCREEN_SIZE_X (800)	// ��ʉ𑜓x�F��
#define SCREEN_SIZE_Y (640)	// ��ʉ𑜓x�F�c

#define GRAVITY 0.9f
#define MAP_CHIP_SIZE_X (32)	// 1���߂�����̑傫��
#define MAP_CHIP_SIZE_Y (32)	// 1���߂�����̑傫��
#define MAP_SIZE_X (30)			//�X�N���[����Ή��}�b�v�̑傫��28
//#define MAP_SIZE_X (28)	    // ϯ�߂̻���
#define MAP_SIZE_Y (25)			// ϯ�߂̻���
#define STAGE_MAX (3)			//�X�e�[�W�̐��i���j
#define PLAYER_MAX (3)			//��ڲ԰�̎c�@

//�v���C���[�̏�Ԃɂ���
enum CHAR_ST {
	ST_DEF,			//�ʏ�
	ST_MOVE,		//�ړ���
	ST_STOP,		//��~��
	ST_JUMP,		//�ެ���
	ST_VANISH,		//��ٽ���
	ST_ROPE,		//۰�ߎg�p
	ST_WALL,		//�Ǔo��
	ST_FEVER,		//̨��ް
	ST_DETH,		//���S
	ST_CLEAR,		//�ر
	ST_MAX
};

//�e�����߂̎��[��Я��Ƃ��Œǉ��������̂���΂����ɏ����Ă�] 
enum CHIP_TYPE {
	CHIP_BLANK,			//  0 ��
	CHIP_N_CLIMB_WALL,	//  1 �o��Ȃ���
	CHIP_CLIMB_WALL,	//  2 �o����
	CHIP_DOOR,			//  3 �h�A
	CHIP_BUTTON_1,		//  4 �{�^��
	CHIP_ROPE_ATTRACT,	//  5 ���[�v�����񂹂�
	CHIP_ROPE_FALL,		//  6 ���[�v�ߗ��Ƃ�
	CHIP_FREE_1,		//  7 ���R
	CHIP_FREE_2,		//  8 ���R
	CHIP_FREE_3,		//  9 ���R
	CHIP_FREE_4,		// 10 ���R
	CHIP_FREE_5,		// 11 ���R
	CHIP_MAX
};

// ��׸���̌���
enum DIR {
	DIR_NON,			//��ԂȂ�
	DIR_DOWN,			//��
	DIR_UP,				//��
	DIR_LEFT,			//��
	DIR_RIGHT,			//�E

	DIR_DETH = DIR_NON,	//���S
	DIR_MAX
};

//���[�v�̏��
enum ROPE_STATE
{
	ST_ROPE_READY,       //��������state
	ST_ROPE_SELECT,      //�ǂ̕����ɐL�΂��������߂�state
	ST_ROPE_EXTENDING,   //�L�΂��Ă�Ԃ�state
	ST_ROPE_EXTENDED,    //�ő�܂ŐL�т���������state
	ST_ROPE_SHRINKING,   //�k�߂Ă���Ԃ�state
};
//gimmick�̃^�C�v�ɂ���
enum GIMMICK_TYPE {
	GIM_NONE,
	GIM_DOOR,			//�h�A
	GIM_BUTTON,			//����
	GIM_ATTRACT,		//�ړ����鏰�Ȃ�
	GIM_FALL,			//������M�~�b�N�Ȃ�
};
//�G�̎�ނɂ���
enum ENEMY_TYPE {
	ENEMY_NONE,
	ENEMY_TURN,			//�U��Ԃ�G
	ENEMY_WARKING,		//�������G
	ENEMY_MIMICRY		//�[�Ԃ���G
};
//�G�̏�Ԃɂ���
enum ENEMY_STATE {
	EM_ST_NONE,		//�����Ȃ����
	EM_ST_MOVE,		//�ʏ�̓������s��
	EM_ST_ALERT,	//�x���x���オ���Ă���Ƃ�
	EM_ST_DIS,		//��ڲ԰�����������Ƃ�
	EM_ST_RETURN,	//��ڲ԰���ړI��������Ė߂��Ă���Ƃ�
	EM_ST_RE_ALERT,	//�x���x���オ���Ă���Ƃ��i�߂��Ă���Ƃ��j
	EM_ST_RE_DIS,	//��ڲ԰�����������Ƃ�(�߂��Ă���Ƃ�)
	EM_ST_FEAR		//���ݏ��
};
//�A�C�e���̎�ނɂ���
enum ITEM_TYPE {
	ITEM_FEVER,
	ITEM_MAX
};
//�G�̌x���x�ɂ���
enum ENEMY_ALERT {
	ALERT_LEVEL_1,
	ALERT_LEVEL_2,
	ALERT_LEVEL_3
};
//�v���C���[�̃A�N�V����
enum PLAYER_ACTIONS{
	ACTION_WAIT,   //�ҋ@���[�V����
	ACTION_JUMP,   //��ԃ��[�V����
	ACTION_CLIMB,  //�̂ڂ郂�[�V����
	ACTION_WALK,   //�������[�V����
	ACTION_MAX
};

struct EnemyServerData {

	EnemyServerData():plFoundFlag(false),dataSendFlag(false){}

	bool plFoundFlag;
	bool dataSendFlag;
	ENEMY_ALERT _level;
};
//�L�[���͂ɂ���
struct KeyState {
	char state[256];
	char laststate[256];
};

//���W��\��
//�Ƃ肠�����񎟌��ōl����
struct Vector2 
{
	Vector2() :x(0), y(0) {}
	Vector2(float inx, float iny) : x(inx), y(iny) {}
	float x, y;
};
//�킩��₷���|�W�V�����ɒu��������
typedef Vector2 Position2;

//�������͎O��������!
struct Vector3
{
	Vector3() :x(0), y(0) {}
	Vector3(float inx, float iny,float inz) : x(inx), y(iny), z(inz){}
	float x, y, z;
};
//�|�W�V�����ɒu��������
typedef Vector3 Position3;

//�ꏊ�̋�`�ɂ���
struct Rect
{
	Position2 pos;

	int w, h;
	Rect() :pos(0, 0), w(0), h(0) {}

	Rect(float x, float y, int inw, int inh) :
		pos(x, y), w(inw), h(inh) {}
	Rect(Position2 &inpos, int inw, int inh) :
		pos(inpos), w(inw), h(inh) {}
	void SetCenter(float x, float y) {
		pos.x = x;
		pos.y = y;
	}
	void SetCenter(Position2& inpos) {
		pos.x = inpos.x;
		pos.y = inpos.y;
	}

	Vector2 Center() {
		return pos;
	}
	//�㉺���E���m�F����
	float Left()	{ return pos.x - (w / 2); }
	float Top()		{ return pos.y - (h / 2); }
	float Right()	{ return pos.x + (w / 2); }
	float Bottom()	{ return pos.y + (h / 2); }
	//�l�p��Positioin2�ŕԂ�
	Position2 LeftTop()		 { return Position2(Left() ,Top());		}
	Position2 LeftBottom()	 { return Position2(Left() ,Bottom()-2);	}
	Position2 RightTop()	 { return Position2(Right(),Top());		}
	Position2 RightBottom()	 { return Position2(Right(),Bottom()-2);	}
	//�������`��\������
	void Draw(unsigned int color = 0xff00ffff) {
		DrawBox((int)Left(), (int)Top(), (int)Right(), (int)Bottom(), color, false);
	}
	void Draw(Position2 offset, unsigned int color = 0xff00ffff) {
		DrawBox((int)Left() - offset.x, (int)Top() - offset.y, (int)Right() - offset.x, (int)Bottom() - offset.y, color, false);
	}
};
//�~�ɂ���
struct Circle
{
	Position2 pos;
	float r;
	Circle() :pos(0, 0), r(0) {}
	Circle(float x, float y, float inr) :
		pos(x, y), r(inr) {}
	Circle(Position2 &inpos, float inr) :
		pos(inpos), r(inr) {}
	void SetCenter(float x,float y,float range) {
		pos.x = x;
		pos.y = y;
		r = range;
	}
	void SetCenter(Position2& inpos, float range) {
		pos.x = inpos.x;
		pos.y = inpos.y;
		r = range;
	}

	Vector2 Center() {
		return pos;
	}
	void Draw(unsigned int color = 0xff00ffff) {
		DrawCircle((int)pos.x,(int)pos.y,(int)r,color,false);
	}
	void Draw(Position2 offset, unsigned int color = 0xff00ffff) {
		DrawCircle((int)pos.x - offset.x, (int)pos.y - offset.y, (int)r, color, false);
	}
};
//�G�̃f�[�^�ɂ��āi�����蔻��Ŏg�p�j
struct ENEMY_DATA 
{
	ENEMY_DATA() :lookDir(DIR_NON), lookRange(), lookAngle(0) {}

	DIR lookDir;
	Circle lookRange;
	float lookAngle;
};
//�S�[�����̃f�[�^�ɂ���
struct RESULT_DATA
{
	RESULT_DATA() :transFlag(false), midFlag(false),goalFlag(false), goalTime(0),life(PLAYER_MAX), foundCount(0) {}
	bool transFlag;			//��ʑJ�ڗp�t���O
	bool midFlag;			//���ԓ_�ɓ������Ă��邩
	bool goalFlag;			//�S�[���ɓ��B�������ǂ���
	int goalTime;
	int life;
	int foundCount;
};
inline
Position2 operator+(const Position2 &pos, const Position2 &pos2)
{
	return Position2(pos.x + pos2.x, pos.y + pos2.y);
}
inline
Position2 operator-(const Position2 &pos, const Position2 &pos2)
{
	return Position2(pos.x - pos2.x, pos.y - pos2.y);
}
