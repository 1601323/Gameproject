#include <string.h>
#include <math.h>
#include <DxLib.h>
#include <iostream>
#include "Input.h"
#include "Geometry.h"
#include "Math.h"

using namespace std;
const unsigned int SENSING_VALUE_HIGH = 15000;		            // ｽﾃｨｯｸが入力を感知する値
const unsigned int SENSING_VALUE_MID = 23000;
const unsigned int SENSING_VALUE_LOW = 30000;

int Input::joypadCnt = 0;

char xboxName[] = { "Controller (XBOX 360 For Windows)" };		// xboxｺﾝﾄﾛｰﾗｰのﾃﾞﾊﾞｲｽ名および製品名

//キー情報の取り方(引数 Input* input)の場合
//KEY key = input->GetInput(1).key;
//KEY lastKey = input->GetLastKey();
//INPUT_INFO inpInfo = input->GetInput(1);

//例
//if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON)
//{
//	//Enterキー or Aボタン 処理
//}
//if (input->GetStickDir(inpInfo.L_STICK) == SD_LEFT)
//{
//	//左に移動
//}

Input::Input()
{
	DxLib::XINPUT_STATE input;
	isXbox = false;

	joypadCnt = GetJoypadNum();                                   //ゲームパッドの接続数を取得

	for (int padNum = 0; padNum < joypadCnt; ++padNum) {
		GetJoypadXInputState(padNum + 1, &input);

		// スティックの補正値を作成
		L_correction[padNum] = Calibration(input.ThumbLX, input.ThumbLY);
		R_correction[padNum] = Calibration(input.ThumbRX, input.ThumbRY);
		inpInfo[padNum].L_Stick.L_SensingFlag = SV_NONE;
	}

	if (joypadCnt >= 1) {
		// 接続されているｺﾝﾄﾛｰﾗｰの種類を取得
		char iName[64];
		char pName[64];
		GetJoypadName(1, iName, pName);
		//Xboxコントローラーなら入力を受理
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

	GetHitKeyStateAll(KeyData);	                           // 全キーデータ呼び出し

	//ここのif文でコントローラーが接続されているかどうかを確認

	//接続されている場合
	if (joypadCnt >= 1)
	{
		for (int padNum = 0; padNum < joypadCnt; ++padNum) {
			GetJoypadXInputState(padNum + 1, &input);

			Position2 stickPos;	                            // 左ｽﾃｨｯｸのX=左スティックの横軸値,Y=左スティックの縦軸値

			Position2 pos1 = { 1.f, 0.f };		            // 角度算出用ベクトル
			Position2 pos2;				                    // スティックベクトル（軸値）

		    //左スティックの代入
			stickPos.x = input.ThumbLX;                     // 左スティックの横軸値( -32768 ～ 32767)
			stickPos.y = input.ThumbLY;                     // 左スティックの縦軸値( -32768 ～ 32767)

			// 補正値をstickPosに反映
			Calibration(&stickPos, L_correction[padNum]);
			pos2 = { stickPos.x , -stickPos.y };

			//スティックの傾きによって感度を切り替える(L_STICK)
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

			//右スティックの代入
			stickPos.x = input.ThumbRX;                     // 右スティックの横軸値( -32768 ～ 32767)
			stickPos.y = input.ThumbRY;                     // 右スティックの縦軸値( -32768 ～ 32767)

		    // 補正値をstickPosに反映
			Calibration(&stickPos, R_correction[padNum]);
			pos2 = { stickPos.x , -stickPos.y };

			//スティックの傾きによって感度を切り替える(R_STICK)
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

			//キー入力情報
			inpInfo[padNum].key.keybit.A_BUTTON = input.Buttons[XINPUT_BUTTON_A];                 // 決定
			inpInfo[padNum].key.keybit.B_BUTTON = input.Buttons[XINPUT_BUTTON_B];
			inpInfo[padNum].key.keybit.Y_BUTTON = input.Buttons[XINPUT_BUTTON_Y];                 // ｷｬﾝｾﾙ
			inpInfo[padNum].key.keybit.X_BUTTON = input.Buttons[XINPUT_BUTTON_X];

			inpInfo[padNum].key.keybit.START_BUTTON = input.Buttons[XINPUT_BUTTON_START];         // ｽﾀｰト
			inpInfo[padNum].key.keybit.BACK_BUTTON = input.Buttons[XINPUT_BUTTON_BACK];           // 戻る

			inpInfo[padNum].key.keybit.L_THUMB = input.Buttons[XINPUT_BUTTON_LEFT_THUMB];         // LBﾎﾞﾀﾝ NUMの+ｷｰ
			inpInfo[padNum].key.keybit.R_THUMB = input.Buttons[XINPUT_BUTTON_RIGHT_THUMB];        // RBﾎﾞﾀﾝ NUMの+ｷｰ

			inpInfo[padNum].key.keybit.R_DOWN_BUTTON = input.Buttons[PAD_INPUT_DOWN];             // 右ｽﾃｨｯｸ NUM　下
			inpInfo[padNum].key.keybit.R_RIGHT_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_RIGHT];  // 右ｽﾃｨｯｸ NUM　右
			inpInfo[padNum].key.keybit.R_LEFT_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_LEFT];    // 右ｽﾃｨｯｸ NUM　左
			inpInfo[padNum].key.keybit.R_UP_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_UP];	      // 右ｽﾃｨｯｸ NUM　上

			inpInfo[padNum].key.keybit.L_DOWN_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_DOWN];    // 左ｽﾃｨｯｸ S ﾃﾞｼﾞﾀﾙ　下
			inpInfo[padNum].key.keybit.L_RIGHT_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_RIGHT];  // 左ｽﾃｨｯｸ D ﾃﾞｼﾞﾀﾙ　右
			inpInfo[padNum].key.keybit.L_LEFT_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_LEFT];    // 左ｽﾃｨｯｸ A ﾃﾞｼﾞﾀﾙ　左
			inpInfo[padNum].key.keybit.L_UP_BUTTON = input.Buttons[XINPUT_BUTTON_DPAD_UP];        // 左ｽﾃｨｯｸ W ﾃﾞｼﾞﾀﾙ　上
			inpInfo[padNum].num = joypadCnt;//パッドの数

		}
	}
	//接続されていない
	else {
		//どのキーをどこに設定するかはまだ未定
		inpInfo[0].key.keybit.A_BUTTON = KeyData[KEY_INPUT_RETURN];
		inpInfo[0].key.keybit.B_BUTTON = KeyData[KEY_INPUT_BACK];

		inpInfo[0].key.keybit.Y_BUTTON = KeyData[KEY_INPUT_ADD];
		inpInfo[0].key.keybit.X_BUTTON = KeyData[KEY_INPUT_RETURN];

		inpInfo[0].key.keybit.START_BUTTON = KeyData[KEY_INPUT_E];
		inpInfo[0].key.keybit.BACK_BUTTON = KeyData[KEY_INPUT_R];

		inpInfo[0].key.keybit.L_THUMB = KeyData[KEY_INPUT_DIVIDE];
		inpInfo[0].key.keybit.R_THUMB = KeyData[KEY_INPUT_MULTIPLY];

		inpInfo[0].key.keybit.R_DOWN_BUTTON = KeyData[KEY_INPUT_DOWN];                    // 方向キー下
		inpInfo[0].key.keybit.R_RIGHT_BUTTON = KeyData[KEY_INPUT_RIGHT];                  // 方向キー右
		inpInfo[0].key.keybit.R_LEFT_BUTTON = KeyData[KEY_INPUT_LEFT];                    // 方向キー左
		inpInfo[0].key.keybit.R_UP_BUTTON = KeyData[KEY_INPUT_UP];	                      // 方向キー上

		inpInfo[0].key.keybit.L_DOWN_BUTTON = KeyData[KEY_INPUT_S];                       // S 下
		inpInfo[0].key.keybit.L_RIGHT_BUTTON = KeyData[KEY_INPUT_D];                      // D 右
		inpInfo[0].key.keybit.L_LEFT_BUTTON = KeyData[KEY_INPUT_A];                       // A 左
		inpInfo[0].key.keybit.L_UP_BUTTON = KeyData[KEY_INPUT_W];                         // W 上
		inpInfo[0].num = joypadCnt;
	}

}

//ゲームパッドの方向を取得する（左右上下の4方向）
STICK_DIR Input::GetStickDir(float stick)
{	
	//cout <<(int)( stick / AngleRad(60.f)) << endl;
	cout << stick << endl;
	if ((int)(stick / AngleRad(60.f)) == 2 ||		// 右 60度
		(int)(stick / AngleRad(60.f)) == 3) {
		return SD_RIGHT;
	}
	if ((int)(stick / AngleRad(60.f)) == 0 ||		// 左 60度
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
	Position2 returnPos;    //戻り値用

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
//パッドの更新を行う
void Input::padInit()
{
	DxLib::XINPUT_STATE input;

	joypadCnt = GetJoypadNum();                                   //ゲームパッドの接続数を取得

	for (int padNum = 0; padNum < joypadCnt; ++padNum) {
		GetJoypadXInputState(padNum + 1, &input);

		// スティックの補正値を作成
		L_correction[padNum] = Calibration(input.ThumbLX, input.ThumbLY);
		R_correction[padNum] = Calibration(input.ThumbRX, input.ThumbRY);
		inpInfo[padNum].L_Stick.L_SensingFlag = SV_NONE;
	}
}