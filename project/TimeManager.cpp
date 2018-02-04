#include <DxLib.h>
#include <ctime>
#include <iostream>
#include "Geometry.h"
#include "ImageMgr.h"
#include "GameMain.h"
#include "TimeManager.h"

#define NUM_X (74)
#define NUM_Y (100)


TimeManager::TimeManager()
{
	ImageMgr& im = ImageMgr::Instance();
	timerFlag = false;
	timerCnt = 0;
	flameCnt = 0;
	numberImage = im.ImageIdReturn("image/UI/NewNum.png", SCENE_RESULT);
	second = 0;
	tenex = 0;
	hunex = 0;
}


TimeManager::~TimeManager()
{
}

void TimeManager::Updata()
{
	if (timerFlag == true) {
		flameCnt++;
		if (flameCnt % 60 == 0) {
			timerCnt++;
		}
	}
	if (timerCnt > 999) {
		timerCnt = 999;
	}

}
void TimeManager::StartTimer()
{
	timerFlag = true;
}
void TimeManager::StopTimer()
{
	timerFlag = false;
}
//ŽžŠÔ‚ð•Ô‚·
int TimeManager::ShowTimer()
{
	if (timerFlag == false) {
		return timerCnt;
	}
	return 0;
}
void TimeManager::Draw()
{
	//ŠÈ’P‚ÉŽÀ‘•
	second = timerCnt % 10;
	tenex = (timerCnt / 10)%10;
	hunex = timerCnt / 100;

	DrawRectExtendGraph(400,			  10, 400+(NUM_X/2),				10 + (NUM_Y /2), NUM_X * second,0,NUM_X,NUM_Y, numberImage, true);
	DrawRectExtendGraph(400- (NUM_X/2)*1, 10, 400+(NUM_X/2) - (NUM_X/2),	10 + (NUM_Y /2), NUM_X * tenex, 0,NUM_X,NUM_Y, numberImage, true);
	DrawRectExtendGraph(400- (NUM_X/2)*2, 10, 400+(NUM_X/2) - (NUM_X/2)*2,  10 + (NUM_Y /2), NUM_X * hunex, 0,NUM_X,NUM_Y, numberImage, true);

}