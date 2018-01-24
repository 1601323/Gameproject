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
	void GetInfo(EnemyServerData inData);							//各敵から情報を受け取る(ﾌﾟﾚｲﾔｰ見つけたなど)
	void SetAlert();						//警戒度をセットする
	ENEMY_ALERT AlertLevel();				//現在の警戒レベルを返す
	void ServerInit();						//初期化を行います

	void Draw(Position2 offset);
};

