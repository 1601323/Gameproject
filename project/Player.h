#pragma once
#include "Object.h"
#include "Geometry.h"
#include "Input.h"

#define ACCEL_X (0.3f)					//�����x
#define GRABITY (0.9f)					//�d��
#define MAX_SPEED (3.0f)				//��߰�ނ�MAX�l
#define MAX_GRABITY (15.0f)				//�d�͂�MAX�l
#define WALL_SPEED (1.0f)				//�Ǔo��̽�߰��
#define JUMP_POWER (15.0f)				//�ެ��ߗ�
#define VANISH_CNT (3)					//������܂ł̶���

class Input;
class HitClass;
class Camera;
class MapCtl;
class Rope;
class GameScene;

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
	Position2 _pos;						//��ׂ̍��W
	DIR _dir;							//��ׂ̌���
	Position2 initPos;					//�����ʒu�ۑ�
	float vx;							//x���x
	float vy;							//y���x
	int vanCnt;							//������܂ł̶���
	bool JumpFlag;						//�ެ��߂��׸�
	bool WallFlag;						//�ǂɒ���t���t���O
	bool moveFlag;						//�ǂɒ���t���Ă���Ƃ������邩�̃t���O
	bool moveRopeJumpFlag;				//۰�߼ެ��ߏ���
	void HitToEnemy();					//�G�Ɠ���������
	char keyData[256];
	char oldkeyData[256];

	void setMove(Input* input);			//�ړ��֐�
	void setState(void);				//�ð������
	void setDir(Input* input);			//��������
	void gravity(void);					//�d��
	bool accelePL(void);				//�ړ�����
	bool moveJump(void);				//�ެ��ߏ���
	bool moveWall(void);				//�ǈړ�����
	bool moveRope(void);				//۰�ߏ��
	bool stVanish(void);				//��ٽ����
	bool stFever(void);					//̨��ް����

public:
	Player();
	~Player();
	Rect _plRect;
	Rect& GetRect();
	void Update(Input* input);
	void Draw(Position2& offset);

	CHAR_ST GetcharState(void);			//�ð���擾
	Position2& GetPos(void);			//pos�擾
	void SetInitPos();					//�����ʒu��Ԃ�
	DIR GetDir(void);					//dir�擾	
	void SetRetryPos(Position2 midPos);
	void Getclass(HitClass* h, Rope*r);	//hit�N���X���󂯎�邽�߂̊֐�

	bool EnterDoor();					//�������@�h�A�ɓ�������true�ɂ��܂�
	float playerSpeedTable[SV_MAX] = { 0.f,1.f,MAX_SPEED,MAX_SPEED };//�X�e�B�b�N�̌X��������player��speed�̏���e�[�u��
	SENSING_VALUE _minSensingValueL;  // �è���̓��͂����m����Œ�̒l L
};

