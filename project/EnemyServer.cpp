#include <DxLib.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include "Geometry.h"
//#include "EnemyFactory.h"
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
}
EnemyServer::EnemyServer(EnemyFactory* f)
{
	_efac = f;
	_commonData._level = ALERT_LEVEL_1;
	_commonData.dataSendFlag = false;
	_commonData.plFoundFlag = false;
	vigiCnt = 0;
	decreaseCnt = 0;
}

EnemyServer::~EnemyServer()
{
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
		vigiCnt += 25;
		if (0 <= vigiCnt &&vigiCnt <= 50) {
			_commonData._level = ALERT_LEVEL_1;
		}
		else if (50< vigiCnt &&vigiCnt <= 100) {
			_commonData._level = ALERT_LEVEL_2;
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
	DrawFormatString(300,100,0xffffff,"%d",vigiCnt);
	DrawBox(400, 30, 400 + vigiCnt, 60, 0xffff25, true);
	DrawBox(400,30,500,60,0xff00ff,false);
}
//���݂̌x�����x����Ԃ�
ENEMY_ALERT EnemyServer::AlertLevel()
{
	return _commonData._level;
}
