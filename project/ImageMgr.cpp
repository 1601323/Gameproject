#include <map>
#include <string>
#include <vector>
#include "DxLib.h"
#include "GameMain.h"
#include "ImageMgr.h"

ImageMgr::ImageMgr()
{
}

ImageMgr::~ImageMgr()
{
}
//ImageMgrに関して
//利用する際はヘッダをインクルードしてください
// 呼び出すところで
//	ImageMgr& im = ImageMgr::Instance();
//とやっておくと呼び出しやすくなります。
//
//	DrawGraph()の関数のハンドル部分に
//		im.ImageIdReturn("画像のファイル名",deleteするSCENE_TYPE)
//を指定してください。
//deleteするSCENE_TYPEは大体の場合次遷移するシーンを指定してください
//もしくはこれを何か変数に代入する形でも大丈夫だと思います。
//返り値はintです。
//
//呼び出された画像のdeleteSceneを設定してIDを返します
//もし、指定したファイルがまだロードされてなかった場合、ロードします
//
//このイメージマネージャは複数読み込みを防ぐものですね＾＾
//動きが遅くなったとか何かあればここが原因かもしれないので報告お願いします。

ImageMgr& ImageMgr::Instance()
{
	static ImageMgr imageMgr;
	return imageMgr;
}

int ImageMgr::ImageIdReturn(const std::string path, const SCENE_TYPE delScene)
{
	if (imageID.count(path) == 0) {
		imageID[path] = LoadGraph(path.c_str());
	}
	deleteTiming[path] = delScene;
	return imageID[path];
}

void ImageMgr::ImageManager(const SCENE_TYPE delScene)
{
	auto itr = deleteTiming.begin();

	while (itr != deleteTiming.end()) {
		if (itr->second == delScene) {
			DeleteGraph(imageID[itr->first]);

			imageID.erase(imageID.find(itr->first));
			itr = deleteTiming.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void ImageMgr::ImageIdAllDelete()
{
	for (auto itr = imageID.begin(); itr != imageID.end(); itr++) {
		DeleteGraph(itr->second);
	}
	imageID.erase(imageID.begin(),imageID.end());
	deleteTiming.erase(deleteTiming.begin(),deleteTiming.end());
}