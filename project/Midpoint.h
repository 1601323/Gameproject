#pragma once
class HitClass;
class Midpoint
{
private:
	HitClass* _hit;
	void GetPoint();	//���Ԓn�_�ɂ��ǂ蒅�������Ƃ�����

	Position2 _pos;
	Rect _hitRect;
	bool GetFlag;		//�ړI�����擾�������ǂ����̃t���O
public:
	Midpoint();
	~Midpoint();	
	void Updata();

	void Draw(Position2 offset);
	bool ReturnGetFlag();		//���Ԓn�_�ɂ��ǂ蒅���Ă��邩���Ȃ�����Ԃ�
};

