#pragma once
#include "Geometry.h"
#include "Math.h"


//スティックの傾きの感度
enum SENSING_VALUE {
	SV_NONE,
	SV_HIGH,
	SV_MID,
	SV_LOW,
	SV_MAX
};

//ゲームパッド方向
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


//ボタン操作
union KEY {
	struct KEYBIT
	{
		unsigned int A_BUTTON : 1;	                  // 決定
		unsigned int B_BUTTON : 1;	                  // 決定
		unsigned int X_BUTTON : 1;	                  // ｷｬﾝｾﾙ
		unsigned int Y_BUTTON : 1;	                  // ｷｬﾝｾﾙ
		unsigned int START_BUTTON : 1;	              // ｽﾀｰﾄ
		unsigned int BACK_BUTTON : 1;	              // 戻る
		unsigned int L_THUMB : 1;	                  // 左ｽﾃｨｯｸ押し込み 旧LBﾎﾞﾀﾝ
		unsigned int R_THUMB : 1;	                  // 右ｽﾃｨｯｸ押し込み 旧RBﾎﾞﾀﾝ
		unsigned int R_DOWN_BUTTON : 1;	              // 右ｽﾃｨｯｸ NUM　下
		unsigned int R_RIGHT_BUTTON : 1;	          // 右ｽﾃｨｯｸ NUM　右
		unsigned int R_LEFT_BUTTON : 1;	              // 右ｽﾃｨｯｸ NUM　左
		unsigned int R_UP_BUTTON : 1;	              // 右ｽﾃｨｯｸ NUM　上
		unsigned int L_DOWN_BUTTON : 1;	              // 左ｽﾃｨｯｸ S ﾃﾞｼﾞﾀﾙ　下
		unsigned int L_RIGHT_BUTTON : 1;	          // 左ｽﾃｨｯｸ D ﾃﾞｼﾞﾀﾙ　右
		unsigned int L_LEFT_BUTTON : 1;	              // 左ｽﾃｨｯｸ A ﾃﾞｼﾞﾀﾙ　左
		unsigned int L_UP_BUTTON : 1;	              // 左ｽﾃｨｯｸ W ﾃﾞｼﾞﾀﾙ　上
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
	int num;                                          //ゲームパッドの接続数を入れておくための変数
	KEY key;
};

class Input
{
public:
	Input();
	~Input();
	void Update(void);
	void InputSet();                                  //これを呼び出すことで入力情報を得る
	static int GetPadNum(void);                       //パッドの数情報を返す
	KEY GetKey(void);                                 //key情報を返す
	KEY GetLastKey(void);                             //lastkey情報を返す
	INPUT_INFO GetInput(int player);                  //パッドの入力情報を返す
	STICK_DIR GetStickDir(float stick);               //スティックの向き情報を返す
	L_STICK STickL();                                 // 左ｽﾃｨｯｸ情報を返す
	R_STICK StickR();                                 // 右ｽﾃｨｯｸ情報を返す

	float lstick;                                     // 左ｽﾃｨｯｸの角度（ﾗｼﾞｱﾝ）
	float rstick;                                     // 右ｽﾃｨｯｸの角度（ﾗｼﾞｱﾝ）
	unsigned char Buttons[16];                        //ボタン用配列
	bool isXbox;                                      //xboxｺﾝﾄﾛｰﾗｰが接続されているかどうかを確認するflag
	INPUT_INFO inpInfo[1];                            //中の要素はプレイヤー数です。とりあえず1
	Position2 L_correction[1];                        // 左ｽﾃｨｯｸの補正値
	Position2 R_correction[1];                        // 右ｽﾃｨｯｸの補正値

	SENSING_VALUE L_SensingFlag;                      // ｽﾃｨｯｸ入力感知ﾌﾗｸL
	SENSING_VALUE R_SensingFlag;                      // ｽﾃｨｯｸ入力感知ﾌﾗｸR

private:
	Position2 Calibration(float L_rawInputX, float L_rawInputY);    //補正値を作成  ※最初に一度だけ実行
	void Calibration(Position2* L_Input, Position2 cVal);           //補正値を反映
	char KeyData[256];                                              //キー情報の配列
	static int joypadCnt;                                           //ジョイパッドが接続されている数を取得するための静的変数
	KEY key;
	KEY lastKey;
	void padInit();										//パッドの入力について
};
