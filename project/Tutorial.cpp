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
	CheckChainPosition();
	
	//�ʓ|�Ȃ̂�flag��
	if (_inpInfo.num >= 1) {
		padFlag = true;
	}
	else {
		padFlag = false;
	}
}

void Tutorial::CheckChainPosition()
{
	//for�ŉ񂵂Ĉ�v���Ă邩�ǂ�����
	for (int i = 0; i < MESSAGE_NUM; i++)
	{
		if (!messageMoveCheck)
		{
			//��v���Ă�����(������Ȃ�)

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
		//�e�L�X�g�����
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
		MessageRect2[i].Draw(offset);
	}

	if (DrawMessageBoxFlag)
	{
		//���ꂼ���flag��true�Ȃ�`�悵�܂���
		//1 �ړ��W�����v
		if (MessageNumFlag[0])
		{
			DrawGraph(200, 150, im.ImageIdReturn(padFlag ? "image/Tutorial/GamePad/Tutorial1.5.png" : "image/Tutorial/Keyboard/Tutorial1.png", SCENE_RESULT), true);
		}
		//2 ����t��
		else if (MessageNumFlag[1])
		{
			DrawGraph(200, 10, im.ImageIdReturn(padFlag ? "image/Tutorial/GamePad/Tutorial2.5.png" : "image/Tutorial/Keyboard/Tutorial2.png", SCENE_RESULT), true);
		}
		//3 ���[�v
		else if (MessageNumFlag[2])
		{
			DrawGraph(200, 200, im.ImageIdReturn(padFlag ? "image/Tutorial/GamePad/Tutorial3.5.png" : "image/Tutorial/Keyboard/Tutorial3.png", SCENE_RESULT), true);
		}
		//4 �X�e���X
		else if (MessageNumFlag[3])
		{
			DrawGraph(200, 200, im.ImageIdReturn("image/Tutorial/Keyboard/Tutorial4.png", SCENE_RESULT), true);
		}
		//5 ���Ƃ�����
		else if (MessageNumFlag[4])
		{
			DrawGraph(200, 200, im.ImageIdReturn("image/Tutorial/Keyboard/Tutorial5.png", SCENE_RESULT), true);
		}
		//6 �g�����X�A�C�e��(�H�p��)
		else if (MessageNumFlag[5])
		{
			DrawGraph(200, 200 ,im.ImageIdReturn("image/Tutorial/Keyboard/Tutorial6.png", SCENE_RESULT), true);
		}
		///7 �S�[��
		else if (MessageNumFlag[6])
		{
			DrawGraph(200, 200, im.ImageIdReturn("image/Tutorial/Keyboard/Tutorial7.png", SCENE_RESULT), true);
		}

		DrawGraph(0, 0, im.ImageIdReturn(padFlag ? "image/UI/backBotton1.png" : "image/UI/keyBackSpace.png", SCENE_RESULT), true);
	}
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

//���X�g��Ԃ�
TutoList_t& Tutorial::TutorialList()
{
	return _tutoList;
}

