#pragma once
#include "Object.h"
#include "Geometry.h"
#include "Input.h"

#define ACCEL_X (0.3f)					//�����x
#define MAX_SPEED (3.0f)				//��߰�ނ�MAX�l
#define MAX_GRAVITY (15.0f)				//�d�͂�MAX�l
#define WALL_SPEED (1.0f)				//�Ǔo��̽�߰��
#define JUMP_POWER (15.0f)				//�ެ��ߗ�
#define VANISH_CNT (2)					//������܂ł̶���
#define VANISH    (2)                   //������܂ł̶���
#define FEVER_CNT (10)					//�t�B�[�o�[�̎���
#define ANIMATION_SPEED_SLOW (0.3f)     //�A�j���[�V�����̑���(���ᑬ)
#define ANIMATION_SPEED_DEF (0.5f)      //�A�j���[�V�����̑���(�ᑬ)
#define ANIMATION_SPEED_HIGH (0.8f)     //�A�j���[�V�����̑���(����)
#define INVINCIBLETIMER (2)             //���G����


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
	ModelMgr* _modelmgr;
	//GameScene& gmScen;
	CHAR_ST _state;						//��ׂ̏��
	Position3 _pos;						//��ׂ̍��W
	DIR _dir;							//��ׂ̌���
	Position3 initPos;					//�����ʒu�ۑ�
	float vx;							//x���x
	float vy;							//y���x
	int vanCnt;							//������܂ł̶���
	int modelhandle;                    //�v���C���[���f���n���h��
	int alfa;                           //�X�e���X�p�A���t�@
	int tranceMax;                      //�ő哧�ߐ�
	int textureIndex;                   //��̃e�N�X�`���C���f�b�N�X
	int  AnimIndex[ACTION_MAX];         //�A�j���[�V������
	float AnimTotalTime[ACTION_MAX];    //�e�A�j���[�V�����̃g�[�^���^�C��
	float AnimNowTime[ACTION_MAX];      //�e�A�j���[�V�����̌��݃^�C��
	float modelDirAngle;                //���f���\���p��Y���̉�]��
	bool feverFlag;						//�t�B�[�o�[�t���O
	int feverTime;						//�t�B�[�o�[�̎���
	bool inviFlag;                      //���G���ԃt���O
	int inviCnt;                        //���G���ԃJ�E���^�[
	bool JumpFlag;						//�ެ��߂��׸�
	bool WallFlag;						//�ǂɒ���t���t���O
	bool moveFlag;						//�ǂɒ���t���Ă���Ƃ������邩�̃t���O
	bool moveRopeJumpFlag;				//۰�߼ެ��ߏ���
	bool airFlag;						//�󒆂ɂ��邩�ǂ����̃t���O
	bool fMoveRight;					//̨��ް���A�ǂ̂ڂ�𐧌䂷��t���O
	bool fMoveLeft;						//����
	bool deathFlag;						//�܂��Ɏ��S�t���O�i_state�ł͊Ǘ�������Ȃ��݂����Ȃ̂�
	bool helpFever;						//̨��ް�I�����̕␳�̂��߂ɗp��
	bool crouchFlag;					//���Ⴊ�݃t���O
	bool ropeFlag;						//���[�v��Ԃ��ǂ�����Ԃ��t���O
	bool vanFlag;						//�X�e���X��Ԃ��ǂ����̃t���O
	bool tmpFlag;						//�␳�p�t���O
	char keyData[256];
	char oldkeyData[256];

	void HitToEnemy();					//�G�Ɠ���������
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
	void moveCrouch(Input* input);					//���Ⴊ��
	bool stVanish(void);				//��ٽ����
	bool stFever(void);					//̨��ް����
	bool plPlaceCheck();				//̨��ް�I�����A��ڲ԰���ǂ̒��ɂ��Ȃ����`�F�b�N���܂�
	void stInvincible(void);            //���G��Ԃ𐧌䂷��֐�

	void FeverUpdata(Input* input);		//�t�B�[�o�[���ɌĂяo���S�̂̏���
	void FeverJump();					//�t�B�[�o�[�p�̃W�����v����
	void FeverGravity();				//�t�B�[�o�[�p�̏d�͏���
	void FeverWall();					//�t�B�[�o�[�p�̕ǈړ�����

	void AnimationSwitching(void);      //�A�j���[�V�����؂�ւ��悤�֐�
	void AnimationManager(PLAYER_ACTIONS actions,float animspeed,float looptime);        //�A�j���[�V�����Z�b�g�֐�

	Position2 tmpOffset;
	VECTOR WorldToScreenPos;            //���[���h���W����X�N���[�����W�ɕϊ�������̃��f���\���p��pos

	Rect _wallRect;						//�Ǘp�̃��N�g

	FEVER_DATA _fd;
	RESULT_DATA _rtData;
	void GetFeverData();
	

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
	void SetInitPausePos();             //�|�[�X��ʂ��烊�g���C����ꍇ�������ɓ���
	void Getclass(HitClass* h, Rope*r);	//hit�N���X���󂯎�邽�߂̊֐�

	bool EnterDoor();					//�������@�h�A�ɓ�������true�ɂ��܂�
	bool GetStateCrouch();				//���Ⴊ��ł��邩�ǂ�����Ԃ��܂�
	bool GetStateVanish();				//�X�e���X���Ă��邩�ǂ�����Ԃ��܂�
	bool GetStateRope();				//���[�v���g���Ă��邩�ǂ�����Ԃ��܂�
	float playerSpeedTable[SV_MAX] = { 0.f,1.f,MAX_SPEED,MAX_SPEED };//�X�e�B�b�N�̌X��������player��speed�̏���e�[�u��
	SENSING_VALUE _minSensingValueL;  // �è���̓��͂����m����Œ�̒l L
	Position2 ReturnWoToScPos2ver();   //���[���h���W����X�N���[�����W�ɕϊ����pos��Ԃ��֐�
};

