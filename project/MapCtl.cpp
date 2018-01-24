#include <stdio.h>
#include <DxLib.h>
#include "HitClass.h"
#include "Geometry.h"
#include "Camera.h"
#include "Player.h"
#include "MapCtl.h"

MapCtl* MapCtl::ptr = nullptr;

// ｺﾝｽﾄﾗｸﾀ
MapCtl::MapCtl()
{
		LoadDivGraph("仮image/マップチップ.png",8,8,1,32,32,chipImage);
}

// ﾃﾞｽﾄﾗｸﾀ
MapCtl::~MapCtl()
{

}

// 生成用
MapCtl* MapCtl::GetInstance(void) {
	// nullの場合newする
	if (ptr == nullptr)
	{
		ptr = new MapCtl();
	}
	return ptr;
}

// playerの座標取得
Position2 MapCtl::GetPlayerPos(void)
{
	return  obj->GetPos();
}

// マップチップの座標返す
Position2 MapCtl::GetChipPos(Position2 idPos)
{
	Position2 tmpPos = { idPos.x / MAP_CHIP_SIZE_X,idPos.y / MAP_CHIP_SIZE_Y };
	// 範囲
	if (static_cast<int>(tmpPos.x) >= 0 && static_cast<int>(tmpPos.x) < MAP_SIZE_X
	 && static_cast<int>(tmpPos.y) >= 0 && static_cast<int>(tmpPos.y) < MAP_SIZE_Y)
	{
		// 左上の座標返す
		return Position2(static_cast<float>(static_cast<int>(tmpPos.x) * MAP_CHIP_SIZE_X), static_cast<float>(static_cast<int>(tmpPos.y) * MAP_CHIP_SIZE_Y));
	}
	return Position2();
}

// 読み込み
void MapCtl::Load(const char* fileName)
{
	FILE* fp;	// ﾌｧｲﾙﾎﾟｲﾝﾀ
				// ｴﾗｰﾁｪｯｸ[]
	if (fopen_s(&fp, fileName, "rb") != 0)
	{
		return;
	}

	// 情報格納用
	MapFileHeader header = {};
	// 読み込み[Map情報,読み込むデータサイズ,読み込む回数,どの場所化のポインタ]
	fread(&header, sizeof(MapFileHeader), 1, fp);
	// mapｻｲｽﾞ
	mapData[fileName].x = header.map_sx;
	mapData[fileName].y = header.map_sy;
	mapData[fileName].layer = header.layer;
	// fseek[FILEﾎﾟｲﾝﾀ,移動ﾊﾞｲﾄ,ﾌｧｲﾙの現在地]
	fseek(fp, 6, SEEK_CUR);
	for (int f = 0; f < header.layer; f++) {

	// ﾏｯﾌﾟﾃﾞｰﾀの大きさ確保
	mapData[fileName].data[f].resize(header.map_sx*header.map_sy);
	// fread[ﾎﾟｲﾝﾀ,ﾃﾞｰﾀの長さ,ﾃﾞｰﾀ数,fileﾎﾟｲﾝﾀ]
		fread(&mapData[fileName].data[f][0], header.map_sx * header.map_sy * (header.chp_mode + 1) /** header.layer*/, 1, fp);
	}
	// クローズ
	fclose(fp);
	// データ渡す
	filedata = fileName;
}

// 描画
//void MapCtl::Draw()
//{
//	for (auto itr = mapData.begin(); itr != mapData.end(); itr++)
//	{
//		for (int y = 0; y < itr->second.y; y++)
//		{
//			for (int x = 0; x < itr->second.x; x++)
//			{
//				// 座標とﾁｯﾌﾟ番号
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
				// 座標とﾁｯﾌﾟ番号
				DrawMapChip(x, y, offset, mapData[filedata].data[f][(y * mapData[filedata].x + x)]);
			}
		}
	}
}
// ﾁｯﾌﾟﾀｲﾌﾟ取得用
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

// ﾏｯﾌﾟﾁｯﾌﾟ情報の取得[座標]
unsigned int MapCtl::GetMapID(Position2 idPos)
{
	Position2 tmpPos = {idPos.x / MAP_CHIP_SIZE_X,idPos.y / MAP_CHIP_SIZE_Y};
	// 範囲
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
	// 範囲
	if (static_cast<int>(tmpPos.x) >= 0 && static_cast<int>(tmpPos.x) < MAP_SIZE_X
		&&  static_cast<int>(tmpPos.y) >= 0 && static_cast<int>(tmpPos.y) < MAP_SIZE_Y)
	{
		return((static_cast<int>(tmpPos.y) * MAP_SIZE_X) + static_cast<int>(tmpPos.x));
	}
	return CHIP_MAX;
}
// ﾏｯﾌﾟチップ描画[座標,ﾁｯﾌﾟ番号]
void MapCtl::DrawMapChip(int x, int y, Position2 offset, unsigned int num)
{
	auto color = GetColor(0, 50, 50);
//	switch (num)			// ﾏｯﾌﾟﾁｯﾌﾟ別に描画
//	{
//	case CHIP_BLANK:		// 何もない
//		color = GetColor(0, 50, 50);
//		break;
//	case CHIP_N_CLIMB_WALL:	// 登れない壁	
//		color = GetColor(85, 44, 32);
//		break;
//	case CHIP_CLIMB_WALL:	// 登れる壁
//		color = GetColor(115, 66, 41);
//		break;
//		// ｷﾞﾐｯｸ用
//	case CHIP_DOOR:			// ドア
//// ﾃﾞﾊﾞｯｸ
//#ifdef _DEGUB
//		color = GetColor(55, 52, 52);
//#endif
//		break;
//	case CHIP_BUTTON_1:		// ボタン
//		color = GetColor(101, 79, 56);
//		break;
//	case CHIP_ROPE_ATTRACT:	// ﾛｰﾌﾟ引き寄せる
//		color = GetColor(202, 81, 55);
//		break;
//	case CHIP_ROPE_FALL:	// ﾛｰﾌﾟ落とす
//// ﾃﾞﾊﾞｯｸ
//#ifdef _DEBUG
//		color = GetColor(0, 102, 0);
//#endif
//		break;
//		//case CHIP_FREE_1:		// 自由に使って
//		//color = GetColor(198,54,89);
//		//break;
//		//case CHIP_FREE_2:		// 自由に使って
//		//	color = GetColor(255,235,88);
//		//	break;
//		//case CHIP_FREE_3:		// 自由に使って
//		//	color = GetColor(160,202,90);
//		//	break;
//		//case CHIP_FREE_4:		// 自由に使って
//		//	color = GetColor(223,84,100);
//		//	break;
//		//case CHIP_FREE_5:		// 自由に使って
//		//	color = GetColor(28,51,112);
//		//	break;
//	}
	DrawGraph(x * 32 + 0 - offset.x, y * 32 + 0 - offset.y, chipImage[num],true);
	//DrawBox(x * 32 + 0 -offset.x, y * 32 + 0 -offset.y, (x * 32) + 32 - offset.x, (y * 32) + 32 -offset.y, color, true);
//
//// ﾃﾞﾊﾞｯｸ用
//#ifdef _DEBUG
//	DrawString(20,  10, "BLANK",		GetColor(255, 255, 255));
//	DrawString(75,  10, "登れない壁",	GetColor(0, 0, 0));
//	DrawString(170, 10, "登れる壁",		GetColor(115, 66, 41));
//	DrawString(240, 10, "ドア",			GetColor(55, 52, 52));
//	DrawString(290, 10, "ボタン1",		GetColor(101, 79, 56));
//	DrawString(370, 10, "ﾛｰﾌﾟ引",		GetColor(202, 81, 55));
//	DrawString(460, 10, "ﾛｰﾌﾟ落",		GetColor(0, 102, 0));
//#else
//	#endif
}

//ギミックにデータをもらうために追加してます
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