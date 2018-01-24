#include <stdio.h>
#include <DxLib.h>
#include "HitClass.h"
#include "Geometry.h"
#include "Camera.h"
#include "Player.h"
#include "MapCtl.h"

MapCtl* MapCtl::ptr = nullptr;

// �ݽ�׸�
MapCtl::MapCtl()
{
		LoadDivGraph("��image/�}�b�v�`�b�v.png",8,8,1,32,32,chipImage);
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

// �`��
//void MapCtl::Draw()
//{
//	for (auto itr = mapData.begin(); itr != mapData.end(); itr++)
//	{
//		for (int y = 0; y < itr->second.y; y++)
//		{
//			for (int x = 0; x < itr->second.x; x++)
//			{
//				// ���W�����ߔԍ�
//				DrawMapChip(x, y, itr->second.data[(y * itr->second.x + x)]);
//			}
//		}
//	}
//}
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
//	switch (num)			// ϯ�����ߕʂɕ`��
//	{
//	case CHIP_BLANK:		// �����Ȃ�
//		color = GetColor(0, 50, 50);
//		break;
//	case CHIP_N_CLIMB_WALL:	// �o��Ȃ���	
//		color = GetColor(85, 44, 32);
//		break;
//	case CHIP_CLIMB_WALL:	// �o����
//		color = GetColor(115, 66, 41);
//		break;
//		// ��Я��p
//	case CHIP_DOOR:			// �h�A
//// ���ޯ�
//#ifdef _DEGUB
//		color = GetColor(55, 52, 52);
//#endif
//		break;
//	case CHIP_BUTTON_1:		// �{�^��
//		color = GetColor(101, 79, 56);
//		break;
//	case CHIP_ROPE_ATTRACT:	// ۰�߈����񂹂�
//		color = GetColor(202, 81, 55);
//		break;
//	case CHIP_ROPE_FALL:	// ۰�ߗ��Ƃ�
//// ���ޯ�
//#ifdef _DEBUG
//		color = GetColor(0, 102, 0);
//#endif
//		break;
//		//case CHIP_FREE_1:		// ���R�Ɏg����
//		//color = GetColor(198,54,89);
//		//break;
//		//case CHIP_FREE_2:		// ���R�Ɏg����
//		//	color = GetColor(255,235,88);
//		//	break;
//		//case CHIP_FREE_3:		// ���R�Ɏg����
//		//	color = GetColor(160,202,90);
//		//	break;
//		//case CHIP_FREE_4:		// ���R�Ɏg����
//		//	color = GetColor(223,84,100);
//		//	break;
//		//case CHIP_FREE_5:		// ���R�Ɏg����
//		//	color = GetColor(28,51,112);
//		//	break;
//	}
	DrawGraph(x * 32 + 0 - offset.x, y * 32 + 0 - offset.y, chipImage[num],true);
	//DrawBox(x * 32 + 0 -offset.x, y * 32 + 0 -offset.y, (x * 32) + 32 - offset.x, (y * 32) + 32 -offset.y, color, true);
//
//// ���ޯ��p
//#ifdef _DEBUG
//	DrawString(20,  10, "BLANK",		GetColor(255, 255, 255));
//	DrawString(75,  10, "�o��Ȃ���",	GetColor(0, 0, 0));
//	DrawString(170, 10, "�o����",		GetColor(115, 66, 41));
//	DrawString(240, 10, "�h�A",			GetColor(55, 52, 52));
//	DrawString(290, 10, "�{�^��1",		GetColor(101, 79, 56));
//	DrawString(370, 10, "۰�߈�",		GetColor(202, 81, 55));
//	DrawString(460, 10, "۰�ߗ�",		GetColor(0, 102, 0));
//#else
//	#endif
}

//�M�~�b�N�Ƀf�[�^�����炤���߂ɒǉ����Ă܂�
std::vector<ChipPosData>MapCtl::getChipPosData() 
{
	std::vector<ChipPosData> chipPosData;

	chipPosData.clear();
	for (int y = 0; y < mapData[filedata].y; y++)
	{
		for (int x = 0; x < mapData[filedata].x; x++)
		{
			ChipPosData d = {};
			d.posX = x*MAP_CHIP_SIZE_X;
			d.posY = y*MAP_CHIP_SIZE_X;
			d.chipType = mapData[filedata].data[0][(y * mapData[filedata].x + x)];
			//if (d.chipType < 3)continue;
			chipPosData.push_back(d);
		}
	}
	return chipPosData;
}