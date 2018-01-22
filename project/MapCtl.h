#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "Object.h"

using namespace std;

class Player;

// mapの各情報入れる
// 構造体内のパッキング[1ﾊﾞｲﾄずつに]
#pragma pack(1)
struct MapFileHeader {
	short map_sx;				// mapｻｲｽﾞx
	short map_sy;				// mapｻｲｽﾞy
	short chp_sx;				// ﾁｯﾌﾟｻｲｽﾞx
	short chp_sy;				// ﾁｯﾌﾟｻｲｽﾞy
	unsigned char layer;		// ﾚｲﾔｰ
	unsigned char chp_mode;		// ﾓｰﾄﾞ
};
#pragma pack()

struct MapData {
	vector<unsigned char>data;
	unsigned int x;
	unsigned int y;
};

//ｷﾞﾐｯｸで利用するデータ
struct ChipPosData {
	char chipType;
	float posX;
	float posY;
};

class MapCtl
{
public:
	~MapCtl();
	static MapCtl* GetInstance(void);
	string filedata;						// ﾃﾞｰﾀ保存用
	Position2 GetPlayerPos(void);			// playerの座標取得
	Position2 GetChipPos(Position2 idPos);	// MapChipの座標返す[左上座標]
	CHIP_TYPE GetChipType(Position2 idPos);	// ﾁｯﾌﾟﾀｲﾌﾟ取得用
	unsigned int GetMapID(Position2 idPos);	// ﾏｯﾌﾟﾁｯﾌﾟID取得[座標]
	unsigned int GetMapNum(Position2 getPos);	//マップチップの番号を取得
	void Draw(void);			// 描画
	void Draw(Position2 offset);
	void Load(const char* fileName);		// 読み込み 

	int chipImage[8];

	//ギミック用に追加
	//CHIP_TYPEとPositionをもつデータ
	//std::vector<ChipPosData> chipPosData;
	//ギミックのための追加です
	//チップタイプと場所を読み込む
	vector<ChipPosData> getChipPosData();
private:
	static MapCtl* ptr;					// ﾎﾟｲﾝﾀ	
	shared_ptr<Object> obj;
	map<string, MapData> mapData;		// ﾏｯﾌﾟﾃﾞｰﾀ	[mapDataを生成]	
	MapCtl();				// ｺﾝｽﾄﾗｸﾀ
	MapCtl(const MapCtl&);	// ｺﾝｽﾄﾗｸﾀ[引数] ｵｰﾊﾞｰﾛｰﾄﾞ
	//MapCtl & operator=(const MapCtl&);
	//void DrawMapChip(int x, int y, unsigned int num);	// ﾏｯﾌﾟﾁｯﾌﾟの描画
	void DrawMapChip(int x, int y,Position2 offset, unsigned int num);	// ﾏｯﾌﾟﾁｯﾌﾟの描画

};

// ﾁｯﾌﾟﾀｲﾌﾟ知りたいならGetChipType
// 今のいるﾁｯﾌﾟの座標を知りたいならGetChipPos