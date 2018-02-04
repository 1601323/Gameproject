#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "Object.h"

using namespace std;

class Player;
class ModelMgr;

// map�̊e�������
// �\���̓��̃p�b�L���O[1�޲Ă���]
#pragma pack(1)
struct MapFileHeader {
	short map_sx;				// map����x
	short map_sy;				// map����y
	short chp_sx;				// ���߻���x
	short chp_sy;				// ���߻���y
	unsigned char layer;		// ڲ԰
	unsigned char chp_mode;		// Ӱ��
};
#pragma pack()

struct MapData {
	vector<unsigned char>data[LAYER_MAX];
	unsigned int x;
	unsigned int y;
	unsigned int layer;
};

//��Я��ŗ��p����f�[�^
struct ChipPosData {
	char chipType;
	float posX;
	float posY;
};
//��а�ŗ��p�����ް�
struct EnemyPosData {
	char enemyType;
	float posX;
	float posY;
};
class MapCtl
{
public:
	~MapCtl();
	static MapCtl* GetInstance(void);
	string filedata;						// �ް��ۑ��p
	Position2 GetPlayerPos(void);			// player�̍��W�擾
	Position2 GetChipPos(Position2 idPos);	// MapChip�̍��W�Ԃ�[������W]
	CHIP_TYPE GetChipType(Position2 idPos);	// �������ߎ擾�p
	unsigned int GetMapID(Position2 idPos);	// ϯ������ID�擾[���W]
	unsigned int GetMapNum(Position2 getPos);	//�}�b�v�`�b�v�̔ԍ����擾
	void Draw(void);			// �`��
	void Draw(Position2 offset);
	void Load(const char* fileName);		// �ǂݍ��� 
	void ChangeStageTexture(unsigned int chipNum);//�X�e�[�W�ɂ���ă`�b�v�̃e�N�X�`����ς���

	int chipImage[8];
	int chipModelHandle[3];
	int textureIndex;

	//�M�~�b�N�p�ɒǉ�
	//CHIP_TYPE��Position�����f�[�^
	//std::vector<ChipPosData> chipPosData;
	//�M�~�b�N�̂��߂̒ǉ��ł�
	//�`�b�v�^�C�v�Əꏊ��ǂݍ���
	vector<ChipPosData> getChipPosData();
	//�G�p�ɒǉ�
	vector<EnemyPosData> getEnemyData();
private:
	static MapCtl* ptr;					// �߲��	
	ModelMgr* _modelmgr;
	shared_ptr<Object> obj;
	map<string, MapData> mapData;		// ϯ���ް�	[mapData�𐶐�]	
	MapCtl();				// �ݽ�׸�
	MapCtl(const MapCtl&);	// �ݽ�׸�[����] ���ް۰��
	//MapCtl & operator=(const MapCtl&);
	//void DrawMapChip(int x, int y, unsigned int num);	// ϯ�����߂̕`��
	void DrawMapChip(int x, int y,Position2 offset, unsigned int num);	// ϯ�����߂̕`��

};

// �������ߒm�肽���Ȃ�GetChipType
// ���̂������߂̍��W��m�肽���Ȃ�GetChipPos