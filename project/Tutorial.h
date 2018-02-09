#pragma once
#include <list>
#include <memory>
#include "Input.h"
#include "Geometry.h"

#define MESSAGE_NUM (7)       //メッセージ数

//メッセージが出現するposition
const Position2 drawMassagePosition[MESSAGE_NUM] = {
	{100,720},                 //1 移動ジャンプ
	{710,750},                 //2 張り付く
	{830,660},                 //3 ロープ
	{700,450},                 //4 ステルス
	{230,220},                 //5 鍵とか金庫
	{480,220},                 //6 トランスアイテム(食パン)
	{740,220}                  //7 ゴール
};

class Player;
class Input;
class HitClass;

class Tutorial
{
private:
	KEY _key;
	KEY _lastKey;
	INPUT_INFO _inpInfo;
	Player& _player;
	HitClass& _hit;
	Position2 tmpOffset;
	typedef std::list<std::shared_ptr<Tutorial>> TutoList_t;
	TutoList_t _tutoList;

	bool padFlag;                       //パッドが刺さっているか見るflag
	bool messageMoveCheck;              //メッセージの入力受理を見るflag
public:
	Tutorial(Player& player,HitClass& h);
	~Tutorial();
	Rect MessageRect;
	Rect MessageRect2[MESSAGE_NUM];
	void Updata(Input* input);
	void Draw(Position2 &offset);
	void CheckChainPosition();         //プレイヤーの座標と指定座標を一致してるかを確認する
	void Getclass(HitClass& h,Player& pl);	       //hitクラスを受け取るための関数
	void RetryInit();                  //リトライ時などで呼び出される
	Rect &GetRect();
	Rect& GetRect2(int num);
	TutoList_t& TutorialList();

	bool GetMessageFlag();
	bool DrawMessageBoxFlag;           //メッセージ画像が出ているかどうかを表すflag
	bool MessageNumFlag[MESSAGE_NUM];  //メッセージ画像のflag個別

};

