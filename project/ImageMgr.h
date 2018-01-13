#pragma once
class ImageMgr
{
private:
	ImageMgr();
	ImageMgr(const ImageMgr&);

	std::map<std::string, int> imageID;					//画像のハンドル
	std::map < std::string, SCENE_TYPE> deleteTiming;	//画像の消去タイミング

public:
	~ImageMgr();
	static ImageMgr& Instance();
	void ImageManager(const SCENE_TYPE deleScene);							//画像の管理をする
	int ImageIdReturn(const std::string path, const SCENE_TYPE delScene);	//画像のハンドルを返す
	void ImageIdAllDelete();
};

