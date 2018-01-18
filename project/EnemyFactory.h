#pragma once
#include <list>
#include <memory>
#include <map>
class GameScene;
class Player;
class Rope;
class Enemy;
class EnemyServer;
class HitClass;

class EnemyFactory
{
	friend GameScene;
private:
	EnemyFactory(Player& p ,Rope& r,EnemyServer& s,HitClass& h);
	Player& _player;
	Rope& _rope;
	EnemyServer& _server;
	HitClass& _hit;
	typedef std::list<std::shared_ptr<Enemy>> EnemyList_t;
	EnemyList_t _enemyList;
	std::weak_ptr<Enemy>EnemyFactory::Create(ENEMY_TYPE et,Position2 pos);
public:
	~EnemyFactory();
	void Updata();
	void Draw(Position2 offset);
	void EnemyFalter();
	EnemyList_t& EnemyList();
};

