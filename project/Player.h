#pragma once
#include "Object.h"
#include "Geometry.h"
#include "Input.h"

#define ACCEL_X (0.3f)					//�����x
#define MAX_SPEED (3.0f)				//��߰�ނ�MAX�l
#define MAX_GRAVITY (15.0f)				//�d�͂�MAX�l
#define WALL_SPEED (1.0f)				//�Ǔo��̽�߰��
#define JUMP_POWER (15.0f)				//�ެ��ߗ�
#define VANISH_CNT (3)					//������܂ł̶���
#define FEVER_CNT (10)					//�t�B�[�o�[�̎���
#define ANIMATION_SPEED_SLOW (0.3f)     //�A�j���[�V�����̑���(���ᑬ)
#define ANIMATION_SPEED_DEF (0.5f)      //�A�j���[�V�����̑���(�ᑬ)
#define ANIMATION_SPEED_HIGH (0.8f)     //�A�j���[�V�����̑���(����)

class Input;
class HitClass;
class Camera;
class MapCtl;
class Rope;
class GameScene;
class ModelMgr;

struct L_STICK;
struct R_STICK;

enum SENSING_VALUE;

class Player :public Object
{
private:
	KEY _key;
	KEY _lastKey;
	INPUT_INFO _inpInfo;
	HitClass *_hit;
	MapCtl *_map;
	Rope* _rope;
	//GameScene& gmScen;
	CHAR_ST _state;						//��ׂ̏��
	Position3 _pos;						//��ׂ̍��W
	DIR _dir;							//��ׂ̌���
	Position3 initPos;					//�����ʒu�ۑ�
	float vx;							//x���x
	float vy;							//y���x
	int vanCnt;							//������܂ł̶���
	bool feverFlag;						//�t�B�[�o�[�t���O
	int feverTime;						//�t�B�[�o�[�̎���
	bool JumpFlag;						//�ެ��߂��׸�
	bool WallFlag;						//�ǂɒ���t���t���O
	bool moveFlag;						//�ǂɒ���t���Ă���Ƃ������邩�̃t���O
	bool moveRopeJumpFlag;				//۰�߼ެ��ߏ���
	bool fMoveRight;					//̨��ް���A�ǂ̂ڂ�𐧌䂷��t���O
	bool fMoveLeft;						//����
	bool deathFlag;						//�܂��Ɏ��S�t���O�i_state�ł͊Ǘ�������Ȃ��݂����Ȃ̂�
	bool helpFever;						//̨��ް�I�����̕␳�̂��߂ɗp��
	void HitToEnemy();					//�G�Ɠ���������
	char keyData[256];
	char oldkeyData[256];

	void setMove(Input* input);			//�ړ��֐�
	void setState(void);				//�ð������
	void setDir(Input* input);			//��������
	void gravity(void);					//�d��
	bool accelePL(void);				//�ړ�����
	void InputSetMove();				//�ړ������̒��̓��͂��󂯎����܂�
	bool moveJump(void);				//�ެ��ߏ���
	bool moveWall(void);				//�ǈړ�����
	bool moveRope(void);				//۰�ߏ��
	void moveFever();
	bool stVanish(void);				//��ٽ����
	bool stFever(void);					//̨��ް����
	bool plPlaceCheck();				//̨��ް�I�����A��ڲ԰���ǂ̒��ɂ��Ȃ����`�F�b�N���܂�

	void FeverUpdata(Input* input);		//�t�B�[�o�[���ɌĂяo���S�̂̏���
	void FeverJump();					//�t�B�[�o�[�p�̃W�����v����
	void FeverGravity();				//�t�B�[�o�[�p�̏d�͏���
	void FeverWall();					//�t�B�[�o�[�p�̕ǈړ�����

	void AnimationSwitching(void);      //�A�j���[�V�����؂�ւ��悤�֐�
	Position2 modelPlayerPos;

	Position2 tmpOffset;
	//�����Ȃ�Ȃ̂ł����ɏ����Ă܂�
	ModelMgr* _modelmgr;
	int modelhandle;
	int alfa;
	int tranceMax;
	float  AnimTotalTime[ACTION_MAX];
	float  AnimNowTime[ACTION_MAX];
	float modelDirAngle;
	int  AnimIndex[ACTION_MAX];
	float outlineNum;
	Position3 TempMoveVector;

public:
	Player();
	~Player();
	Rect _plRect;
	Rect& GetRect();
	void Update(Input* input);
	void Draw(Position2& offset);

	CHAR_ST GetcharState(void);			//�ð���擾
	Position2& GetPos(void);			//pos�擾
	void SetInitPos();					//�����ʒu�ɖ߂�
	void SetInitPos(Position2 p);		//�����ʒu���Z�b�g����
	DIR GetDir(void);					//dir�擾	
	void SetRetryPos(Position2 midPos);
	void Getclass(HitClass* h, Rope*r);	//hit�N���X���󂯎�邽�߂̊֐�

	bool EnterDoor();					//�������@�h�A�ɓ�������true�ɂ��܂�
	float playerSpeedTable[SV_MAX] = { 0.f,1.f,MAX_SPEED,MAX_SPEED };//�X�e�B�b�N�̌X��������player��speed�̏���e�[�u��
	SENSING_VALUE _minSensingValueL;  // �è���̓��͂����m����Œ�̒l L

	Position2& GetModelPos(void);
};

