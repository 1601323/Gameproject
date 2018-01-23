#pragma once
#include "Geometry.h"
class EnemyFactory;

class EnemyServer
{
private:
	EnemyFactory * _efac;
	EnemyServerData _commonData;
	int decreaseCnt;
	void AlertManager();

	int data;
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

