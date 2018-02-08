#pragma once
#include <list>
#include <memory>
#include <map>
#include "Geometry.h"
class GameScene;
class Gimmick;
class Player;
class Rope;
//����m�F�p
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
	std::weak_ptr<Gimmick>GimmickFactory::Create(CHIP_TYPE gt,Position2 pos);	//���X�g�ւ̒ǉ����s��
public:

	~GimmickFactory();
	void Updata(Input& input);
	void Draw();
	void Draw(Position2 offset);
	void SetInit();
	GimmickList_t& GimmickList();	//���X�g��Ԃ�
};

