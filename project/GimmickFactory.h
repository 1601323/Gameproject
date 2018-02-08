#pragma once
#include <list>
#include <memory>
#include <map>
#include "Geometry.h"
class GameScene;
class Gimmick;
class Player;
class Rope;
//動作確認用
class Input;

class GimmickFactory
{
	friend GameScene;
private:
	GimmickFactory(Player& p,Rope& r);
	Player& _player;
	Rope& _rope;
	typedef std::list<std::shared_ptr<Gimmick>> GimmickList_t;
	GimmickList_t _gimmickList;
	std::weak_ptr<Gimmick>GimmickFactory::Create(CHIP_TYPE gt,Position2 pos);	//リストへの追加を行う
public:

	~GimmickFactory();
	void Updata(Input& input);
	void Draw();
	void Draw(Position2 offset);
	void SetInit();
	GimmickList_t& GimmickList();	//リストを返す
};

