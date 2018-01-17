#include <DxLib.h>
#include <list>
#include <memory>
#include <map>
#include <vector>
#include "Assert.h"
#include "Geometry.h"
#include "GameScene.h"
#include "Enemy.h"
#include "EnemyFactory.h"
#include "EnemyServer.h"

#include "EmAround.h"
#include "EmLookback.h"

using namespace std;
using Enemy_ptr = std::shared_ptr<Enemy>;
typedef std::list<std::shared_ptr<Enemy>> EnemyList_t;

EnemyFactory::EnemyFactory(Player& p,Rope& r,EnemyServer& s):_player(p),_rope(r),_server(s)
{
}

EnemyFactory::~EnemyFactory()
{
}

std::weak_ptr<Enemy>EnemyFactory::Create(ENEMY_TYPE et,Position2 pos)
{
	//後でposも受け取るようにそれぞれのクラスのコンストラクタを書き換える
	switch (et) {
	case ENEMY_NONE:
		break;
	case ENEMY_TURN:
		_enemyList.push_back(Enemy_ptr(new EmLookback(pos,_player,_rope,_server)));
		break;
	case ENEMY_WARKING:
		_enemyList.push_back(Enemy_ptr(new EmAround(pos,_player,_rope,_server)));
		break;
	case ENEMY_MIMICRY:
		break;
	default:
		ASSERT();
		break;
	}
	return _enemyList.back();
}
void EnemyFactory::Updata()
{
	for (auto& em : _enemyList) {
		em->Updata();
	}
}
void EnemyFactory::Draw(Position2 offset)
{
	for (auto& em : _enemyList) {
		em->Draw(offset);
	}
}
void EnemyFactory::EnemyFalter()
{
	for (auto& em : _enemyList) {
		em->EnemyFalter();
	}
}
//リストを返す
EnemyList_t& EnemyFactory::EnemyList()
{
	return _enemyList;
}