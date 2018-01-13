#pragma once
class HitClass;
class Midpoint
{
private:
	HitClass* _hit;
	Player* _pl;
	void GetPoint();	//中間地点にたどり着いたことを示す

	Position2 _pos;
	Rect _hitRect;
	bool GetFlag;		//目的物を取得したかどうかのフラグ
public:
	Midpoint();
	~Midpoint();	
	void Updata();
	void GetClass(Player* p);
	void Draw(Position2 offset);
	bool ReturnGetFlag();		//中間地点にたどり着いているかいないかを返す
	Rect& GetRect();
};

