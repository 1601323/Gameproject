#include "Object.h"
#include "Player.h"
#include "Geometry.h"
#include "Camera.h"
#include "Rope.h"
#include "DxLib.h"
#include "MapCtl.h"

#define CAMERA_LEFT_POS (0)
#define CAMERA_RIGHT_POS	  ((MAP_CHIP_SIZE_X * MAP_SIZE_X) - SCREEN_SIZE_X)
#define CAMERA_MOVE_POS		   (MAP_CHIP_SIZE_X * 6)
#define CAMERA_MOVE_LIMIT_POS ((MAP_CHIP_SIZE_X * MAP_SIZE_X) - (SCREEN_SIZE_X - (MAP_CHIP_SIZE_X * 6)))

Camera* Camera::cam_ptr = nullptr;

// ｺﾝｽﾄﾗｸﾀ
Camera::Camera()
{
	_player = nullptr;
	_mapCtl = nullptr;
	this->SetGameScrSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	this->SetCamAreaOffset({ MAP_CHIP_SIZE_X * 6,MAP_CHIP_SIZE_Y * 6 });
}

// ﾃﾞｽﾄﾗｸﾀ
Camera::~Camera()
{
}

//// ここに描画してほしいやつ追加
//bool Camera::AddObject(Object * obj)
//{
//	return true;
//}

Camera * Camera::GetInstance(void)
{
	// nullの場合newする
	if (cam_ptr == nullptr)
	{
		cam_ptr = new Camera();
	}

	return cam_ptr;
}

// ﾀｰｹﾞｯﾄの決定
bool Camera::SetTarget(Player* player)
{
	_player = player;
	return _player;
}

// ﾏｯﾌﾟｺﾝﾄﾛｰﾙの設定
bool Camera::SetMapCtl(MapCtl* mapCtl) {
	_mapCtl = mapCtl;
	return _mapCtl;
}

// 更新
void Camera::Update()
{
	//ﾌﾟﾚｲﾔｰの座標取得
	auto pPos = _player->GetPos();

	// 左側のずらさないとき
	if (pPos.x < CAMERA_MOVE_POS) 
	{
		tmpPos.x = CAMERA_LEFT_POS;	
	}
	// 真ん中のずらすとき
	else if ((pPos.x >= CAMERA_MOVE_POS) && (pPos.x <  CAMERA_MOVE_LIMIT_POS)) 
	{
		tmpPos.x = pPos.x - CAMERA_MOVE_POS;
	}
	// 右側のずらさないとき
	else if (pPos.x >= CAMERA_MOVE_LIMIT_POS) 
	{
		tmpPos.x = CAMERA_RIGHT_POS;
	}
}

// ゲーム画面に映る範囲サイズ
bool Camera::SetGameScrSize(Position2 gmScrSize)
{
	GameScrSize = gmScrSize;

	return true;
}

// ｵﾌｾｯﾄ情報
bool Camera::SetCamAreaOffset(Position2 offset)
{
	CamOffset = offset;

	return true;
}

// ｶﾒﾗｻｲｽﾞの取得
Position2 Camera::GetGameScrSize(void)
{
	return GameScrSize;
}

// 描画
void Camera::Draw()
{
	//ﾏｯﾌﾟの描画
	//_mapCtl->Draw(tmpPos);

	//ﾌﾟﾚｲﾔｰの描画
	//_player->Draw(tmpPos);

	//ここに他のやつも書いてあげてね
	//_rope->DrawRopeRect(tmpPos);

	//リスト化して表示したい順にソートして　Drawして
	//リストようの関数作ってあげたりすると引数でObjg*　_objみたいなのでpublicでやればよきよき
}

Camera::Camera(const Camera &)
{
}

Position2& Camera::ReturnOffset()
{
	return tmpPos;
}