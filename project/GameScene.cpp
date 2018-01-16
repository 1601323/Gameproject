#include <DxLib.h>
#include <iostream>
#include <list>
#include <memory>
#include <map>
#include <vector>
#include "GameScene.h"
#include "GameMain.h"
#include "Input.h"
#include "Gimmick.h"
#include "MapCtl.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Object.h"
#include "Rope.h"
#include "Geometry.h"
#include "ResultScene.h"

#include "GimmickFactory.h"
#include "EnemyFactory.h"
#include "HitClass.h"
#include "EnemyServer.h"

GameScene::GameScene()
{
	_updater = &GameScene::NormalUpdata;
	_player = new Player();
	_rope = new Rope(_player);
	_server = new EnemyServer();
	_cam = Camera::GetInstance();
	// ϯ�߲ݽ�ݽ
	_map = MapCtl::GetInstance();
	// ϯ���ް��̓ǂݍ���
	_map->Load("map/1218_000.map");
	//_map->Load("map/1.map");
	//_fac = new GimmickFactory(player,rope);
	_fac = new GimmickFactory(*_player,*_rope);
	//// ���ޯĎw��
	_cam->SetTarget(_player);	// player�
	_cam->SetMapCtl(_map);		//Obj�p������Ȃ�Add��
	//�M�~�b�N�Ăяo���p�̊֐��ł��B
	//���̂悤�ɐ錾����Ƃǂ��ł��ݒu�ł���̂Ŋm�F���ɂ����Ă�������
	//_fac->Create(CHIP_TYPE::CHIP_DOOR, Position2(200,40));				//�Z���T�[�h�A
	//_fac->Create(CHIP_TYPE::CHIP_BUTTON_1,Position2(380,420));			//����������
	//_fac->Create(CHIP_TYPE::CHIP_ROPE_FALL,Position2(340, 300));			//���[�v�ňړ�������́i�������肷���j
	//_fac->Create(CHIP_TYPE::CHIP_ROPE_ATTRACT, Position2(32 * 15, 32 * 5));	//���[�v�ňړ����鑫��
	//�}�b�v��ǂݎ��A���X�g�ɃM�~�b�N���������܂��B
	auto gimData = _map->getChipPosData();
	for (auto& data : gimData) {
		if(CHIP_DOOR <= data.chipType && data.chipType < CHIP_MAX)
		_fac->Create(static_cast<CHIP_TYPE>(data.chipType),Position2(data.posX,data.posY));
	}
	//��а̧��ذ�ł��B�t�@�C�����ł���܂ł͒��ڎw��ɂȂ�܂�
	_emFac = new EnemyFactory(*_player, *_rope,*_server);
	_emFac->Create(ENEMY_TYPE::ENEMY_TURN, Position2(300, 416));
<<<<<<< HEAD
	//_emFac->Create(ENEMY_TYPE::ENEMY_WARKING, Position2(350, 130));
=======
	_emFac->Create(ENEMY_TYPE::ENEMY_WARKING, Position2(350, 130));
>>>>>>> bde5c3d0ac1b2c3ccf60ed9efac762e5bdc67085

	_hit = new HitClass(_fac,_emFac);

	//_server = new EnemyServer();

	//�t�@�N�g���[�̃��X�g�𗘗p����hit��Ԃ��܂�
	_rope->GetClass(_hit);
	_player->Getclass(_hit,_rope);

	GameInit();
	count = 0;
}

GameScene::~GameScene()
{
	delete _player;

	delete _rope;
	delete _fac;
	delete _emFac;
	delete _hit;	
	delete _server;
}
void GameScene::GameInit()
{
	//������Ԃ̃f�[�^������
	_rtData = RESULT_DATA();
}

void GameScene::NormalUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	_cam->Update();
	Position2& offset = _cam->ReturnOffset();
	_map->Draw(offset);
	//۰�ߎg�p���͓G�Ȃǂ��~�܂�
	if (_player->GetcharState() == ST_ROPE) {
		UsingRopeUpdata(input,offset);
	}
	else {
		ObjectUpdata(input,offset);
	}
	_server->Updata();

	Draw(offset);

	KEY key = input->GetInput(1).key;
	KEY lastKey = input->GetLastKey();
	INPUT_INFO inpInfo = input->GetInput(1);
#ifdef _DEBUG
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON)
	{
		//Enter�L�[ or A�{�^�� ����
		DrawString(200, 200, "idek", 0xfffff);
	}
#endif
	//�N���A�ɂ���ʑJ�ڂ�������
	if (_player->EnterDoor()) {
		_rtData.goalFlag = true;
		gm.SetResultData(_rtData);
		_updater = &GameScene::TransitionUpdata;
	}
		if (_player->GetcharState() == ST_DETH) {
			_rtData.goalFlag = false;
			gm.SetResultData(_rtData);
			_updater = &GameScene::TransitionUpdata;
	}
}
void GameScene::ObjectUpdata(Input* input,Position2& offset)
{
	_fac->Updata(*input);
	_rope->Updata(input,offset);
	_player->Update(input);
	_emFac->Updata();
	_cam->Update();
}
//���[�v���g���Ă���Ƃ��ɌĂяo�����
void GameScene::UsingRopeUpdata(Input* input,Position2& offset)
{	


	for (auto& gim : _fac->GimmickList()) {		//rope�ɍ��E�����M�~�b�N����Updata���Ăяo��
		if (gim->GetType() == GIM_FALL || gim->GetType() == GIM_ATTRACT) {
			gim->Updata(*input);
			//gim->Updata();					//�S�̓I�Ɋ��������悱����Ɉڍs
		}
	}
	_emFac->EnemyFalter();
	_rope->Updata(input,offset);
	_player->Update(input);

	_cam->Update();
}
//�N���A������A���U���g�ɑJ�ڂ��邽�߂�updata�ł�
void GameScene::TransitionUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	_cam->Update();
	Position2& offset = _cam->ReturnOffset();
	_map->Draw(offset);
	Draw(offset);
	count++;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,100+count);
	DrawBox(0,0,SCREEN_SIZE_X,SCREEN_SIZE_Y,0x000000,true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	if (count >= 120) {
		gm.Instance().ChangeScene(new ResultScene());
	}
}
void GameScene::Draw(Position2& offset)
{
	//���̂Ƃ���}�b�v��updata�ŕ\�������Ă���
	//_map->Draw();
	_fac->Draw(offset);
	_emFac->Draw(offset);
	_player->Draw(offset);
	_server->Draw(offset);
}
//�V�[���J�ڂ̂��߂ɗp��
SCENE_TYPE GameScene::GetScene()
{
	return SCENE_GAME;
}
void GameScene::Updata(Input* input) 
{
	(this->*_updater)(input);

}