#include <DxLib.h>
#include <stdio.h>

#include "GameMain.h"

int main() 
{
	GameMain::Instance().Run();
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow) {
//	ChangeWindowMode(true);
//	KeyState key;
//	if (DxLib_Init() == 1) {
//		return -1;
//	}
//	while (ProcessMessage() == 0 ) {
//		GetHitKeyStateAll(key.state);
//		ClearDrawScreen();
//		if (CheckHitKey(KEY_INPUT_ESCAPE))
//		{
//			break;
//		}
//		ScreenFlip();
//		memcpy(key.laststate, key.state, sizeof(key.state));
//	}
//	DxLib_End();
//	return 0;
//}