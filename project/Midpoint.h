#pragma once
class HitClass;
class ModelMgr;

class Midpoint
{
private:
	HitClass* _hit;
	Player* _pl;
	ModelMgr* _modelmgr;

	void GetPoint();	//中間地点にたどり着いたことを示す
	void FollowDir();	//ついていくための方向などをここで設定

	Position2 _pos;		//鍵などの場所
	Position2 _midPos;	//薬品、チェックポイントの場所

	Position2 initPos;	//リトライ時のやり直し地点について
	Rect _hitRect;		//鍵のあたり判定について
	Rect _midRect;		//中間点のあたり判定について
	DIR tmpDir;			//プレイヤーの向いている方向を保持する
	bool checkpointFlag;//鍵を手に入れたかどうかについて
	bool GetFlag;		//目的物を取得したかどうかのフラグ
	//実験
	bool bubbleFlag;
	int cnt;
	int bubble;
	int modelhandle;
public:
	Midpoint();
	~Midpoint();	
	void Updata();
	void GetClass(Player* p);
	void Draw(Position2 offset);
	bool ReturnGetFlag();		//中間地点にたどり着いているかいないかを返す
	bool ReturnCheckFlag();		//鍵を取得しているかどうかを返す
	Rect& GetRect();
	Rect& GetRect2();
	Position2& GetInitPos();
};

