#pragma once
#include "Geometry.h"
class EnemyFactory;

struct SetColor {
	SetColor() :red(0), green(0), blue(0) {}
	int red;
	int green;
	int blue;
};

class EnemyServer
{
private:
	EnemyFactory * _efac;
	EnemyServerData _commonData;
	int decreaseCnt;
	void AlertManager();
	int lightImage;
	int gaugeImage;
	SetColor lampColor;

	void SetLampColor();
public:
	EnemyServer();
	EnemyServer(EnemyFactory* f);
	~EnemyServer();
	int vigiCnt;
	void Updata();
	void GetInfo(EnemyServerData inData);							//�e�G��������󂯎��(��ڲ԰�������Ȃ�)
	void SetAlert();						//�x���x���Z�b�g����
	ENEMY_ALERT AlertLevel();				//���݂̌x�����x����Ԃ�
	void ServerInit();						//���������s���܂�

	void Draw(Position2 offset);
};

