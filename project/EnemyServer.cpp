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
	lightImage = im.ImageIdReturn("��image/UI/Patrite2.png",SCENE_RESULT);
	gaugeImage = im.ImageIdReturn("��image/UI/UI_WarningGage1.png",SCENE_RESULT);
}
EnemyServer::EnemyServer(EnemyFactory* f)
{
	_efac = f;
	_commonData._level = ALERT_LEVEL_1;
	_commonData.dataSendFlag = false;
	_commonData.plFoundFlag = false;
	vigiCnt = 0;
	decreaseCnt = 0;
	ImageMgr& im = ImageMgr::Instance();
	lightImage = im.ImageIdReturn("��image/UI/Patrite2.png",SCENE_RESULT);
	gaugeImage = im.ImageIdReturn("��image/UI/UI_WarningGage1.png", SCENE_RESULT);

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
}
void EnemyServer::Updata()
{
	AlertManager();
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
//���ꂼ��̓G����������炤
void EnemyServer::GetInfo(EnemyServerData inData)
{
	//��ڲ԰������񂪏オ���Ă����烌�x�����グ��
	if (inData.plFoundFlag == true) {
		vigiCnt += 10;
		if (0 <= vigiCnt &&vigiCnt <= 30) {
			_commonData._level = ALERT_LEVEL_1;
		}
		else if (30< vigiCnt &&vigiCnt <= 80) {
			_commonData._level = ALERT_LEVEL_2;
		}
		else if (80 < vigiCnt && vigiCnt <= 100) {
			_commonData._level = ALERT_LEVEL_3;
		}
		else {
		}
		inData.dataSendFlag = false;
	}
}

//�x���x���Z�b�g����
void EnemyServer::SetAlert()
{
	vigiCnt += 25;
	if (0 <= vigiCnt &&vigiCnt <= 50) {
		_commonData._level = ALERT_LEVEL_1;
	}
	else if (50< vigiCnt &&vigiCnt <= 100) {
		_commonData._level = ALERT_LEVEL_2;
	}
}
void EnemyServer::Draw(Position2 offset) 
{
	ImageMgr& im = ImageMgr::Instance();
#ifdef _DEBUG
	DrawFormatString(300,100,0xffffff,"%d",vigiCnt);
	DrawBox(600, 30, 600 + vigiCnt, 60, 0xffff25, true);
	DrawBox(600,30,700,60,0xff00ff,false);
#endif
	//�ꏊ�͊��S�ɂ͌��肵�Ă��Ȃ�
	SetDrawBright(0,0,255);
	DrawExtendGraph(500,30,500+(vigiCnt*2),90,im.ImageIdReturn("��image/UI/UI_WarningGage1.png", SCENE_RESULT),true);
	DrawExtendGraph(500, 30,700,90 ,im.ImageIdReturn("��image/UI/UI_WarningGage.png",SCENE_RESULT), true);

	//�F�ω�������
	SetDrawBright(0,0,255);
	DrawGraph(670, 5, lightImage, true);
	SetDrawBright(255,255,255);
	DrawGraph(670,5,im.ImageIdReturn("��image/UI/Patrite1.png",SCENE_RESULT),true);

}
//���݂̌x�����x����Ԃ�
ENEMY_ALERT EnemyServer::AlertLevel()
{
	return _commonData._level;
}
