#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "GimPull.h"
#include "Player.h"
#include "Rope.h"
#include "HitClass.h"
#include "MapCtl.h"
#include "ModelMgr.h"

GimPull::GimPull(Position2 pos,Rope& r,Player& p):_rope(r),_pos(pos),_player(p)
{
	_hit = new HitClass();
	_modelmgr = ModelMgr::Instance();

	_map = MapCtl::GetInstance();
	_state = GM_NONE;
	_gmRect.w = 32*3;
	_gmRect.h = 32;
	count = 60;
	_initPos = _pos;
	_gimType = GIM_ATTRACT;
	modelhandle = _modelmgr->ModelIdReturn("floor_model/floor.pmx", SCENE_RESULT);

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
		if (_rope.GetRopeState() == ST_ROPE_SHRINKING&&_hit->IsHit(GetRect(), _rope.GetCircle())) {
			if (_rope.GetCircle().pos.x < _pos.x+ (_gmRect.w /2)) {		//左側
				_state = GM_HIT;
			}
			else {										//右側
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
		cout << _pos.x << endl;
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
	MV1SetRotationXYZ(modelhandle, VGet(0.0f,AngleRad(90.f), 0.0f));
	MV1SetPosition(modelhandle, ConvWorldPosToScreenPos(VGet(_pos.x - offset.x + (_gmRect.w / 2),_pos.y - offset.y + (_gmRect.h ), 0)));
	MV1SetScale(modelhandle, VGet(5.f, 5.f, 5.f));
	MV1DrawModel(modelhandle);
	_modelmgr->SetMaterialDotLine(modelhandle, 0.0f);
	if (_state != GM_END&& _state != GM_PAUSE) {			//ENDとPAUSE以外であれば色は同じまま
	//	DrawBox((int)(_pos.x - offset.x),(int)( _pos.y-offset.y),(int) (_pos.x -offset.x+ 32 * 3), (int)_pos.y - offset.y + 32, GetColor(0, 216, 140), true);
		//DrawBox((int)(_pos.x - offset.x), (int)(_pos.y - offset.y), (int)(_pos.x - offset.x + (32 * 3)), (int)_pos.y - offset.y + 32, GetColor(0, 216, 140), true);
	}
	else if (_state == GM_PAUSE) {
		//DrawBox((int)(_pos.x - offset.x), (int)(_pos.y - offset.y), (int)(_pos.x - offset.x + 32 * 3), (int)_pos.y - offset.y + 32, GetColor(0, 0, 255), true);
	}
	else if (_state == GM_END) {	//ENDになったら赤に変える
		//DrawBox((int)(_pos.x - offset.x), (int)(_pos.y - offset.y), (int)(_pos.x - offset.x + 32 * 3), (int)_pos.y - offset.y + 32, GetColor(255, 0, 0), true);
	}
	else {}
	//_gmRect.SetCenter(_pos.x - offset.x + (_gmRect.w / 2), _pos.y - offset.y + (_gmRect.h / 2));
	//_gmRect.Draw();	
	_gmRect.SetCenter(_pos.x + (_gmRect.w / 2), _pos.y + (_gmRect.h / 2));
#ifdef _DEBUG
	//_gmRect.Draw(offset);
	//DrawPixel(_pos.x - offset.x + (_gmRect.w / 2), _pos.y - offset.y + (_gmRect.h / 2), 0xffffff);
	//DrawPixel(_pos.x - offset.x, _pos.y - offset.y, 0xff00ff);
#endif
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