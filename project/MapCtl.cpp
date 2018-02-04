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

// ｺﾝｽﾄﾗｸﾀ
MapCtl::MapCtl()
{
	LoadDivGraph("仮image/マップチップ.png",8,8,1,32,32,chipImage);
	_modelmgr = ModelMgr::Instance();
	//chipModelHandle[1] = _modelmgr->ModelIdReturn("wall＿model/wall.pmx", SCENE_RESULT);
	//chipModelHandle[2] = _modelmgr->ModelIdReturn("wall＿model/wall2.pmx", SCENE_RESULT);
	//リトライしたら
	chipModelHandle[1] = MV1LoadModel("wall＿model/wall.pmx");
	chipModelHandle[2] = MV1LoadModel("wall＿model/wall2.pmx");
	textureIndex = MV1GetMaterialDifMapTexture(chipModelHandle[2], 3);
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
	//レイヤー枚数分回します
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
	ImageMgr& im = ImageMgr::Instance();

	//DrawGraph(x * 32 + 0 - offset.x, y * 32 + 0 - offset.y, chipImage[num],true);
	MV1SetPosition(chipModelHandle[num], ConvWorldPosToScreenPos(VGet(x*32 - offset.x+15,y*32 - offset.y+15,0)));
	MV1SetScale(chipModelHandle[num], VGet(3.18f, 3.18f,3.18f));
	//テクスチャを変更
	 if (num == 2) MV1SetTextureGraphHandle(chipModelHandle[num], textureIndex, im.ImageIdReturn("wall＿model/wall4.png", SCENE_TITLE), FALSE);
	//モデルを輪郭線0.0fで描画 
	_modelmgr->Draw(chipModelHandle[num],0.0f);
}

//ギミックにデータをもらうために追加してます
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
//敵にﾃﾞｰﾀを渡すために用意
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