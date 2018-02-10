#include <list>
#include <memory>
#include "Assert.h"
#include "Tutorial.h"
#include "ImageMgr.h"
#include "GameMain.h"
#include "Geometry.h"
#include "Player.h"
#include "HitClass.h"

using namespace std;
typedef std::list<std::shared_ptr<Tutorial>> TutoList_t;

Tutorial::Tutorial(Player& player, HitClass& h):_player(player),_hit(h)
{
	DrawMessageBoxFlag = false;
	messageMoveCheck = false;
	MessageRect.w = 20;
	MessageRect.h = 20;
	for (int f = 0; f < MESSAGE_NUM; f++) {
		MessageRect2[f].w = 20;
		MessageRect2[f].h = 20;
		HitFlag[f] = false;
		checkFlag[f] = false;
	}

}

Tutorial::~Tutorial()
{

}

void Tutorial::Updata(Input* input)
{
	_key = input->GetInput(1).key;
	_lastKey = input->GetLastKey();
	_inpInfo = input->GetInput(1);
	//CheckChainPosition();
	
	//if (DrawMessageBoxFlag == false) {
		for (int f = 0; f < MESSAGE_NUM; f++) {
			if (_hit.IsHit(GetRect2(f), _player.GetRect()))
			{
				messageMoveCheck = true;
				break;
			}
			else {
				DrawMessageBoxFlag = false;
			}
		}
	//}
	check1();
	check2();
	check3();
	check4();
	check5();
	check6();
	check7();
	//面倒なのでflagで
	if (_inpInfo.num >= 1) {
		padFlag = true;
	}
	else {
		padFlag = false;
	}
}

void Tutorial::CheckChainPosition()
{
	//forで回して一致してるかどうかを
	for (int i = 0; i < MESSAGE_NUM; i++)
	{
		if (!messageMoveCheck)
		{
			//一致していたら

			//if(_hit.TutorialHit(_player))
			if(_hit.IsHit(GetRect2(i),_player.GetRect()))
			{
				MessageNumFlag[i] = true;
				DrawMessageBoxFlag = true;
				messageMoveCheck = true;
			}
			else {
				MessageNumFlag[i] = false;
				DrawMessageBoxFlag = false;

			}
		}
		//テキストを閉じる
		if (_key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON)
		{
			DrawMessageBoxFlag = false;
		}
	}
}

void Tutorial::Draw(Position2 &offset)
{
	ImageMgr& im = ImageMgr::Instance();

	for (int i = 0; i < MESSAGE_NUM; i++)
	{
		//MessageRect.SetCenter(drawMassagePosition[i].x, drawMassagePosition[i].y);
		//MessageRect.Draw(offset);
		MessageRect2[i].SetCenter(drawMassagePosition[i].x, drawMassagePosition[i].y);
		//MessageRect2[i].Draw(offset);
	}
	if (DrawMessageBoxFlag)
	{
		//1 移動ジャンプ
		if (MessageNumFlag[0] == true) {
			DrawGraph(200, 150, im.ImageIdReturn(padFlag ? "image/Tutorial/GamePad/Tutorial1.5.png" : "image/Tutorial/Keyboard/Tutorial1.png", SCENE_RESULT), true);
			//DrawGraph(530, 380, im.ImageIdReturn(padFlag ? "image/UI/backBotton1.png" : "image/UI/keyBackSpace.png", SCENE_RESULT), true);
		}
		//2 張り付く
		if (MessageNumFlag[1] == true) {
			DrawGraph(200, 10, im.ImageIdReturn(padFlag ? "image/Tutorial/GamePad/Tutorial2.5.png" : "image/Tutorial/Keyboard/Tutorial2.png", SCENE_RESULT), true);
			//DrawGraph(530, 380, im.ImageIdReturn(padFlag ? "image/UI/backBotton1.png" : "image/UI/keyBackSpace.png", SCENE_RESULT), true);
		}
		//3 ロープ
		if (MessageNumFlag[2] == true) {
			DrawGraph(200, 200, im.ImageIdReturn(padFlag ? "image/Tutorial/GamePad/Tutorial3.5.png" : "image/Tutorial/Keyboard/Tutorial3.png", SCENE_RESULT), true);
			//DrawGraph(530, 380, im.ImageIdReturn(padFlag ? "image/UI/backBotton1.png" : "image/UI/keyBackSpace.png", SCENE_RESULT), true);
		}
		//4 ステルス
		if (MessageNumFlag[3] == true) {
			DrawGraph(200, 200, im.ImageIdReturn("image/Tutorial/Keyboard/Tutorial4.png", SCENE_RESULT), true);
			//DrawGraph(530, 380, im.ImageIdReturn(padFlag ? "image/UI/backBotton1.png" : "image/UI/keyBackSpace.png", SCENE_RESULT), true);
		}
		//5 鍵とか金庫
		if (MessageNumFlag[4] == true) {
			DrawGraph(200, 10, im.ImageIdReturn(padFlag ? "image/Tutorial/GamePad/Tutorial2.5.png" : "image/Tutorial/Keyboard/Tutorial2.png", SCENE_RESULT), true);
			//DrawGraph(530, 380, im.ImageIdReturn(padFlag ? "image/UI/backBotton1.png" : "image/UI/keyBackSpace.png", SCENE_RESULT), true);
		}
		//6 トランスアイテム(食パン)
		if (MessageNumFlag[5] == true) {
			DrawGraph(200, 200, im.ImageIdReturn("image/Tutorial/Keyboard/Tutorial5.png", SCENE_RESULT), true);
			//DrawGraph(530, 380, im.ImageIdReturn(padFlag ? "image/UI/backBotton1.png" : "image/UI/keyBackSpace.png", SCENE_RESULT), true);
		}
		//6 トランスアイテム(食パン)
		if (MessageNumFlag[6] == true) {
			DrawGraph(200, 200, im.ImageIdReturn("image/Tutorial/Keyboard/Tutorial6.png", SCENE_RESULT), true);
			//DrawGraph(530, 380, im.ImageIdReturn(padFlag ? "image/UI/backBotton1.png" : "image/UI/keyBackSpace.png", SCENE_RESULT), true);
		}
	}
	//if (DrawMessageBoxFlag)
	//{
	//	//それぞれのflagがtrueなら描画しますよ
	//	//1 移動ジャンプ
	//	if (MessageNumFlag[0])
	//	{
	//		DrawGraph(200, 150, im.ImageIdReturn(padFlag ? "image/Tutorial/GamePad/Tutorial1.5.png" : "image/Tutorial/Keyboard/Tutorial1.png", SCENE_RESULT), true);
	//	}
	//	//2 張り付く
	//	else if (MessageNumFlag[1])
	//	{
	//		DrawGraph(200, 10, im.ImageIdReturn(padFlag ? "image/Tutorial/GamePad/Tutorial2.5.png" : "image/Tutorial/Keyboard/Tutorial2.png", SCENE_RESULT), true);
	//	}
	//	//3 ロープ
	//	else if (MessageNumFlag[2])
	//	{
	//		DrawGraph(200, 200, im.ImageIdReturn(padFlag ? "image/Tutorial/GamePad/Tutorial3.5.png" : "image/Tutorial/Keyboard/Tutorial3.png", SCENE_RESULT), true);
	//	}
	//	//4 ステルス
	//	else if (MessageNumFlag[3])
	//	{
	//		DrawGraph(200, 200, im.ImageIdReturn("image/Tutorial/Keyboard/Tutorial4.png", SCENE_RESULT), true);
	//	}
	//	//5 鍵とか金庫
	//	else if (MessageNumFlag[4])
	//	{
	//		DrawGraph(200, 200, im.ImageIdReturn("image/Tutorial/Keyboard/Tutorial5.png", SCENE_RESULT), true);
	//	}
	//	//6 トランスアイテム(食パン)
	//	else if (MessageNumFlag[5])
	//	{
	//		DrawGraph(200, 200 ,im.ImageIdReturn("image/Tutorial/Keyboard/Tutorial6.png", SCENE_RESULT), true);
	//	}
	//	///7 ゴール
	//	else if (MessageNumFlag[6])
	//	{
	//		DrawGraph(560, 450, im.ImageIdReturn("image/Tutorial/Keyboard/Tutorial7.png", SCENE_RESULT), true);
	//	}

	//	DrawGraph(530, 380, im.ImageIdReturn(padFlag ? "image/UI/backBotton1.png" : "image/UI/keyBackSpace.png", SCENE_RESULT), true);
	//}
}

void Tutorial::Getclass(HitClass& h, Player& pl)
{
	_hit = h;
	_player = pl;
}

bool Tutorial::GetMessageFlag()
{
	return DrawMessageBoxFlag;
}

Rect& Tutorial::GetRect()
{
	return MessageRect;
}
Rect& Tutorial::GetRect2(int num)
{
	return MessageRect2[num];
}
void Tutorial::RetryInit()
{
	for (int i = 0; i < MESSAGE_NUM; i++)
	{
		MessageNumFlag[i] = false;
	}
	DrawMessageBoxFlag = false;
	messageMoveCheck = false;
}

//リストを返す
TutoList_t& Tutorial::TutorialList()
{
	return _tutoList;
}

void Tutorial::check1()
{
	if (_hit.IsHit(GetRect2(0), _player.GetRect()) == true) {
 		if (HitFlag[0] == false) {
			MessageNumFlag[0] = true;
			DrawMessageBoxFlag = true;
			checkFlag[0] = true;
		}
	}
	else {
		MessageNumFlag[0] = false;
	}
	if (_key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON)
	{
		if (checkFlag[0] == true) {
			DrawMessageBoxFlag = false;
			HitFlag[0] = true;
		}
	}
}
void Tutorial::check2()
{
	if (_hit.IsHit(GetRect2(1), _player.GetRect()) == true) {
		if (HitFlag[1] == false) {
			DrawMessageBoxFlag = true;
			MessageNumFlag[1] = true;
			checkFlag[1] = true;
		}
	}
	else {
		MessageNumFlag[1] = false;
	}

	if (_key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON)
	{
		if (checkFlag[1] == true) {
			DrawMessageBoxFlag = false;
			HitFlag[1] = true;
		}
	}

}
void Tutorial::check3()
{
	if (_hit.IsHit(GetRect2(2), _player.GetRect()) == true) {
		if (HitFlag[2] == false) {
			DrawMessageBoxFlag = true;
			MessageNumFlag[2] = true;
			checkFlag[2] = true;
		}
	}
	else {
		MessageNumFlag[2] = false;
	}

	if (_key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON)
	{
		if (checkFlag[2] == true) {
			DrawMessageBoxFlag = false;
			HitFlag[2] = true;
		}
	}
}
void Tutorial::check4()
{
	if (_hit.IsHit(GetRect2(3), _player.GetRect()) == true) {
		if (HitFlag[3] == false) {
			DrawMessageBoxFlag = true;
			MessageNumFlag[3] = true;
			checkFlag[3] = true;
		}
	}
	else {
		MessageNumFlag[3] = false;
	}

	if (_key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON)
	{
		if (checkFlag[3] == true) {
			DrawMessageBoxFlag = false;
			HitFlag[3] = true;
		}
	}

}
void Tutorial::check5()
{
	if (_hit.IsHit(GetRect2(4), _player.GetRect()) == true) {
		if (HitFlag[4] == false) {
			DrawMessageBoxFlag = true;
			MessageNumFlag[4] = true;
			checkFlag[4] = true;
		}
	}
	else {
		MessageNumFlag[4] = false;
	}

	if (_key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON)
	{
		if (checkFlag[4] == true) {
			DrawMessageBoxFlag = false;
			HitFlag[4] = true;
		}
	}

}
void Tutorial::check6()
{
	if (_hit.IsHit(GetRect2(5), _player.GetRect()) == true) {
		if (HitFlag[5] == false) {
			DrawMessageBoxFlag = true;
			MessageNumFlag[5] = true;
			checkFlag[5] = true;
		}
	}
	else {
		MessageNumFlag[5] = false;
	}

	if (_key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON)
	{
		if (checkFlag[5] == true) {
			DrawMessageBoxFlag = false;
			HitFlag[5] = true;
		}
	}
}
void Tutorial::check7()
{
	if (_hit.IsHit(GetRect2(6), _player.GetRect()) == true) {
		if (HitFlag[6] == false) {
			DrawMessageBoxFlag = true;
			MessageNumFlag[6] = true;
			checkFlag[6] = true;
		}
	}
	else {
		MessageNumFlag[6] = false;
	}

	if (_key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON)
	{
		if (checkFlag[6] == true) {
			DrawMessageBoxFlag = false;
			HitFlag[6] = true;
		}
	}

}