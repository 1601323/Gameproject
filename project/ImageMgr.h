#pragma once
class ImageMgr
{
private:
	ImageMgr();
	ImageMgr(const ImageMgr&);

	std::map<std::string, int> imageID;					//�摜�̃n���h��
	std::map < std::string, SCENE_TYPE> deleteTiming;	//�摜�̏����^�C�~���O

public:
	~ImageMgr();
	static ImageMgr& Instance();
	void ImageManager(const SCENE_TYPE deleScene);							//�摜�̊Ǘ�������
	int ImageIdReturn(const std::string path, const SCENE_TYPE delScene);	//�摜�̃n���h����Ԃ�
	void ImageIdAllDelete();
};

