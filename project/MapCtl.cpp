#include <stdio.h>
#include <DxLib.h>
#include "HitClass.h"
#include "Geometry.h"
#include "Camera.h"
#include "Player.h"
#include "MapCtl.h"
#include "ModelMgr.h"
#include "ImageMgr.h"

MapCtl* MapCtl::ptr = nullptr;

// �ݽ�׸�
MapCtl::MapCtl()
{
	LoadDivGraph("��image/�}�b�v�`�b�v.png",8,8,1,32,32,chipImage);
	_modelmgr = ModelMgr::Instance();
	//chipModelHandle[1] = _modelmgr->ModelIdReturn("wall�Qmodel/wall.pmx", SCENE_RESULT);
	//chipModelHandle[2] = _modelmgr->ModelIdReturn("wall�Qmodel/wall2.pmx", SCENE_RESULT);
	//���g���C������
	chipModelHandle[1] = MV1LoadModel("wall�Qmodel/wall.pmx");
	chipModelHandle[2] = MV1LoadModel("wall�Qmodel/wall2.pmx");
	textureIndex = MV1GetMaterialDifMapTexture(chipModelHandle[2], 3);
}

// �޽�׸�
MapCtl::~MapCtl()
{

}

// �����p
MapCtl* MapCtl::GetInstance(void) {
	// null�̏ꍇnew����
	if (ptr == nullptr)
	{
		ptr = new MapCtl();
	}
	return ptr;
}

// player�̍��W�擾
Position2 MapCtl::GetPlayerPos(void)
{
	return  obj->GetPos();
}

// �}�b�v�`�b�v�̍��W�Ԃ�
Position2 MapCtl::GetChipPos(Position2 idPos)
{
	Position2 tmpPos = { idPos.x / MAP_CHIP_SIZE_X,idPos.y / MAP_CHIP_SIZE_Y };
	// �͈�
	if (static_cast<int>(tmpPos.x) >= 0 && static_cast<int>(tmpPos.x) < MAP_SIZE_X
	 && static_cast<int>(tmpPos.y) >= 0 && static_cast<int>(tmpPos.y) < MAP_SIZE_Y)
	{
		// ����̍��W�Ԃ�
		return Position2(static_cast<float>(static_cast<int>(tmpPos.x) * MAP_CHIP_SIZE_X), static_cast<float>(static_cast<int>(tmpPos.y) * MAP_CHIP_SIZE_Y));
	}
	return Position2();
}

// �ǂݍ���
void MapCtl::Load(const char* fileName)
{
	FILE* fp;	// ̧���߲��
				// �װ����[]
	if (fopen_s(&fp, fileName, "rb") != 0)
	{
		return;
	}

	// ���i�[�p
	MapFileHeader header = {};
	// �ǂݍ���[Map���,�ǂݍ��ރf�[�^�T�C�Y,�ǂݍ��މ�,�ǂ̏ꏊ���̃|�C���^]
	fread(&header, sizeof(MapFileHeader), 1, fp);
	// map����
	mapData[fileName].x = header.map_sx;
	mapData[fileName].y = header.map_sy;
	mapData[fileName].layer = header.layer;
	// fseek[FILE�߲��,�ړ��޲�,̧�ق̌��ݒn]
	fseek(fp, 6, SEEK_CUR);
	//���C���[�������񂵂܂�
	for (int f = 0; f < header.layer; f++) {
		// ϯ���ް��̑傫���m��
		mapData[fileName].data[f].resize(header.map_sx*header.map_sy);
		// fread[�߲��,�ް��̒���,�ް���,file�߲��]
		fread(&mapData[fileName].data[f][0], header.map_sx * header.map_sy * (header.chp_mode + 1) /** header.layer*/, 1, fp);
	}
	// �N���[�Y
	fclose(fp);
	// �f�[�^�n��
	filedata = fileName;
}

void MapCtl::Draw(Position2 offset)
{
	for (int f = 0; f < mapData[filedata].layer; f++) {
		for (int y = 0; y < mapData[filedata].y; y++)
		{
			for (int x = 0; x < mapData[filedata].x; x++)
			{
				// ���W�����ߔԍ�
				DrawMapChip(x, y, offset, mapData[filedata].data[f][(y * mapData[filedata].x + x)]);
			}
		}
	}
}
// �������ߎ擾�p
CHIP_TYPE MapCtl::GetChipType(Position2 idPos)
{
	Position2 tmpPos = {idPos.x / MAP_CHIP_SIZE_X,idPos.y/MAP_CHIP_SIZE_Y};
	if (static_cast<int>(tmpPos.x) >= 0 && static_cast<int>(tmpPos.x) < MAP_SIZE_X
	&&  static_cast<int>(tmpPos.y) >= 0 && static_cast<int>(tmpPos.y) < MAP_SIZE_Y)
	{
		return (CHIP_TYPE)mapData[filedata].data[0][(static_cast<int>(tmpPos.y) * MAP_SIZE_X) + static_cast<int>(tmpPos.x)];
	}
	return CHIP_MAX;
}

// ϯ�����ߏ��̎擾[���W]
unsigned int MapCtl::GetMapID(Position2 idPos)
{
	Position2 tmpPos = {idPos.x / MAP_CHIP_SIZE_X,idPos.y / MAP_CHIP_SIZE_Y};
	// �͈�
	if (static_cast<int>(tmpPos.x) >= 0 && static_cast<int>(tmpPos.x) < MAP_SIZE_X
	&&  static_cast<int>(tmpPos.y) >= 0 && static_cast<int>(tmpPos.y) < MAP_SIZE_Y)
	{
		return mapData[filedata].data[0][(static_cast<int>(tmpPos.y) * MAP_SIZE_X) + static_cast<int>(tmpPos.x)];
	}
	return CHIP_MAX;
}
unsigned int MapCtl::GetMapNum(Position2 idPos)
{
	Position2 tmpPos = { idPos.x / MAP_CHIP_SIZE_X,idPos.y / MAP_CHIP_SIZE_Y };
	// �͈�
	if (static_cast<int>(tmpPos.x) >= 0 && static_cast<int>(tmpPos.x) < MAP_SIZE_X
		&&  static_cast<int>(tmpPos.y) >= 0 && static_cast<int>(tmpPos.y) < MAP_SIZE_Y)
	{
		return((static_cast<int>(tmpPos.y) * MAP_SIZE_X) + static_cast<int>(tmpPos.x));
	}
	return CHIP_MAX;
}
// ϯ�߃`�b�v�`��[���W,���ߔԍ�]
void MapCtl::DrawMapChip(int x, int y, Position2 offset, unsigned int num)
{
	auto color = GetColor(0, 50, 50);
	ImageMgr& im = ImageMgr::Instance();

	//DrawGraph(x * 32 + 0 - offset.x, y * 32 + 0 - offset.y, chipImage[num],true);
	MV1SetPosition(chipModelHandle[num], ConvWorldPosToScreenPos(VGet(x*32 - offset.x+15,y*32 - offset.y+15,0)));
	MV1SetScale(chipModelHandle[num], VGet(3.18f, 3.18f,3.18f));
	//�e�N�X�`����ύX
	 if (num == 2) MV1SetTextureGraphHandle(chipModelHandle[num], textureIndex, im.ImageIdReturn("wall�Qmodel/wall4.png", SCENE_TITLE), FALSE);
	//���f����֊s��0.0f�ŕ`�� 
	_modelmgr->Draw(chipModelHandle[num],0.0f);
}

//�M�~�b�N�Ƀf�[�^�����炤���߂ɒǉ����Ă܂�
std::vector<ChipPosData>MapCtl::getChipPosData() 
{
	std::vector<ChipPosData> chipPosData;

	chipPosData.clear();
	for (int l = 0; l < mapData[filedata].layer; l++) {
		for (int y = 0; y < mapData[filedata].y; y++)
		{
			for (int x = 0; x < mapData[filedata].x; x++)
			{
				ChipPosData d = {};
				d.posX = x*MAP_CHIP_SIZE_X;
				d.posY = y*MAP_CHIP_SIZE_X;
				d.chipType = mapData[filedata].data[l][(y * mapData[filedata].x + x)];
				//if (d.chipType < 3)continue;
				chipPosData.push_back(d);
			}
		}
	}
	return chipPosData;
}
//�G���ް���n�����߂ɗp��
std::vector<EnemyPosData> MapCtl::getEnemyData()
{
	std::vector<EnemyPosData> enemyPosData;
	enemyPosData.clear();
	for (int l = 0; l < mapData[filedata].layer; l++) {
		for (int y = 0; y < mapData[filedata].y; y++)
		{
			for (int x = 0; x < mapData[filedata].x; x++)
			{
				EnemyPosData d = {};
				d.posX = x*MAP_CHIP_SIZE_X;
				d.posY = y*MAP_CHIP_SIZE_X;
				d.enemyType = mapData[filedata].data[l][(y * mapData[filedata].x + x)];
				//if (d.chipType < 3)continue;
				enemyPosData.push_back(d);
			}
		}
	}
	return enemyPosData;
}