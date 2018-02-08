#pragma once
#include <list>
#include <memory>
#include "Input.h"
#include "Geometry.h"

#define MESSAGE_NUM (7)       //���b�Z�[�W��

//���b�Z�[�W���o������position
const Position2 drawMassagePosition[MESSAGE_NUM] = {
	{200,580},                 //1 �ړ��W�����v
	{750,580},                 //2 ����t��
	{0,0},                 //3 ���[�v
	{0,0},                 //4 �X�e���X
	{0,0},                 //5 ���Ƃ�����
	{0,0},                 //6 �g�����X�A�C�e��(�H�p��)
	{0,0}                  //7 �S�[��
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

	bool padFlag;                       //�p�b�h���h�����Ă��邩����flag
	bool messageMoveCheck;              //���b�Z�[�W�̓��͎󗝂�����flag
public:
	Tutorial(Player& player,HitClass& h);
	~Tutorial();
	Rect MessageRect;

	void Updata(Input* input);
	void Draw(Position2 &offset);
	void CheckChainPosition();         //�v���C���[�̍��W�Ǝw����W����v���Ă邩���m�F����
	void Getclass(HitClass& h,Player& pl);	       //hit�N���X���󂯎�邽�߂̊֐�
	void RetryInit();                  //���g���C���ȂǂŌĂяo�����
	Rect &GetRect();
	TutoList_t& TutorialList();

	bool GetMessageFlag();
	bool DrawMessageBoxFlag;           //���b�Z�[�W�摜���o�Ă��邩�ǂ�����\��flag
	bool MessageNumFlag[MESSAGE_NUM];  //���b�Z�[�W�摜��flag��

};
