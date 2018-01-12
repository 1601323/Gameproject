#include <DxLib.h>
#include "GimDrop.h"
#include "HitClass.h"
#include "Input.h"
#include "Rope.h"
#include "Player.h"
#include "MapCtl.h"



GimDrop::GimDrop(Position2 pos,Rope& r,Player& p):_rope(r),_player(p)
{
	_hit = new HitClass();
	_map = MapCtl::GetInstance();
	_state = GM_NONE;
	gravity = 0.9;
	velocity = 0;
	_gmRect.h = 23;
	_gmRect.w = 23;
	_pos.x = pos.x + (MAP_CHIP_SIZE_X/2);
	_pos.y = pos.y+MAP_CHIP_SIZE_Y - (_gmRect.h / 2);
	dropFlag = false;
	count = 60;

	_gimType = GIM_FALL;
}


GimDrop::~GimDrop()
{
	delete _hit;
}

void GimDrop::Updata(Input& _input)
{
	//_gmRect.SetCenter(_pos.x , _pos.y);

	_key = _input.GetInput(1).key;
	_lastKey = _input.GetLastKey();
	_inpInfo = _input.GetInput(1);
	CheckDoMove();
	if (_state == GM_HIT) {
		MoveLeft();
	}
	else if (_state == GM_MOVE) {
		MoveRight();
	}
	else {

	}
	//Draw();	
}
//_stateの変更について
void GimDrop::CheckDoMove()
{
	//if (_state == GM_NONE ||_state== GM_PAUSE) {	//状態が一度しか切り替わらないようにする
	if(_state != GM_HIT && _state != GM_MOVE){							//動いている途中でも当てたら方向転換ができるようになってます
		////ロープとのあたり判定を取る
		if (_rope.GetRopeState()== ST_ROPE_SHRINKING &&_hit->IsHit(GetRect(), _rope.GetCircle())) {
			if (_rope.GetCircle().pos.x < _pos.x) {	//ropeのﾚｸﾄのXが物体のXより大きい
				_state = GM_HIT;		//左側に動く
			}
			else {
				_state = GM_MOVE;		//右側に動く
			}
		}	
		//デバック用　ｴﾝﾀｰで作動（移動量を設定しないと頭おかしい挙動をします）
		//if (_inpInfo.key.keybit.A_BUTTON && !_lastKey.keybit.A_BUTTON) {
		//	//動き始める
		//	_state = GM_HIT;
		//}
	}
}

void GimDrop::MoveLeft()
{
	//行く先の床があるかどうかを調べる
	Position2 nextPos = _pos;
	nextPos.x = _pos.x + (_gmRect.h / 2);
	nextPos.y = _pos.y + (_gmRect.w);
	//落下の際のために反対側も判定しておく
	Position2 nextPos1 = _pos;
	nextPos1.x = _pos.x - (_gmRect.w / 2);
	nextPos1.y = _pos.y + (_gmRect.h);
	//正面の行く先
	//とりあえず左の行く先だけ
	Position2 flontPos = _pos;
	flontPos.x = _pos.x - (_gmRect.w / 2);
	//ropeの方向がまっすぐだったら
	if (_rope._ropeDir == ROPE_DIR_STRAIGHT) {
		//行く先にプレイヤーがいたら止まる
		if (_hit->IsHit(_player.GetRect(), flontPos)) {
			_state = GM_PAUSE;
			return;
		}
		else if (_map->GetChipType(flontPos) != CHIP_BLANK&& _map->GetChipType(flontPos) != CHIP_ROPE_FALL) {	//壁があったら止まる
			_state = GM_PAUSE;
		}
		else if (dropFlag == false) {	//基本動作
			if (_map->GetChipType(nextPos) != CHIP_BLANK) {

				//ﾛｰﾌﾟをどう受け取るか決めていないのでとりあえずﾊﾞｯｸｽﾍﾟｰｽで止まるようにする
				if (_inpInfo.key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON) {
					_state = GM_PAUSE;
				}
				//_state = GM_END;
			}
			else if (_map->GetChipType(nextPos) == CHIP_BLANK &&_map->GetChipType(nextPos1) == CHIP_BLANK) {	//行く先に床がなければ落ちる
				dropFlag = true;
			}
			//ﾛｰﾌﾟの移動量を受け取る
			_pos.x += _rope.GetRopeVec().x;
			//if (_rope._ropeDir != ROPE_DIR_STRAIGHT) {	//まっすぐの時はｙ軸側には移動させない
			//	_pos.y += _rope.GetRopeVec().y;
			//}
			if (_rope.GetRopeVec().x == 0) {
				_state = GM_PAUSE;
			}
		}
		else {	//行く先に床がなかった場合（落下運動）
			if (_map->GetChipType(nextPos) != CHIP_BLANK) {	//落ちた先に地面があれば割れて動作を終了する
				_state = GM_END;
			}
			velocity += gravity;
			_pos.y += (int)velocity / 2;
		}
	}
	else {	//それ以外の時
			//行く先にプレイヤーがいたら止まる
		if (_hit->IsHit(_player.GetRect(), flontPos)) {
			_state = GM_PAUSE;
			return;
		}
		else if (_map->GetChipType(flontPos) != CHIP_BLANK&& _map->GetChipType(flontPos) != CHIP_ROPE_FALL) {	//壁があったら止まる
			_state = GM_PAUSE;
		}
		else if (dropFlag == false) {	//基本動作
			if (_map->GetChipType(nextPos) != CHIP_BLANK) {

				//ﾛｰﾌﾟをどう受け取るか決めていないのでとりあえずﾊﾞｯｸｽﾍﾟｰｽで止まるようにする
				if (_inpInfo.key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON) {
					_state = GM_PAUSE;
				}
				//_state = GM_END;
			}
			//else if (_map->GetChipType(nextPos) == CHIP_BLANK) {	//行く先に床がなければ落ちる
			//	dropFlag = true;
			//}
			//ﾛｰﾌﾟの移動量を受け取る
			_pos.x += _rope.GetRopeVec().x;
			_pos.y += _rope.GetRopeVec().y;
	
			if (_rope.GetRopeVec().x == 0) {
				_state = GM_PAUSE;
			}
		}
		//else {	//行く先に床がなかった場合（落下運動）
		//	if (_map->GetChipType(nextPos) != CHIP_BLANK) {	//落ちた先に地面があれば割れて動作を終了する
		//		_state = GM_END;
		//	}
		//	velocity += gravity;
		//	_pos.y += (int)velocity / 2;
		//}
	}

}

void GimDrop::MoveRight()
{
	//行く先の床があるかどうかを調べる
	Position2 nextPos = _pos;
	nextPos.x = _pos.x - (_gmRect.w / 2);
	nextPos.y = _pos.y + (_gmRect.h);
	//反対側も判定しておく
	Position2 nextPos1 = _pos;
	nextPos1.x = _pos.x + (_gmRect.h / 2);
	nextPos1.y = _pos.y + (_gmRect.w);
	//正面の行く先
	//とりあえず右の行く先だけ
	Position2 flontPos = _pos;
	flontPos.x = _pos.x + (_gmRect.w / 2);
	if (_rope._ropeDir == ROPE_DIR_STRAIGHT) {
		//行く先にプレイヤーがいたら止まる
		if (_hit->IsHit(_player.GetRect(), flontPos)) {
			_state = GM_PAUSE;
			return;
		}
		else if (_map->GetChipType(flontPos) != CHIP_BLANK&& _map->GetChipType(flontPos) != CHIP_ROPE_FALL) {	//壁があったら止まる
			_state = GM_PAUSE;
		}
		else if (dropFlag == false) {	//基本動作	

			if (_map->GetChipType(nextPos) == CHIP_BLANK && _map->GetChipType(nextPos1)== CHIP_BLANK) {	//行く先に床がなければ落ちる
				dropFlag = true;
			}
			else if (_map->GetChipType(nextPos) != CHIP_BLANK) {

				//ﾛｰﾌﾟをどう受け取るか決めていないのでとりあえずﾊﾞｯｸｽﾍﾟｰｽで止まるようにする
				if (_inpInfo.key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON) {
					_state = GM_PAUSE;
				}
				//ﾛｰﾌﾟの移動量を受け取る
				_pos.x += _rope.GetRopeVec().x;
				if (_rope.GetRopeVec().x == 0) {
					_state = GM_PAUSE;
				}
			}
		}
		else {	//行く先に床がなかった場合（落下運動）
			nextPos.y = _pos.y + (_gmRect.h * 2);
			if (_map->GetChipType(nextPos) != CHIP_BLANK) {	//落ちた先に地面があれば割れて動作を終了する
				_state = GM_END;
			}
			velocity += gravity;
			_pos.y += (int)velocity / 2;
		}
	}
	else {
		//行く先にプレイヤーがいたら止まる
		if (_hit->IsHit(_player.GetRect(), flontPos)) {
			_state = GM_PAUSE;
			return;
		}
		else if (_map->GetChipType(flontPos) != CHIP_BLANK&& _map->GetChipType(flontPos) != CHIP_ROPE_FALL) {	//壁があったら止まる
			_state = GM_PAUSE;
		}
		else if (dropFlag == false) {	//基本動作	

			//if (_map->GetChipType(nextPos) == CHIP_BLANK) {	//行く先に床がなければ落ちる
			//	dropFlag = true;
			//}
		if (_map->GetChipType(nextPos) != CHIP_BLANK) {

				//ﾛｰﾌﾟをどう受け取るか決めていないのでとりあえずﾊﾞｯｸｽﾍﾟｰｽで止まるようにする
				if (_inpInfo.key.keybit.B_BUTTON && !_lastKey.keybit.B_BUTTON) {
					_state = GM_PAUSE;
				}
				//ﾛｰﾌﾟの移動量を受け取る
				_pos.x += _rope.GetRopeVec().x;
				_pos.y += _rope.GetRopeVec().y;
	
				if (_rope.GetRopeVec().x == 0) {
					_state = GM_PAUSE;
				}
			}
		}
		//else {	//行く先に床がなかった場合（落下運動）
		//	nextPos.y = _pos.y + (_gmRect.h * 2);
		//	if (_map->GetChipType(nextPos) != CHIP_BLANK) {	//落ちた先に地面があれば割れて動作を終了する
		//		_state = GM_END;
		//	}
		//	velocity += gravity;
		//	_pos.y += (int)velocity / 2;
		//}
	}
	

}

//描画
void GimDrop::Draw(Position2 offset) 
{
	if (_state != GM_END && _state != GM_PAUSE) {
		//そのものの描画
		DrawCircle(_pos.x - offset.x,_pos.y -offset.y,10,GetColor(255,0,255),true);
	}
	else if (_state == GM_END) {
		//割れたりしてる描画
		count--;
		if (count >= 0) {
			DrawString(_pos.x - offset.x -70, _pos.y - offset.y -30, "＼ガシャーン／", 0xffff00);
		}
		DrawCircle(_pos.x - offset.x, _pos.y - offset.y, 10, GetColor(255, 0, 0), true);
	}
	else if (_state == GM_PAUSE) {		//移動が一時停止しているとき（壁にぶつかったなど)
		DrawCircle(_pos.x - offset.x, _pos.y - offset.y, 10, GetColor(0, 0, 255), true);
	}
	_gmRect.SetCenter(_pos.x -offset.x, _pos.y - offset.y);
	_gmRect.Draw();
}
//あたり判定用
Rect& GimDrop::GetRect()
{
	return _gmRect;
}
//gimmickの種類を返すための関数
GIMMICK_TYPE& GimDrop::GetType()
{
	return _gimType;
}