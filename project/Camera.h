#pragma once

#include <memory>
#include "Object.h"
#include "Player.h"

using namespace std;

class Player;
class MapCtl;

class Camera :
	public Object
{
public:
	Position2 tmpPos;
	Position2 GetGameScrSize(void);
	bool SetTarget(Player* _player);			// ﾀｰｹﾞｯﾄを決める関数
	bool SetMapCtl(MapCtl* mapCtl);				// ﾏｯﾌﾟｺﾝﾄﾛｰﾙ入れる関数
	bool SetGameScrSize(Position2 gmScrSize);	// ｹﾞｰﾑ画面に映る範囲ｻｲｽﾞ
	bool SetCamAreaOffset(Position2 offset);	// ｵﾌｾｯﾄ情報を保持する										
	void Update();
	void Draw();
	Camera(const Camera&);						// シングルトン
	const Camera& operator=(const Camera&);
	//Camera(Player* player);
	virtual~Camera();
	//bool AddObject(Object* obj);				// カメラに移したいやつ追加用
	static Camera* GetInstance(void);

	Position2& ReturnOffset();					//offsetの値を返します
private:
	Camera();
	MapCtl* _mapCtl;
	Player* _player;
	Rope* _rope;
	Position2 CamOffset;
	Position2 GameScrSize;
	static Camera* cam_ptr;		// カメラのポインタ
};

