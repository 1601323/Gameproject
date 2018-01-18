#include <string.h>
#include <math.h>
#include <DxLib.h>
#include <iostream>
#include "Input.h"
#include "Geometry.h"
#include "Math.h"

using namespace std;
const unsigned int SENSING_VALUE_HIGH = 15000;		            // �è�������͂����m����l
const unsigned int SENSING_VALUE_MID = 23000;
const unsigned int SENSING_VALUE_LOW = 30000;

int Input::joypadCnt = 0;

char xboxName[] = { "Controller (XBOX 360 For Windows)" };		// xbox���۰װ�����޲�������ѐ��i��

//�L�[���̎���(���� Input* input)�̏ꍇ
//KEY key = input->GetInput(1).key;
//KEY lastKey = input->GetLastKey();
//INPUT_INFO inpInfo = input->GetInput(1);

//��
//if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON)
//{
//	//Enter�L�[ or A�{�^�� ����
//}
//if (input->GetStickDir(inpInfo.L_STICK) == SD_LEFT)
//{
//	//���Ɉړ�
//}

Input::Input()
{
	DxLib::XINPUT_STATE input;
	isXbox = false;

	joypadCnt = GetJoypadNum();                                   //�Q�[���p�b�h�̐ڑ������擾

	for (int padNum = 0; padNum < joypadCnt; ++padNum) {
		GetJoypadXInputState(padNum + 1, &input);

		// �X�e�B�b�N�̕␳�l���쐬
		L_correction[padNum] = Calibration(input.ThumbLX, input.ThumbLY);
		R_correction[padNum] = Calibration(input.ThumbRX, input.ThumbRY);
		inpInfo[padNum].L_Stick.L_SensingFlag = SV_NONE;
	}

	if (joypadCnt >= 1) {
		// �ڑ�����Ă�����۰װ�̎�ނ��擾
		char iName[64];
		char pName[64];
		GetJoypadName(1, iName, pName);
		//Xbox�R���g���[���[�Ȃ���͂���
		if (!strcmp(iName, xboxName) || !strcmp(pName, xboxName)) {
			isXbox = true;
		}
		else {
			isXbox = false;
		}
	}
}

Input::~Input()
{
}

void Input::Update(void)
{
	memcpy(&lastKey, &inpInfo[0].key, sizeof(KEY));
	if (isXbox == true) {
		padInit();
	}
}

void Input::InputSet()
{
	DxLib::XINPUT_STATE input = {};

	GetHitKeyStateAll(KeyData);	                           // �S�L�[�f�[�^�Ăяo��

	//������if���ŃR���g���[���[���ڑ�����Ă��邩�ǂ������m�F

	//�ڑ�����Ă���ꍇ
	if (joypadCnt >= 1)
	{
		for (int padNum = 0; padNum < joypadCnt; ++padNum) {
			GetJoypadXInputState(padNum + 1, &input);

			Position2 stickPos;	                            // ���è����X=���X�e�B�b�N�̉����l,Y=���X�e�B�b�N�̏c���l

			Position2 pos1 = { 1.f, 0.f };		            // �p�x�Z�o�p�x�N�g��
			Position2 pos2;				                    // �X�e�B�b�N�x�N�g���i���l�j

		    //���X�e�B�b�N�̑��
			stickPos.x = input.ThumbLX;                     // ���X�e�B�b�N�̉����l( -32768 �` 32767)
			stickPos.y = input.ThumbLY;                     // ���X�e�B�b�N�̏c���l( -32768 �` 32767)

			// �␳�l��stickPos�ɔ��f
			Calibration(&stickPos, L_correction[padNum]);
			pos2 = { stickPos.x , -stickPos.y };

			//�X�e�B�b�N�̌X���ɂ���Ċ��x��؂�ւ���(L_STICK)
			inpInfo[padNum].L_Stick.L_SensingFlag = SV_NONE;
			if (VecLen(stickPos) >= SENSING_VALUE_HIGH)
			{
				inpInfo[padNum].L_Stick.L_SensingFlag = SV_HIGH;
			}
			if (VecLen(stickPos) >= SENSING_VALUE_MID)
			{
				inpInfo[padNum].L_Stick.L_SensingFlag = SV_MID;
			}
			if (VecLen(stickPos) >= SENSING_VALUE_LOW)
			{
				inpInfo[padNum].L_Stick.L_SensingFlag = SV_LOW;
			}

			inpInfo[padNum].L_Stick.lstick = AngleOf2Vector(pos1, pos2);

			//�E�X�e�B�b�N�̑��
			stickPos.x = input.ThumbRX;                     // �E�X�e�B�b�N�̉����l( -32768 �` 32767)
			stickPos.y = input.ThumbRY;                     // �E�X�e�B�b�N�̏c���l( -32768 �` 32767)

		    // �␳�l��stickPos�ɔ��f
			Calibration(&stickPos, R_correction[padNum]);
			pos2 = { stickPos.x , -stickPos.y };

			//�X�e�B�b�N�̌X���ɂ���Ċ��x��؂�ւ���(R_STICK)
			inpInfo[padNum].R_Stick.R_SensingFlag = SV_NONE;
			if (VecLen(stickPos) >= SENSING_VALUE_HIGH)
			{
				inpInfo[padNum].R_Stick.R_SensingFlag = SV_HIGH;
			}
			if (VecLen(stickPos) >= SENSING_VALUE_MID)
			{
				inpInfo[padNum].R_Stick.R_SensingFlag = SV_MID;
			}
			if (VecLen(stickPos) >= SENSING_VALUE_LOW)
			{
				inpInfo[padNum].R_Stick.R_SensingFlag = SV_LOW;
			}

			inpInfo[padNum].R_Stick.rstick = AngleOf2Vector(pos1, pos2);

			//�L�[���͏��
			inpInfo[padNum].key.keybit.A_BUTTON = input.Buttons[XINPUT_BUTTON_A];                 // ����
			inpInfo[padNum].key.keybit.B_BUTTON = input.Buttons[XINPUT_BUTTON_B];
			inpInfo[padNum].key.keybit.Y_BUTTON = input.Buttons[XINPUT_BUTTON_Y];                 // ��ݾ�
			inpInfo[padNum].key.keybit.X_BUTTON = input.Buttons[XINPUT_BUTTON_X];

			inpInfo[padNum].key.keybit.START_BUTTON = input.Buttons[XINPUT_BUTTON_START];         // ����g
			inpInfo[padNum].key.keybit.BACK_BUTTON = input.Buttons[XINPUT_BUTTON_BACK];           // �߂�

			inpInfo[padNum].key.keybit.L_THUMB = input.Buttons[XINPUT_BUTTON_LEFT_THUMB];         // LB���� NUM��+��
			inpInfo[padNum].key.keybit.R_THUMB = input.Buttons[XINPUT_BUTTON_RIGHT_THUMB];        // RB���� NUM��+��

			inpInfo[padNum].key.keybit.R_DOWN_BUTTON = input.Buttons[PAD_INPUT_DOWN];             // �E�è�� NUM�@��
			inpInfo[padNum].key.keybit.R_RIGHT_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_RIGHT];  // �E�è�� NUM�@�E
			inpInfo[padNum].key.keybit.R_LEFT_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_LEFT];    // �E�è�� NUM�@��
			inpInfo[padNum].key.keybit.R_UP_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_UP];	      // �E�è�� NUM�@��

			inpInfo[padNum].key.keybit.L_DOWN_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_DOWN];    // ���è�� S �޼��ف@��
			inpInfo[padNum].key.keybit.L_RIGHT_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_RIGHT];  // ���è�� D �޼��ف@�E
			inpInfo[padNum].key.keybit.L_LEFT_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_LEFT];    // ���è�� A �޼��ف@��
			inpInfo[padNum].key.keybit.L_UP_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_UP];        // ���è�� W �޼��ف@��
			inpInfo[padNum].num = joypadCnt;//�p�b�h�̐�

		}
	}
	//�ڑ�����Ă��Ȃ�
	else {
		//�ǂ̃L�[���ǂ��ɐݒ肷�邩�͂܂�����
		inpInfo[0].key.keybit.A_BUTTON = KeyData[KEY_INPUT_RETURN];
		inpInfo[0].key.keybit.B_BUTTON = KeyData[KEY_INPUT_BACK];

		inpInfo[0].key.keybit.Y_BUTTON = KeyData[KEY_INPUT_ADD];
		inpInfo[0].key.keybit.X_BUTTON = KeyData[KEY_INPUT_RETURN];

		inpInfo[0].key.keybit.START_BUTTON = KeyData[KEY_INPUT_E];
		inpInfo[0].key.keybit.BACK_BUTTON = KeyData[KEY_INPUT_R];

		inpInfo[0].key.keybit.L_THUMB = KeyData[KEY_INPUT_DIVIDE];
		inpInfo[0].key.keybit.R_THUMB = KeyData[KEY_INPUT_MULTIPLY];

		inpInfo[0].key.keybit.R_DOWN_BUTTON = KeyData[KEY_INPUT_DOWN];                    // �����L�[��
		inpInfo[0].key.keybit.R_RIGHT_BUTTON = KeyData[KEY_INPUT_RIGHT];                  // �����L�[�E
		inpInfo[0].key.keybit.R_LEFT_BUTTON = KeyData[KEY_INPUT_LEFT];                    // �����L�[��
		inpInfo[0].key.keybit.R_UP_BUTTON = KeyData[KEY_INPUT_UP];	                      // �����L�[��

		inpInfo[0].key.keybit.L_DOWN_BUTTON = KeyData[KEY_INPUT_S];                       // S ��
		inpInfo[0].key.keybit.L_RIGHT_BUTTON = KeyData[KEY_INPUT_D];                      // D �E
		inpInfo[0].key.keybit.L_LEFT_BUTTON = KeyData[KEY_INPUT_A];                       // A ��
		inpInfo[0].key.keybit.L_UP_BUTTON = KeyData[KEY_INPUT_W];                         // W ��
		inpInfo[0].num = joypadCnt;
	}

}

//�Q�[���p�b�h�̕������擾����i���E�㉺��4�����j
STICK_DIR Input::GetStickDir(float stick)
{	
	//cout <<(int)( stick / AngleRad(60.f)) << endl;
	cout << stick << endl;
	if ((int)(stick / AngleRad(60.f)) == 2 ||		// �E 60�x
		(int)(stick / AngleRad(60.f)) == 3) {
		return SD_RIGHT;
	}
	if ((int)(stick / AngleRad(60.f)) == 0 ||		// �� 60�x
		(int)(stick / AngleRad(60.f)) == 5) {
		return SD_LEFT;
	}
	if ((int)(stick / AngleRad(60.f)) == 1) {
		return SD_UP;
	}
	if ((int)(stick / AngleRad(60.f)) == 4 ) {
		return SD_DOWN;
	}

	return SD_NONE;
}

Position2 Input::Calibration(float rawInputX, float rawInputY)
{
	Position2 returnPos;    //�߂�l�p

	returnPos = { (0 - (rawInputX)), (0 - (rawInputY)) };

	return returnPos;
}

void Input::Calibration(Position2* L_Input, Position2 cVal)
{
	L_Input->x = cVal.x;
	L_Input->y = cVal.y;
}

L_STICK Input::STickL()
{
	L_STICK returnL;

	returnL = { lstick, L_SensingFlag };

	return returnL;
}

R_STICK Input::StickR()
{
	R_STICK returnR;

	returnR = { rstick, R_SensingFlag };

	return returnR;
}

INPUT_INFO Input::GetInput(int player)
{
	return inpInfo[player - 1];
}

KEY Input::GetKey(void) {
	return key;
}

KEY Input::GetLastKey(void) {
	return lastKey;
}

int Input::GetPadNum(void) {
	return joypadCnt;
}
//�p�b�h�̍X�V���s��
void Input::padInit()
{
	DxLib::XINPUT_STATE input;

	joypadCnt = GetJoypadNum();                                   //�Q�[���p�b�h�̐ڑ������擾

	for (int padNum = 0; padNum < joypadCnt; ++padNum) {
		GetJoypadXInputState(padNum + 1, &input);

		// �X�e�B�b�N�̕␳�l���쐬
		L_correction[padNum] = Calibration(input.ThumbLX, input.ThumbLY);
		R_correction[padNum] = Calibration(input.ThumbRX, input.ThumbRY);
		inpInfo[padNum].L_Stick.L_SensingFlag = SV_NONE;
	}
}