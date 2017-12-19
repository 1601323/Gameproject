#pragma once
#include "Geometry.h"
#include "Math.h"


//�X�e�B�b�N�̌X���̊��x
enum SENSING_VALUE {
	SV_NONE,
	SV_HIGH,
	SV_MID,
	SV_LOW,
	SV_MAX
};

//�Q�[���p�b�h����
enum STICK_DIR {
	SD_NONE,
	SD_RIGHT,
	SD_LEFT,
	SD_UP,
	SD_DOWN
};

struct L_STICK 
{
	float lstick;
	SENSING_VALUE L_SensingFlag;
};

struct R_STICK
{
	float rstick;
	SENSING_VALUE R_SensingFlag;
};


//�{�^������
union KEY {
	struct KEYBIT
	{
		unsigned int A_BUTTON : 1;	                  // ����
		unsigned int B_BUTTON : 1;	                  // ����
		unsigned int X_BUTTON : 1;	                  // ��ݾ�
		unsigned int Y_BUTTON : 1;	                  // ��ݾ�
		unsigned int START_BUTTON : 1;	              // ����
		unsigned int BACK_BUTTON : 1;	              // �߂�
		unsigned int L_THUMB : 1;	                  // ���è���������� ��LB����
		unsigned int R_THUMB : 1;	                  // �E�è���������� ��RB����
		unsigned int R_DOWN_BUTTON : 1;	              // �E�è�� NUM�@��
		unsigned int R_RIGHT_BUTTON : 1;	          // �E�è�� NUM�@�E
		unsigned int R_LEFT_BUTTON : 1;	              // �E�è�� NUM�@��
		unsigned int R_UP_BUTTON : 1;	              // �E�è�� NUM�@��
		unsigned int L_DOWN_BUTTON : 1;	              // ���è�� S �޼��ف@��
		unsigned int L_RIGHT_BUTTON : 1;	          // ���è�� D �޼��ف@�E
		unsigned int L_LEFT_BUTTON : 1;	              // ���è�� A �޼��ف@��
		unsigned int L_UP_BUTTON : 1;	              // ���è�� W �޼��ف@��
	} keybit;	                                      //16
	unsigned short int data;
};

struct INPUT_INFO
{
	INPUT_INFO() 
	{
		L_Stick.L_SensingFlag = SV_NONE;
		L_Stick.lstick = 0;
		R_Stick.R_SensingFlag = SV_NONE;
		R_Stick.rstick = 0;
		key.data = 0;
	}
	L_STICK L_Stick;							     
	R_STICK R_Stick;							      
	int num;                                          //�Q�[���p�b�h�̐ڑ��������Ă������߂̕ϐ�
	KEY key;
};

class Input
{
public:
	Input();
	~Input();
	void Update(void);
	void InputSet();                                  //������Ăяo�����Ƃœ��͏��𓾂�
	static int GetPadNum(void);                       //�p�b�h�̐�����Ԃ�
	KEY GetKey(void);                                 //key����Ԃ�
	KEY GetLastKey(void);                             //lastkey����Ԃ�
	INPUT_INFO GetInput(int player);                  //�p�b�h�̓��͏���Ԃ�
	STICK_DIR GetStickDir(float stick);               //�X�e�B�b�N�̌�������Ԃ�
	L_STICK STickL();                                 // ���è������Ԃ�
	R_STICK StickR();                                 // �E�è������Ԃ�

	float lstick;                                     // ���è���̊p�x�i׼ޱ݁j
	float rstick;                                     // �E�è���̊p�x�i׼ޱ݁j
	unsigned char Buttons[16];                        //�{�^���p�z��
	bool isXbox;                                      //xbox���۰װ���ڑ�����Ă��邩�ǂ������m�F����flag
	INPUT_INFO inpInfo[1];                            //���̗v�f�̓v���C���[���ł��B�Ƃ肠����1
	Position2 L_correction[1];                        // ���è���̕␳�l
	Position2 R_correction[1];                        // �E�è���̕␳�l

	SENSING_VALUE L_SensingFlag;                      // �è�����͊��m�׸L
	SENSING_VALUE R_SensingFlag;                      // �è�����͊��m�׸R

private:
	Position2 Calibration(float L_rawInputX, float L_rawInputY);    //�␳�l���쐬  ���ŏ��Ɉ�x�������s
	void Calibration(Position2* L_Input, Position2 cVal);           //�␳�l�𔽉f
	char KeyData[256];                                              //�L�[���̔z��
	static int joypadCnt;                                           //�W���C�p�b�h���ڑ�����Ă��鐔���擾���邽�߂̐ÓI�ϐ�
	KEY key;
	KEY lastKey;
	void padInit();										//�p�b�h�̓��͂ɂ���
};
