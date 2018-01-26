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

	Position2 _pos;		//���Ȃǂ̏ꏊ
	Position2 _midPos;	//��i�A�`�F�b�N�|�C���g�̏ꏊ

	Position2 initPos;	//���g���C���̂�蒼���n�_�ɂ���
	Rect _hitRect;		//���̂����蔻��ɂ���
	Rect _midRect;		//���ԓ_�̂����蔻��ɂ���
	DIR tmpDir;			//�v���C���[�̌����Ă��������ێ�����
	bool checkpointFlag;//������ɓ��ꂽ���ǂ����ɂ���
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
	bool ReturnCheckFlag();		//�����擾���Ă��邩�ǂ�����Ԃ�
	Rect& GetRect();
	Rect& GetRect2();
	Position2& GetInitPos();
};

