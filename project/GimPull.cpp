#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "GimPull.h"
#include "Player.h"
#include "Rope.h"
#include "HitClass.h"
#include "MapCtl.h"
#include "ModelMgr.h"
#include "ImageMgr.h"
#include "Assert.h"

GimPull::GimPull(Position2 pos,Rope& r,Player& p):_rope(r),_pos(pos),_player(p)
{
	_hit = new HitClass();
	_modelmgr = ModelMgr::Instance();

	_map = MapCtl::GetInstance();
	sensordoorMotionFlag = false;
	_state = GM_NONE;
	_gmRect.w = 32*3;
	_gmRect.h = 32;
	count = 60;
	DownCnt = 0;
	_initPos = _pos;
	_gimType = GIM_ATTRACT;
	//モデル読み込み
	modelhandle = _modelmgr->ModelIdReturn("floor_model/pull floor_model/pull floor.pmx", SCENE_RESULT);
	//取っ手の部分
	handleHandle = _modelmgr->ModelIdReturn("floor_model/pull floor_model/handle.pmx", SCENE_RESULT);
	//テクスチャのindexを取得
	textureIndex = MV1GetMaterialDifMapTexture(modelhandle, 0);
}


GimPull::~GimPull()
{	
	delete _hit;
}

void GimPull::Updata(Input& _input) 
{
	//_gmRect.SetCenter(_pos.x+(_gmRect.w/2),_pos.y+(_gmRect.h/2));
	_key = _input.GetInput(1).key;
	_lastKey = _input.GetLastKey();
	_inpInfo = _input.GetInput(1);
	if (_state == GM_NONE || _state == GM_PAUSE) {
		CheckDoMove();
	}
	if (_state == GM_HIT||_state == GM_MOVE) {
		Move();
	}
	//Draw();
}

void GimPull::CheckDoMove() 
{
	if (_state != GM_END ) {
		////ロープとのあたり判定を取る
		//if (_rope.GetRopeState() == ST_ROPE_SHRINKING &&_hit->IsHit(GetRect(), _rope.GetCircle())) {
		//	//当たっていたら
		//	_state = GM_HIT;
		//	if (/*_rope.GetRopeState() == ST_ROPE_SHRINKING &&*/_rope.GetCircle().pos.x > _pos.x) {	//ﾛｰﾌﾟの場所が中心より右側であれば
		//		_state = GM_MOVE;
		//	}
		//}
		if (_rope.GetRopeState() == ST_ROPE_SHRINKING &&
			(_hit->IsHit(GetRect(), _rope.GetCircle() )|| (_hit->IsHit(GetRect(), _rope.GetCircle2())))) {
			//if (_rope.GetCircle().pos.x < _pos.x+ (_gmRect.w /2)) {		//左側
			if(_player.GetTmpDir() == DIR_RIGHT){
				_state = GM_HIT;
			}
			else if(_player.GetTmpDir() == DIR_LEFT){										//右側
				_state = GM_MOVE;
			}
		}
		//デバック用　ｴﾝﾀｰで作動
		//if (_inpInfo.key.keybit.A_BUTTON && !_lastKey.keybit.A_BUTTON) {
		//	_state = GM_HIT;
		//}
	}
}

void GimPull::Move()
{
	float inx = _rope.GetRopeVec().x;
	//行く先の壁を確認
	Position2 nextPos[2];
	//右
	nextPos[0].x = _pos.x + (_gmRect.w) + 15;
	nextPos[0].y = _pos.y;
	//左
	nextPos[1].x = _pos.x - 15;
	nextPos[1].y = _pos.y;

	Position2 nextPos2[2];
	//右
	nextPos2[0].x = _pos.x + (_gmRect.w) + inx;
	nextPos2[0].y = _pos.y;
	//左
	nextPos2[1].x = _pos.x + inx;
	nextPos2[1].y = _pos.y;
	//足元判定を追加
	Position2 downPos[2];
	//右下
	downPos[0].x = _pos.x + (_gmRect.w) - 20;
	downPos[0].y = _pos.y + (_gmRect.h) + 20;
	//左下
	downPos[1].x = _pos.x + 20;
	downPos[1].y = _pos.y + (_gmRect.h) + 20;
	//ﾛｰﾌﾟの移動量を見てそれに従い移動する予定
	//count -=abs(_rope.GetRopeVec().x);
	count -= 1;
	if (count > 0) {
	//	cout << _pos.x << endl;
		if (_state == GM_HIT) {			//ｴﾝﾀｰもしくはﾛｰﾌﾟの当たった場所が中心より左側
			if (_map->GetChipType(nextPos2[1]) != CHIP_BLANK &&_map->GetChipType(nextPos2[1]) != CHIP_ROPE_ATTRACT /*||(_map->GetChipType(downPos[0]) == CHIP_BLANK&& _map->GetChipType(downPos[1]) == CHIP_BLANK)*/) {
				_state = GM_END;
				count = 60;
			}
			else if (_hit->IsHit(_player.GetRect(), nextPos2[0]) || _hit->IsHit(_player.GetRect(), nextPos2[1]) ||_hit->IsHit(_player.GetRect(),GetRect())) {
				_state = GM_PAUSE;
				
			}
			else {
				_pos.x += _rope.GetRopeVec().x;
			}
		}
		else if (_state == GM_MOVE) {	//ropeの当たった場所が中心より右側
			if (_map->GetChipType(nextPos2[0]) != CHIP_BLANK &&_map->GetChipType(nextPos2[0]) != CHIP_ROPE_ATTRACT/* ||( _map->GetChipType(downPos[1]) == CHIP_BLANK && _map->GetChipType(downPos[1]) == CHIP_BLANK)*/) {
				_state = GM_END;
				count = 60;
			}
			else if (_hit->IsHit(_player.GetRect(), nextPos2[0])|| _hit->IsHit(_player.GetRect(), nextPos2[1]) || _hit->IsHit(_player.GetRect(), GetRect())) {
				_state = GM_PAUSE;
			}
			else {
				_pos.x += abs(_rope.GetRopeVec().x);
			}
		}
		else {
		}
	}
	else {		//設定した移動量だけ移動し終わったら状態をENDに変える
		_state = GM_END;
		count = 60;
	}
	//一定値以上動いたら強制的に動きを止める
	if (abs(_pos.x - _initPos.x) >= 100) {
		_state = GM_END;
	}
}

void GimPull::Draw(Position2 offset)
{
	//モデルの回転角度の設定(ラジアン)
	MV1SetRotationXYZ(modelhandle, VGet(0.0f,0.0f, 0.0f));
	MV1SetRotationXYZ(handleHandle, VGet(0.0f, 0.0f, 0.0f));

	//モデルのposを設定+ワールド座標からスクリーンへ変換
	MV1SetPosition(modelhandle, ConvWorldPosToScreenPos(VGet(_pos.x - offset.x + (_gmRect.w / 2),_pos.y - offset.y + (_gmRect.h ), 0)));
	if (_state != GM_END)
	{
		MV1SetPosition(handleHandle, ConvWorldPosToScreenPos(VGet(_pos.x - offset.x + (_gmRect.w / 2), _pos.y - offset.y + (_gmRect.h), 0)));
	}
	else {
		DownCnt+=5;
		//endになったら落ちるよ
		MV1SetPosition(handleHandle, ConvWorldPosToScreenPos(VGet(_pos.x - offset.x + (_gmRect.w / 2), min(_pos.y - offset.y + (_gmRect.h) + DownCnt ,SCREEN_SIZE_Y+50), 0)));
	}

	//モデルの拡大縮小値の設定
	MV1SetScale(modelhandle, VGet(5.5f, 7.0f, 5.5f));
	MV1SetScale(handleHandle, VGet(5.5f, 7.0f, 5.5f));

	//ステージごとにテクスチャを変更
	ChangeStageTexture();

	//モデルを輪郭線0.0fで描画 
	_modelmgr->Draw(modelhandle, 0.0f);
	_modelmgr->Draw(handleHandle, 0.0f);

	//_gmRect.SetCenter(_pos.x - offset.x + (_gmRect.w / 2), _pos.y - offset.y + (_gmRect.h / 2));
	//_gmRect.Draw();	
	_gmRect.SetCenter(_pos.x + (_gmRect.w / 2), _pos.y + (_gmRect.h / 2));
}
//あたり矩形を返す
Rect& GimPull::GetRect() 
{
	return _gmRect;
}

GIMMICK_TYPE& GimPull::GetType()
{
	return _gimType;
}
void GimPull::SetInit()
{
	_pos = _initPos;
	_state = GM_NONE;
}
void GimPull::ChangeStageTexture()
{
	ImageMgr& im = ImageMgr::Instance();
	GameMain& gm = GameMain::Instance();

	switch (gm.GetNowStage())
	{
		//初級
	case 0:
		MV1SetTextureGraphHandle(modelhandle, textureIndex, im.ImageIdReturn("floor_model/floor5.png", SCENE_TITLE), FALSE);
		break;
		//中級
	case 1:
		MV1SetTextureGraphHandle(modelhandle, textureIndex, im.ImageIdReturn("floor_model/floor6.png", SCENE_TITLE), FALSE);
		break;
		//上級
	case 2:
		MV1SetTextureGraphHandle(modelhandle, textureIndex, im.ImageIdReturn("floor_model/floor7.png", SCENE_TITLE), FALSE);
		break;
	default:
		ASSERT();
		break;
	}

	if (_state == GM_END)
	{
		MV1SetTextureGraphHandle(modelhandle, textureIndex, im.ImageIdReturn("floor_model/floorMax.png", SCENE_TITLE), FALSE);
	}
}