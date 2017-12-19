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
//ImageMgr�Ɋւ���
//���p����ۂ̓w�b�_���C���N���[�h���Ă�������
// �Ăяo���Ƃ����
//	ImageMgr& im = ImageMgr::Instance();
//�Ƃ���Ă����ƌĂяo���₷���Ȃ�܂��B
//
//	DrawGraph()�̊֐��̃n���h��������
//		im.ImageIdReturn("�摜�̃t�@�C����",delete����SCENE_TYPE)
//���w�肵�Ă��������B
//delete����SCENE_TYPE�͑�̂̏ꍇ���J�ڂ���V�[�����w�肵�Ă�������
//�������͂���������ϐ��ɑ������`�ł����v���Ǝv���܂��B
//�Ԃ�l��int�ł��B
//
//�Ăяo���ꂽ�摜��deleteScene��ݒ肵��ID��Ԃ��܂�
//�����A�w�肵���t�@�C�����܂����[�h����ĂȂ������ꍇ�A���[�h���܂�
//
//���̃C���[�W�}�l�[�W���͕����ǂݍ��݂�h�����̂ł��ˁO�O
//�������x���Ȃ����Ƃ���������΂�����������������Ȃ��̂ŕ񍐂��肢���܂��B

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