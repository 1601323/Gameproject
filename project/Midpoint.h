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

	Position2 _pos;
	Position2 initPos;
	Rect _hitRect;
	DIR tmpDir;			//プレイヤーの向いている方向を保持する
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
	Rect& GetRect();
	Position2& GetInitPos();
};

