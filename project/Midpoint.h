#pragma once
class Input;
class HitClass;
class ModelMgr;

class Midpoint
{
private:
	HitClass* _hit;
	Player* _pl;
	ModelMgr* _modelmgr;
	KEY _key;
	KEY _lastKey;
	INPUT_INFO _inpInfo;

	void GetPoint();			//���Ԓn�_�ɂ��ǂ蒅�������Ƃ�����
	void FollowDir();			//���Ă������߂̕����Ȃǂ������Őݒ�

	Position2 _pos;				//���Ȃǂ̃`�F�b�N�|�C���g�̏ꏊ
	Position2 _midPos;			//���ɂȂǂ̏ꏊ
	
	Position2 initPos;			//���g���C���̂�蒼���n�_�ɂ��āi��)
	Rect _hitRect;				//���̂����蔻��ɂ���
	Rect _midRect;				//���ԓ_�̂����蔻��ɂ���
	DIR tmpDir;					//�v���C���[�̌����Ă��������ێ�����
	bool checkpointFlag;		//������ɓ��ꂽ���ǂ����ɂ���
	bool GetFlag;				//�ړI�����擾�������ǂ����̃t���O
	bool uiFlag;				//UI��\�����邽�߂̃t���O
	//����
	bool bubbleFlag;
	int cnt;
	int bubble;
	int modelhandle;
public:
	Midpoint();
	~Midpoint();	
	void Updata(Input* input);
	void Updata();
	void GetClass(Player* p);
	void Draw(Position2 offset);
	bool ReturnGetFlag();		//���Ԓn�_�ɂ��ǂ蒅���Ă��邩���Ȃ�����Ԃ�
	bool ReturnCheckFlag();		//�����擾���Ă��邩�ǂ�����Ԃ�
	Rect& GetRect();
	Rect& GetRect2();
	Position2& GetInitPos();
	//�}�b�v���珉���ʒu���󂯎��܂�
	void SetInitPos(CHIP_TYPE c,Position2 p);
};

