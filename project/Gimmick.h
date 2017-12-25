#pragma once
#include "Geometry.h"
#include "Object.h"
class Input;

enum GimmickState {
	GM_NONE,	//通常状態（作動前）
	GM_HIT,		//作動前（ロープが当たった）
	GM_MOVE,	//作動中
	GM_MOVE2,	//元に戻っていく動作などに（ドアが閉まるとか）（ネーミングセンス()）
	GM_PAUSE,	//作動終了（繰り返すもの）
	GM_END,		//作動完全終了
};

class HitClass;
class Player;

class Gimmick : public Object
{
private:
	HitClass* _hit;
public:
	Gimmick();
	~Gimmick();
	Rect _gmRect;
	CLEAR_DATA _clearData;
	GIMMICK_TYPE _gimType;
	virtual Rect& GetRect() =0;
	GimmickState _state;
	virtual GIMMICK_TYPE& GetType();
	virtual CLEAR_DATA& GetClearData();

	void NextState();
	GimmickState nowState();
	virtual void Updata(Input& input);
	virtual void Updata();
	virtual void Draw(Position2 offset);
};

