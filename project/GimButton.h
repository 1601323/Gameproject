#pragma once
#include "Gimmick.h"
#include "Geometry.h"
class Player;
class HitClass;
class GimButton :
	public Gimmick
{
private:
	HitClass* _hit;
	Player& _pl;
	GimmickState _state;
	Position2 _pos;
	Position2 _initPos;

	//遊び心
	//許されたい
	int count;
	int drawCount;
	int color;
	int Font;	
	void CheckDoMove();		//作動のための条件
	void Move();			//ギミックの内容について
	void Draw(Position2 offset);			//描画
	void SetInit();			//やりなおしのリセットを行う
public:
	GimButton(Position2 pos,Player& p);
	~GimButton();
	Rect _gmRect;
	Rect& GetRect();		//あたり判定用のレクトを返す
	GIMMICK_TYPE& GetType();
	void Updata();
};

