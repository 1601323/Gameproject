#pragma once
class HitClass;
class ModelMgr;

class Midpoint
{
private:
	HitClass* _hit;
	Player* _pl;
	ModelMgr* _modelmgr;

	void GetPoint();	//���Ԓn�_�ɂ��ǂ蒅�������Ƃ�����
	void FollowDir();	//���Ă������߂̕����Ȃǂ������Őݒ�

	Position2 _pos;
	Position2 initPos;
	Rect _hitRect;
	DIR tmpDir;			//�v���C���[�̌����Ă��������ێ�����
	bool GetFlag;		//�ړI�����擾�������ǂ����̃t���O
	//����
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
	bool ReturnGetFlag();		//���Ԓn�_�ɂ��ǂ蒅���Ă��邩���Ȃ�����Ԃ�
	Rect& GetRect();
	Position2& GetInitPos();
};

