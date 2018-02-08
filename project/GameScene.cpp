#include <DxLib.h>
#include <iostream>
#include <stdio.h>
#include <list>
#include <memory>
#include <map>
#include <vector>
#include "Geometry.h"
#include "Object.h"
#include "Assert.h"
#include "ImageMgr.h"
#include "GameMain.h"
#include "Input.h"
#include "Gimmick.h"
#include "MapCtl.h"
#include "Player.h"
#include "Enemy.h"
#include "Rope.h"

#include "ResultScene.h"
#include "SoundMgr.h"
#include "SelectScene.h"

#include "GimmickFactory.h"
#include "EnemyFactory.h"
#include "HitClass.h"
#include "EnemyServer.h"
#include "Midpoint.h"

#include "ModelMgr.h"
#include "ImageMgr.h"

#include "TimeManager.h"
#include "Camera.h"

#include "GameScene.h"

#define COLOR_NUM_TIME (40)

GameScene::GameScene()
{
	ImageMgr& im = ImageMgr::Instance();
	_updater = &GameScene::FadeInUpdata;
	bgmFlag = false;
	GameInit();
	_player = new Player();
	_rope = new Rope(_player);
	_server = new EnemyServer();
	_mid = new Midpoint();
	_timer = new TimeManager();
	// ﾏｯﾌﾟｲﾝｽﾀﾝｽ
	_map = MapCtl::GetInstance();
	// ﾏｯﾌﾟﾃﾞｰﾀの読み込み
	//_map->Load("map/1218_001.map");
	_map->Load(mapName);
	auto chipData = _map->getChipPosData();
	_hit = new HitClass();

	//_map->Load("map/1.map");
	//_fac = new GimmickFactory(player,rope);
	_fac = new GimmickFactory(*_player, *_rope);
	//// ﾀｰｹﾞｯﾄ指定
	//_cam->SetTarget(_player);	// player基準
	//_cam->SetMapCtl(_map);		//Obj継承するならAddで
	//ギミック呼び出し用の関数です。
	//このように宣言するとどこでも設置できるので確認等につかってください
	//_fac->Create(CHIP_TYPE::CHIP_ROPE_FALL,Position2(340, 300));			//ロープで移動するもの（落ちたりするやつ）
	//_fac->Create(CHIP_TYPE::CHIP_ROPE_ATTRACT, Position2(32 * 15, 32 * 5));	//ロープで移動する足場
	//_fac->Create(CHIP_TYPE::CHIP_ROPE_ATTRACT, Position2(32 * 15, 32 * 5));	//ロープで移動する足場
	//マップを読み取り、リストにギミックを持たせます。
	//auto gimData = _map->getChipPosData();
	for (auto& data : chipData) {
		if (CHIP_DOOR <= data.chipType && data.chipType < CHIP_PLAYER_POS)
			_fac->Create(static_cast<CHIP_TYPE>(data.chipType), Position2(data.posX, data.posY));
		//if (data.chipType == CHIP_DOOR) {
		//	_player->SetInitPos(Position2(data.posX, data.posY));
		//}
	}
	_hit->GetClass(_fac);
	//ｴﾈﾐｰﾌｧｸﾄﾘｰです。ファイルができるまでは直接指定になります
	_emFac = new EnemyFactory(*_player, *_rope, *_server, * _hit);
	//_emFac->Create(ENEMY_TYPE::ENEMY_TURN, Position2(300, 450));
	//_emFac->Create(ENEMY_TYPE::ENEMY_WARKING, Position2(350, 230));
	//_emFac->Create(ENEMY_TYPE::ENEMY_WARKING, Position2(350, 450));
	for (auto& data : chipData) {
		//ﾌﾟﾚｲﾔｰの場所設定
		if (data.chipType == CHIP_ENEMY_AROUND || data.chipType == CHIP_ENEMY_LOOK) {
			_emFac->Create(static_cast<ENEMY_TYPE>(data.chipType),Position2(data.posX,data.posY));
		}
	}
	//その他チップから必要なデータを読み込みます
	for (auto& other : chipData) {
		if (other.chipType == CHIP_PLAYER_POS) {
			_player->SetInitPos(Position2(other.posX,other.posY));
		}
		if (other.chipType == CHIP_MID_KEY || other.chipType == CHIP_MID_SAFE) {
			_mid->SetInitPos(static_cast<CHIP_TYPE>(other.chipType),Position2(other.posX,other.posY));
		}
	}
	_hit->GetClass(_emFac);
	//_hit = new HitClass(_fac, _emFac);

	//_server = new EnemyServer();
	//ファクトリーのリストを利用したhitを返します
	_rope->GetClass(_hit);
	_player->Getclass(_hit, _rope);
	_mid->GetClass(_player);
	_timer->StartTimer();
	//GameInit();	
	_cam = Camera::GetInstance();

	_cam->SetTarget(_player);	// player基準
	_cam->SetMapCtl(_map);		//Obj継承するならAddで

	count = 0;
	pauseNowNum = 0;
	pauseRetireNowNum = 1;
	pauseDirNumY = 0;
	pauseDirNumX = 0;
	dirMoveCnt = 0;
	selectPauseFlag = false;
	ChackFlag = false;
	ChackDrawFlag = false;
	RetryOrRetireFlag = false;


	_minSensingValueL = SV_HIGH;
	//numberImage = im.ImageIdReturn("image/UI/NewNum.png",SCENE_RESULT);
	lightImage = im.ImageIdReturn("image/UI/Patrite2.png", SCENE_RESULT);
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
	SoundMgr& so = SoundMgr::Instance();

	//初期状態のデータを入れる
	_rtData = RESULT_DATA();
	gm.SetResultData(_rtData);
	_feverData = FEVER_DATA();
	gm.SetFeverData(_feverData);
	so.BgmStart("Bgm/game4.mp3", SCENE_GAME);
	bgmFlag = true;
	switch (gm.GetNowStage()) {
	case 0:
		mapName = "map/tutorial.map";
		break;
	case 1:
		mapName = "map/normal.map";
		break;
	case 2:
		mapName = "map/hard.map";
		break;
	default:
		ASSERT();
		break;
	}
}
void GameScene::FadeInUpdata(Input* input) 
{
	SoundMgr& so = SoundMgr::Instance();
	GameMain& gm = GameMain::Instance();	
	if (bgmFlag == false) {
		so.BgmStart("Bgm/game4.mp3", SCENE_GAME);
		bgmFlag = true;
	}

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
	GameMain& gm = GameMain::Instance();
	SoundMgr& so = SoundMgr::Instance();
	UpdateManager();
 	_cam->Update();
	Position2& offset = _cam->ReturnOffset();
	DrawBack(offset);

	_map->Draw(offset);


	//ﾛｰﾌﾟ使用中は敵などが止まる
	if (_player->GetStateRope() == true) {
		UsingRopeUpdata(input, offset);
	}
	else {

		ObjectUpdata(input, offset);
	}
	_server->SetMidFlag(_rtData.midFlag);
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
		//Enterキー or Aボタン 処理
		//DrawString(200, 200, "idek", 0xfffff);
	}
#endif

	if (key.keybit.START_BUTTON && !lastKey.keybit.START_BUTTON) {
		_updater = &GameScene::PauseUpdata;
		so.ChangeSound(100);
	}
	JudgeTransition();
}
void GameScene::JudgeTransition()
{
	GameMain& gm = GameMain::Instance();
	//クリアによる画面遷移を仮実装
	if (_mid->ReturnGetFlag() == true) {
		_rtData.midFlag = true;
		gm.SetResultData(_rtData);
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
		for (auto& gim : _fac->GimmickList()) {
			gim->sensordoorMotionFlag = false;
		}
		_updater = &GameScene::TransitionUpdata;
	}
	if (_player->GetcharState() == ST_DETH || _player->GetcharState() == ST_OVER) {
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
	_mid->Updata(input);
}
//ロープを使っているときに呼び出される
void GameScene::UsingRopeUpdata(Input* input, Position2& offset)
{
	//_cam->Update();
	for (auto& gim : _fac->GimmickList()) {		//ropeに左右されるギミックだけUpdataを呼び出す
		if (gim->GetType() == GIM_FALL || gim->GetType() == GIM_ATTRACT) {
			gim->Updata(*input);
			gim->Updata();					//全体的に完成し次第こちらに移行
		}
	}
	_emFac->EnemyFalter();
	_rope->Updata(input, offset);
	_player->Update(input);
}
//クリアした後、リザルトに遷移するためのupdataです
void GameScene::TransitionUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	SoundMgr& so = SoundMgr::Instance();
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
			so.BgmFadeOut("Bgm/game4.mp3", SCENE_GAME);
			bgmFlag = false;
		}
		else if (gm.GetResultData().life > 0) {
			//リトライ

			RetryProcess();
			_updater = &GameScene::FadeInUpdata;
		}
		else{
			gm.Instance().ChangeScene(new ResultScene());
			so.BgmFadeOut("Bgm/game4.mp3", SCENE_GAME);
			bgmFlag = false;
		}
		count = 0;
	}
	so.BgmFadeOut("Bgm/game4.mp3", SCENE_GAME);
	bgmFlag = false;
}
//ポーズ用updata
void GameScene::PauseUpdata(Input* input) 
{
	dirMoveCnt++;
	dirMoveCnt %= COLOR_NUM_TIME;

	GameMain& gm = GameMain::Instance();
	SoundMgr& so = SoundMgr::Instance();
	_cam->Update();
	Position2& offset = _cam->ReturnOffset();
	DrawBack(offset);
	_map->Draw(offset);
	Draw(offset);
	DrawUI();
	DrawPauseUi();
#ifdef _DEBUG
	//DrawBox(300,100,500,300,0xabcdef,true);
	//DrawString(320,150,"PAUSE",0xffffff);
#endif
	KEY key = input->GetInput(1).key;
	KEY lastKey = input->GetLastKey();
	INPUT_INFO inpInfo = input->GetInput(1);

	PauseSelect(input);

	//ポーズ解除
	if (key.keybit.START_BUTTON && !lastKey.keybit.START_BUTTON)
	{
		dirMoveCnt = 0;
		ChackDrawFlag = false;
		ChackFlag = false;
		so.ChangeSound();
		_updater = &GameScene::NormalUpdata;
	}

	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		switch (pauseNowNum)
		{
			//最初からやり直す
		case MODE_RETRY:
			//ただ戻るだけでなく1回確認を入れる
			RetryOrRetireFlag = true;
			ChackDrawFlag = true;
			break;
			//ステージセレクトに戻る
		case MODE_SELECT:
			RetryOrRetireFlag = false;
			ChackDrawFlag = true;
			break;
		default:
			break;
		}
		//_updater = &GameScene::NormalUpdata;
	}
	if (ChackDrawFlag)
	{
		DrawCheckUi();//ui表示
		CheckReTireSelect(input);//入力関連

		if ((key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) && ChackFlag)
		{
			switch (pauseRetireNowNum)
			{
			case 0:
				dirMoveCnt = 0;
				ChackDrawFlag = false;
				ChackFlag = false;
				//flagをみてリトライ、リタイアに分ける
				if (RetryOrRetireFlag)
				{
					RetryPauseProcess();
					_updater = &GameScene::FadeInUpdata;
				}
				else {
					gm.Instance().ChangeScene(new SelectScene());
				}
				break;
			case 1:
				dirMoveCnt = 0;
				ChackDrawFlag = false;
				ChackFlag = false;
				//再起
				_updater = &GameScene::PauseUpdata;
				break;
			default:
				ChackDrawFlag = false;
				ChackFlag = false;
				break;
			}
		}
	}
	
}

//pauseの選ばれている処理
void GameScene::PauseSelect(Input* input)
{
	KEY key = input->GetInput(1).key;
	KEY lastKey = input->GetLastKey();
	INPUT_INFO inpInfo = input->GetInput(1);

	//if (pauseNowNum <= 0)pauseNowNum = 1;

	if (inpInfo.num >= 1) {
		if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
			inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL &&
			selectPauseFlag == false)
		{
			pauseNowNum--;
			if (pauseNowNum <= 0) {
				pauseNowNum = MODE_MAX - 1;
			}
			selectPauseFlag = true;
		}
		else if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
			inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL &&
			selectPauseFlag == false)
		{
			pauseNowNum++;
			if (pauseNowNum >= MODE_MAX) {
				pauseNowNum = 0;
			}
			selectPauseFlag = true;
		}
		else if (!((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
			inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) &&
			!((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL)) {
			selectPauseFlag = false;
		}
		else {
			pauseNowNum = pauseNowNum;
		}
	}
	else {

		//ステージ選択
		if (inpInfo.key.keybit.R_UP_BUTTON && !lastKey.keybit.R_UP_BUTTON) {
			pauseNowNum--;
			if (pauseNowNum < 0) {
				pauseNowNum = MODE_MAX - 1;
			}
		}
		else if (inpInfo.key.keybit.R_DOWN_BUTTON && !lastKey.keybit.R_DOWN_BUTTON) {
			pauseNowNum++;
			if (pauseNowNum >= MODE_MAX) {
				pauseNowNum = 0;
			}
		}
		else {
		}
	}
}

//リトライ時の初期化呼び出しをまとめたもの
void GameScene::RetryProcess()
{
	if (_mid->ReturnCheckFlag() || _mid->ReturnGetFlag()/*_rtData.midFlag == true*/) {
		//ポースからの場合必ず初期関数に入る
		_player->SetRetryPos(_mid->GetInitPos());
	}
	else
	{
		//ポースからの場合必ず初期関数に入る
		 _player->SetInitPos();
	}
	_mid->Updata();
	for (auto& em : _emFac->EnemyList()) {
		em->SetInitPos();
	}
	_server->ServerInit();
}
//この先まとめるかもしれないので仮設置
void GameScene::UpdateManager()
{

}

//リタイア時確認確認用カーソル移動処理
void  GameScene::CheckReTireSelect(Input* input)
{
	KEY key = input->GetInput(1).key;
	KEY lastKey = input->GetLastKey();
	INPUT_INFO inpInfo = input->GetInput(1);

	if (inpInfo.num >= 1) {
		if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_RIGHT) &&
			inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL)
		{
			pauseRetireNowNum--;
			if (pauseRetireNowNum <= 0) {
				pauseRetireNowNum = 1;
			}
			ChackFlag = true;
		}
		else if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_LEFT) &&
			inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL )
		{
			pauseRetireNowNum++;
			if (pauseRetireNowNum >= 2) {
				pauseRetireNowNum = 0;
			}
			ChackFlag = true;

		}
		else if (!((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_RIGHT) &&
			inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) &&
			!((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_LEFT) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL)) {
		}
		else {
			pauseRetireNowNum = pauseRetireNowNum;
		}
	}
	else {

		//ステージ選択
		if (inpInfo.key.keybit.R_RIGHT_BUTTON && !lastKey.keybit.R_RIGHT_BUTTON) {
			pauseRetireNowNum--;
			if (pauseRetireNowNum < 0) {
				pauseRetireNowNum = 1;
			}
			ChackFlag = true;

		}
		else if (inpInfo.key.keybit.R_LEFT_BUTTON && !lastKey.keybit.R_LEFT_BUTTON) {
			pauseRetireNowNum++;
			if (pauseRetireNowNum >= MODE_MAX) {
				pauseRetireNowNum = 0;
			}
			ChackFlag = true;

		}
		else {

		}
	}

}

void GameScene::Draw(Position2& offset)
{
	GameMain& gm = GameMain::Instance();

	//今のところマップはupdataで表示させておく
	//_map->Draw();
	_fac->Draw(offset);
	_emFac->Draw(offset);
	//ドアが開ききってから描画
	for (auto& gim : _fac->GimmickList())
	{
		if (gim->sensordoorMotionFlag && !gm.GetResultData().goalFlag)
		{
			_player->Draw(offset);
		}
	}
	//_player->Draw(offset);
	_server->Draw(offset);
	_mid->Draw(offset);
}
void GameScene::DrawUI()
{
	ImageMgr& im = ImageMgr::Instance();
	GameMain& gm = GameMain::Instance();

	for (int f = 0; f < gm.GetResultData().life; f++) {
		DrawGraph(20 + 25 * f, 30, im.ImageIdReturn("image/UI/UI_life.png", SCENE_RESULT),true);
	}
	for (int f = 0; f < gm.ReturnFeverData().feverCnt; f++) {
		DrawGraph(20+25*f,70,im.ImageIdReturn("image/UI/Bread.png",SCENE_RESULT),true);
	}
	//場所は完全には決定していない
	//色変化を実装
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	SetDrawBright(_server->ReturnColor().red, _server->ReturnColor().green, _server->ReturnColor().blue);
	DrawExtendGraph(700, 0, 770, 85, lightImage, true);
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawExtendGraph(700, 0, 770, 85, im.ImageIdReturn("image/UI/Patrite1.png", SCENE_RESULT), true);

	//_timer->Draw();
}

//ポース用のuiを表示
void GameScene::DrawPauseUi(void)
{
	ImageMgr& im = ImageMgr::Instance();
	//ボード
	DrawExtendGraph(200, 50, 620,370,im.ImageIdReturn("image/Pause/Board.png", SCENE_RESULT), true);

	DrawGraph(300, 130, im.ImageIdReturn("image/Restart/Retry.png", SCENE_RESULT), true);
	DrawGraph(300, 220, im.ImageIdReturn("image/Pause/End.png", SCENE_RESULT), true);

	switch (pauseNowNum) {
	case 0:
		pauseDirNumY = 120;
		break;
	case 1:
		pauseDirNumY = 210;
		break;
	default:
		break;
	}

	DrawGraph(210 + abs(30 - (200 + (dirMoveCnt / 2 % 60)) % 20), pauseDirNumY, im.ImageIdReturn("image/yazirushi2.png", SCENE_RESULT), true);

}

//リタイア確認用のUI表示
void GameScene::DrawCheckUi()
{
	ImageMgr& im = ImageMgr::Instance();
	//ボードの上にボードを置く
	DrawExtendGraph(70, 80, 740, 470, im.ImageIdReturn("image/Pause/Board.png", SCENE_RESULT), true);
	if (RetryOrRetireFlag)
	{
		//リトライするか?
		DrawGraph(150, 130, im.ImageIdReturn("image/Restart/RetryQuestion.png", SCENE_RESULT), true);
	}
	else {
		//リタイアするか?
		DrawGraph(150, 130, im.ImageIdReturn("image/Pause/EndQuestion.png", SCENE_RESULT), true);
	}
	//はいいいえ
	DrawGraph(220, 350, im.ImageIdReturn("image/Restart/Yes.png", SCENE_RESULT), true);
	DrawGraph(460, 350, im.ImageIdReturn("image/Restart/No.png", SCENE_RESULT), true);

	switch (pauseRetireNowNum) {
	case 0:
		pauseDirNumX = 185;
		break;
	case 1:
		pauseDirNumX = 455;
		break;
	default:
		break;
	}

	SetDrawBright(243, 152, 0);
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255 * (static_cast<float>(abs(20 - dirMoveCnt)) / static_cast<float>((COLOR_NUM / 1.5f))));
	DrawGraph(pauseDirNumX, 340, im.ImageIdReturn("image/restart/Frame.png", SCENE_RESULT), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetDrawBright(255, 255, 255);

}

//背景描画
void GameScene::DrawBack(Position2 offset)
{
	//まだ多重スクロールはしないです

	ImageMgr& im = ImageMgr::Instance();
	//DrawGraph(0-offset.x,0-offset.y,im.ImageIdReturn("image/Game/title5.png",SCENE_RESULT),true);
	//DrawExtendGraph(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, im.ImageIdReturn("image/Game/title5.png", SCENE_RESULT), true);
	//DrawExtendGraph(0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y, im.ImageIdReturn("image/Game/j.bmp", SCENE_RESULT), true);
	DrawGraph(min(0 - offset.x*0.2,0),min( 0 - offset.y,0), im.ImageIdReturn("image/Game/haikkk.png",SCENE_RESULT),true);
	DrawGraph(min(0, -offset.x*0.5), min(0, -offset.y), im.ImageIdReturn("image/Game/hai.png", SCENE_RESULT), true);
	//DrawGraph(0 - offset.x, 0 - offset.y, im.ImageIdReturn("image/Game/back3.png", SCENE_RESULT), true);
	//DrawGraph(0 - offset.x, 0 - offset.y, im.ImageIdReturn("image/Game/back2.png", SCENE_RESULT), true);
	//DrawGraph(0 - offset.x, 0 - offset.y, im.ImageIdReturn("image/Game/back1.png", SCENE_RESULT), true);

}

void GameScene::RetryPauseProcess()
{
	GameMain& gm = GameMain::Instance();
	_rtData = RESULT_DATA();
	gm.SetResultData(_rtData);
	_feverData = FEVER_DATA();
	gm.SetFeverData(_feverData);
	_player->SetInitPausePos();

	_mid->Updata();
	for (auto& em : _emFac->EnemyList()) {
		em->SetInitPos();
	}
	for (auto& gim : _fac->GimmickList()) {
		gim->SetInit();
	}
	_server->ServerInit();
}

//シーン遷移のために用意
SCENE_TYPE GameScene::GetScene()
{
	return SCENE_GAME;
}
void GameScene::Updata(Input* input)
{
	(this->*_updater)(input);

}