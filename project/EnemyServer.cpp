#include <DxLib.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <map>
#include <string>
#include <vector>
#include "Geometry.h"
#include "GameMain.h"
//#include "EnemyFactory.h"
#include "ImageMgr.h"
#include "Enemy.h"
#include "EmAround.h"
#include "EmLookback.h"
#include "EnemyServer.h"

EnemyServer::EnemyServer()
{
	_commonData._level = ALERT_LEVEL_1;
	_commonData.dataSendFlag = false;
	_commonData.plFoundFlag = false;
	vigiCnt = 0;
	decreaseCnt = 0;
	ImageMgr& im = ImageMgr::Instance();
	lightImage = im.ImageIdReturn("仮image/UI/Patrite2.png",SCENE_RESULT);
	lampColor.red = 0;
	lampColor.green = 0;
	lampColor.blue = 255;
	cnt = 0;
}
EnemyServer::EnemyServer(EnemyFactory* f)
{
	_efac = f;
	_commonData._level = ALERT_LEVEL_1;
	_commonData.dataSendFlag = false;
	_commonData.plFoundFlag = false;
	_commonData.midFlag = false;
	vigiCnt = 0;
	decreaseCnt = 0;
	ImageMgr& im = ImageMgr::Instance();
	lightImage = im.ImageIdReturn("仮image/UI/Patrite2.png",SCENE_RESULT);
	cnt = 0;
}

EnemyServer::~EnemyServer()
{
}
void EnemyServer::ServerInit()
{
	_commonData._level = ALERT_LEVEL_1;
	_commonData.dataSendFlag = false;
	_commonData.plFoundFlag = false;
	vigiCnt = 0;
	decreaseCnt = 0;
	cnt = 0;
}
void EnemyServer::Updata()
{
	if (cnt <= 50) {
		cnt++;
	}
	else {
		cnt *= -1;
	}
	AlertManager();
	SetAlert();

	SetLampColor();
}
void EnemyServer::AlertManager()
{
	if (vigiCnt > 0) {
		decreaseCnt++;
		if (decreaseCnt > 300) {
			vigiCnt -= 1;
			decreaseCnt = 0;
		}
	}
}
//それぞれの敵から情報をもらう
void EnemyServer::GetInfo(EnemyServerData inData)
{
	//ﾌﾟﾚｲﾔｰ発見情報が上がってきたらレベルを上げる
	if (inData.plFoundFlag == true) {
		vigiCnt +=20;
		SetAlert();
		inData.dataSendFlag = false;
	}
}

//警戒度をセットする
void EnemyServer::SetAlert()
{
	if (0 <= vigiCnt &&vigiCnt <= 40) {
		_commonData._level = ALERT_LEVEL_1;
	}
	else if (40< vigiCnt &&vigiCnt <= 80) {
		_commonData._level = ALERT_LEVEL_2;
	}
	else if (80 < vigiCnt && vigiCnt <= 100) {
		_commonData._level = ALERT_LEVEL_3;
	}
	else {
	}
}
//警戒ランプの色を設定します
void EnemyServer::SetLampColor()
{
	if (_commonData._level == ALERT_LEVEL_1) {
		lampColor.blue = 255;
		lampColor.green = 0;
		lampColor.red = 0;
	}
	else if (_commonData._level == ALERT_LEVEL_2) {
		lampColor.green = 255;
		lampColor.red = 255;
		lampColor.blue = 0;
	}
	else if (_commonData._level == ALERT_LEVEL_3) {
		lampColor.red = 255;
		lampColor.green = 0;
		lampColor.blue = 0;
	}
	else {
		lampColor.red = 255;
		lampColor.green = 255;
		lampColor.blue = 255;
	}
}
void EnemyServer::Draw(Position2 offset) 
{
	ImageMgr& im = ImageMgr::Instance();
//#ifdef _DEBUG
	DrawFormatString(300,100,0xffffff,"%d",vigiCnt);
//	DrawBox(600, 30, 600 + vigiCnt, 60, 0xffff25, true);
//	DrawBox(600,30,700,60,0xff00ff,false);
//#endif
	//場所は完全には決定していない

	//色変化を実装
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,180);
	SetDrawBright(lampColor.red,lampColor.green,lampColor.blue);
	DrawExtendGraph(700, 0,770,85, lightImage, true);
	SetDrawBright(255,255,255);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawExtendGraph(700,0,770,85,im.ImageIdReturn("仮image/UI/Patrite1.png",SCENE_RESULT),true);

}
//現在の警戒レベルを返す
ENEMY_ALERT EnemyServer::AlertLevel()
{
	return _commonData._level;
}
//ﾌﾟﾚｲﾔｰが中間地点を取得したかどうかをもらう
void EnemyServer::SetMidFlag(bool mid)
{
	_commonData.midFlag = mid;
}
//敵全体に中間地点情報を渡す
bool EnemyServer::SendMidFlag()
{
	return _commonData.midFlag;
}