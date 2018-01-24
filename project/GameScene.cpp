#include <DxLib.h>
#include <iostream>
#include <stdio.h>
#include <list>
#include <memory>
#include <map>
#include <vector>
#include "Geometry.h"
#include "Assert.h"
#include "ImageMgr.h"
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

#include "ResultScene.h"

#include "GimmickFactory.h"
#include "EnemyFactory.h"
#include "HitClass.h"
#include "EnemyServer.h"
#include "Midpoint.h"

#include "ModelMgr.h"
#include "ImageMgr.h"

#include "TimeManager.h"


GameScene::GameScene()
{
	ImageMgr& im = ImageMgr::Instance();
	_updater = &GameScene::FadeInUpdata;
	GameInit();
	_player = new Player();
	_rope = new Rope(_player);
	_server = new EnemyServer();
	_mid = new Midpoint();
	_timer = new TimeManager();
	_cam = Camera::GetInstance();
	// ϯ�߲ݽ�ݽ
	_map = MapCtl::GetInstance();
	// ϯ���ް��̓ǂݍ���
	//_map->Load("map/1218_001.map");
	_map->Load(mapName);
	_hit = new HitClass();

	//_map->Load("map/1.map");
	//_fac = new GimmickFactory(player,rope);
	_fac = new GimmickFactory(*_player, *_rope);
	//// ���ޯĎw��
	_cam->SetTarget(_player);	// player�
	_cam->SetMapCtl(_map);		//Obj�p������Ȃ�Add��
	//�M�~�b�N�Ăяo���p�̊֐��ł��B
	//���̂悤�ɐ錾����Ƃǂ��ł��ݒu�ł���̂Ŋm�F���ɂ����Ă�������
	//_fac->Create(CHIP_TYPE::CHIP_ROPE_FALL,Position2(340, 300));			//���[�v�ňړ�������́i�������肷���j
	//_fac->Create(CHIP_TYPE::CHIP_ROPE_ATTRACT, Position2(32 * 15, 32 * 5));	//���[�v�ňړ����鑫��
	//_fac->Create(CHIP_TYPE::CHIP_ROPE_ATTRACT, Position2(32 * 15, 32 * 5));	//���[�v�ňړ����鑫��
	//�}�b�v��ǂݎ��A���X�g�ɃM�~�b�N���������܂��B
	auto gimData = _map->getChipPosData();
	for (auto& data : gimData) {
		if (CHIP_DOOR <= data.chipType && data.chipType < CHIP_MAX)
			_fac->Create(static_cast<CHIP_TYPE>(data.chipType), Position2(data.posX, data.posY));
		if (data.chipType == CHIP_DOOR) {
			_player->SetInitPos(Position2(data.posX, data.posY));
		}
	}
	_hit->GetClass(_fac);
	//��а̧��ذ�ł��B�t�@�C�����ł���܂ł͒��ڎw��ɂȂ�܂�
	_emFac = new EnemyFactory(*_player, *_rope, *_server, * _hit);
	//_emFac->Create(ENEMY_TYPE::ENEMY_TURN, Position2(300, 450));
	_emFac->Create(ENEMY_TYPE::ENEMY_WARKING, Position2(350, 230));
	//_emFac->Create(ENEMY_TYPE::ENEMY_WARKING, Position2(350, 450));


	_hit->GetClass(_emFac);
	//_hit = new HitClass(_fac, _emFac);

	//_server = new EnemyServer();
	//�t�@�N�g���[�̃��X�g�𗘗p����hit��Ԃ��܂�
	_rope->GetClass(_hit);
	_player->Getclass(_hit, _rope);
	_mid->GetClass(_player);
	_timer->StartTimer();
	//GameInit();
	count = 0;
}
GameScene::~GameScene()
{
	delete _player;
	delete _timer;
	delete _mid;
	delete _rope;
	delete _fac;
	delete _emFac;
	delete _hit;
	delete _server;
}
void GameScene::GameInit()
{
	GameMain& gm = GameMain::Instance();
	//������Ԃ̃f�[�^������
	_rtData = RESULT_DATA();
	switch (gm.GetNowStage()) {
	case 0:
		mapName = "map/119.map";
		break;
	case 1:
		mapName = "map/map2.map";
		break;
	case 2:
		mapName = "map/3.map";
		break;
	default:
		ASSERT();
		break;
	}
}
void GameScene::FadeInUpdata(Input* input) 
{
	GameMain& gm = GameMain::Instance();
	KEY key = input->GetInput(1).key;
	KEY lastKey = input->GetLastKey();
	INPUT_INFO inpInfo = input->GetInput(1);
	_cam->Update();
	Position2& offset = _cam->ReturnOffset();
	DrawBack(offset);
	_map->Draw(offset);
	Draw(offset);
	DrawUI();
	count++;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220 - (count*2));
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (count >= 60) {
		_updater = &GameScene::NormalUpdata;
		count = 0;
	}
}
void GameScene::NormalUpdata(Input* input)
{
	UpdateManager();
	_cam->Update();
	Position2& offset = _cam->ReturnOffset();
	DrawBack(offset);

	_map->Draw(offset);
	//۰�ߎg�p���͓G�Ȃǂ��~�܂�
	if (_player->GetcharState() == ST_ROPE) {
		UsingRopeUpdata(input, offset);
	}
	else {
		ObjectUpdata(input, offset);
	}
	_server->Updata();

	Draw(offset);
	_timer->Updata();
	DrawUI();
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

	if (key.keybit.START_BUTTON && !lastKey.keybit.START_BUTTON) {
		_updater = &GameScene::PauseUpdata;
	}
	JudgeTransition();
}
void GameScene::JudgeTransition()
{
	GameMain& gm = GameMain::Instance();
	//�N���A�ɂ���ʑJ�ڂ�������
	if (_mid->ReturnGetFlag() == true) {
		_rtData.midFlag = true;
	}
	else
	{
		_rtData.midFlag = false;
	}
	if (_player->EnterDoor()) {
		_timer->StopTimer();
		_rtData.goalFlag = true;
		_rtData.goalTime = _timer->ShowTimer();
		gm.SetResultData(_rtData);
		_updater = &GameScene::TransitionUpdata;
	}
	if (_player->GetcharState() == ST_DETH) {
		_rtData.life--;
		gm.SetResultData(_rtData);
		_updater = &GameScene::TransitionUpdata;
	}
	
}
void GameScene::ObjectUpdata(Input* input, Position2& offset)
{
	//_cam->Update();
	_fac->Updata(*input);
	_rope->Updata(input, offset);
	_player->Update(input);
	_emFac->Updata();
	_mid->Updata();
}
//���[�v���g���Ă���Ƃ��ɌĂяo�����
void GameScene::UsingRopeUpdata(Input* input, Position2& offset)
{
	//_cam->Update();
	for (auto& gim : _fac->GimmickList()) {		//rope�ɍ��E�����M�~�b�N����Updata���Ăяo��
		if (gim->GetType() == GIM_FALL || gim->GetType() == GIM_ATTRACT) {
			gim->Updata(*input);
			gim->Updata();					//�S�̓I�Ɋ��������悱����Ɉڍs
		}
	}
	_emFac->EnemyFalter();
	_rope->Updata(input, offset);
	_player->Update(input);
}
//�N���A������A���U���g�ɑJ�ڂ��邽�߂�updata�ł�
void GameScene::TransitionUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	//_cam->Update();
	Position2& offset = _cam->ReturnOffset();
	DrawBack(offset);
	_map->Draw(offset);
	Draw(offset);
	DrawUI();
	count++;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100 + count);
	DrawBox(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (count >= 120) {
		if (_rtData.goalFlag == true) {
			gm.Instance().ChangeScene(new ResultScene());
		}
		else if (gm.GetResultData().life >= 0) {
			RetryProcess();
			_updater = &GameScene::FadeInUpdata;
		}
		else {
			gm.Instance().ChangeScene(new ResultScene());
		}
		count = 0;
	}
}
void GameScene::PauseUpdata(Input* input) 
{
	GameMain& gm = GameMain::Instance();
	//_cam->Update();
	Position2& offset = _cam->ReturnOffset();
	DrawBack(offset);
	_map->Draw(offset);
	Draw(offset);
	DrawUI();
#ifdef _DEBUG
	DrawBox(300,100,500,300,0xabcdef,true);
	DrawString(320,150,"PAUSE",0xffffff);
#endif
	KEY key = input->GetInput(1).key;
	KEY lastKey = input->GetLastKey();
	INPUT_INFO inpInfo = input->GetInput(1);

	if (key.keybit.START_BUTTON && !lastKey.keybit.START_BUTTON) {
		_updater = &GameScene::NormalUpdata;
	}
}
void GameScene::RetryProcess()
{
	if (_rtData.midFlag == true) {
		_player->SetRetryPos(_mid->GetInitPos());
	}
	else
	{
		_player->SetInitPos();
	}
	_mid->Updata();
	for (auto& em : _emFac->EnemyList()) {
		em->SetInitPos();
	}
	_server->ServerInit();
}
//���̐�܂Ƃ߂邩������Ȃ��̂ŉ��ݒu
void GameScene::UpdateManager()
{

}
void GameScene::Draw(Position2& offset)
{
	//���̂Ƃ���}�b�v��updata�ŕ\�������Ă���
	//_map->Draw();
	_fac->Draw(offset);
	_emFac->Draw(offset);
	_player->Draw(offset);
	_server->Draw(offset);
	_mid->Draw(offset);
}
void GameScene::DrawUI()
{
	ImageMgr& im = ImageMgr::Instance();
	GameMain& gm = GameMain::Instance();

	for (int f = 0; f < gm.GetResultData().life; f++) {
		DrawGraph(20 + 25 * f, 30, im.ImageIdReturn("��image/UI/UI_life.png", SCENE_RESULT),true);
	}
}
//�w�i�`��
void GameScene::DrawBack(Position2 offset)
{
	//�܂����d�X�N���[���͂��Ȃ��ł�
	ImageMgr& im = ImageMgr::Instance();
	DrawGraph(0-offset.x,0-offset.y,im.ImageIdReturn("��image/Game/backOmage.png",SCENE_RESULT),true);
	DrawGraph(0 - offset.x, 0 - offset.y, im.ImageIdReturn("��image/Game/back3.png", SCENE_RESULT), true);
	DrawGraph(0 - offset.x, 0 - offset.y, im.ImageIdReturn("��image/Game/back2.png", SCENE_RESULT), true);
	DrawGraph(0 - offset.x, 0 - offset.y, im.ImageIdReturn("��image/Game/back1.png", SCENE_RESULT), true);

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