#include <DxLib.h>
#include <iostream>
#include "Geometry.h"
#include "Input.h"
#include "GameMain.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "ResultScene.h"
#include "ModelMgr.h"
#include "ImageMgr.h"

#define NUM_X (74)
#define NUM_Y (100)


ResultScene::ResultScene()
{
	_updater = &ResultScene::NormalUpdata;
	_minSensingValueL = SV_HIGH;
	_mode = JUMP_SELECT;
	clearFlag = false;
	selectFlag = false;
	nowNum = 0;
	dirMoveCnt = 0;
	_modelmgr = ModelMgr::Instance();
	//���f���ǂݍ���
	playerModelHandle = MV1LoadModel("player_model/player.pmx");
	medicineHandle = MV1LoadModel("gimmick_model/�t���X�R/�ے�t���X�R.pmx");
	//�A�j���[�V�������A�^�b�`+�����Ԃ̐ݒ�
	AnimIndexH = MV1AttachAnim(playerModelHandle, ACTION_HAPPY, -1, false);
	AnimTotalTimeH = MV1GetAttachAnimTotalTime(playerModelHandle, AnimIndexH);

	AnimIndexS = MV1AttachAnim(playerModelHandle, ACTION_AOONI, -1, false);
	AnimTotalTimeS = MV1GetAttachAnimTotalTime(playerModelHandle, AnimIndexS);
	//��̃e�N�X�`����index���擾
	textureIndex = MV1GetMaterialDifMapTexture(playerModelHandle, 1);
}

ResultScene::~ResultScene()
{
}

void ResultScene::NormalUpdata(Input* input)
{
	GameMain& gm = GameMain::Instance();
	key = input->GetInput(1).key;
	lastKey = input->GetLastKey();
	inpInfo = input->GetInput(1);
	_rtData = gm.GetResultData();
	if (gm.GetResultData().goalFlag == true && gm.GetResultData().midFlag) {
		GameClear();
		clearFlag = true;
	}
	else {
		GameOver();
		clearFlag = true;
	}
	Select(input);
	Draw();
#ifdef _DEBUG
	//DrawString(10, 10, "���U���g�ɑJ�ڂ��Ă��I", 0xffffff);
#endif
	if (key.keybit.A_BUTTON && !lastKey.keybit.A_BUTTON) {
		if (nowNum == JUMP_SELECT) {
			dirMoveCnt = 0;
			gm.Instance().ChangeScene(new SelectScene());
		}
		else if (nowNum == JUMP_TITLE) {
			dirMoveCnt = 0;
			gm.Instance().ChangeScene(new TitleScene());
		}
		else if (nowNum == JUMP_RETRY) {
			dirMoveCnt = 0;
			gm.Instance().ChangeScene(new GameScene());
		}
		else {
		}
	}
}
//�N���A�ƃQ�[���I�[�o�[�ɕ����Ă���
void ResultScene::GameClear()
{
	GameMain& gm = GameMain::Instance();
	gm.NewDataSet();
	gm.BestDataSet();
	//DrawFormatString(100, 150, 0xffffff, "%d", _rtData.goalTime);

#ifdef _DEBUG
	//DrawString(100, 100, "Clear", 0xff00ff);
#endif
}
void ResultScene::GameOver()
{
#ifdef _DEBUG
	//DrawString(100, 100, "GameOver", 0xff00ff);
#endif
}
void ResultScene::Select(Input*  input)
{
	if (clearFlag == true) {
		if (nowNum <= 0) nowNum = 1;//�N���A�̏ꍇ����nowNum��1�ɂ��Ƃ�

		if (inpInfo.num >= 1) {
			if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL	&&
				selectFlag == false) {
				nowNum--;
				if (nowNum <= 0) {
					nowNum = JUMP_MAX - 1;
				}
				selectFlag = true;
			}
			else if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL&&
				selectFlag == false) {
				nowNum++;
				if (nowNum >= JUMP_MAX) {
					nowNum = 1;
				}
				selectFlag = true;
			}
			else if (!((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) &&
				!((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
					inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL)) {
				selectFlag = false;
			}
			else {
				nowNum = nowNum;
			}
		}
		else {

			//�X�e�[�W�I��
			if (inpInfo.key.keybit.R_UP_BUTTON && !lastKey.keybit.R_UP_BUTTON) {
				nowNum--;
				if (nowNum <= 0) {
					nowNum = JUMP_MAX-1;
				}
			}
			else if (inpInfo.key.keybit.R_DOWN_BUTTON && !lastKey.keybit.R_DOWN_BUTTON) {
				nowNum++;
				if (nowNum >= JUMP_MAX) {
					nowNum = 1;
				}
			}
			else {
			}
		}
	}
	else if (clearFlag == false) {
		if (inpInfo.num >= 1) {
			if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL	&&
				selectFlag == false) {
				nowNum--;
				if (nowNum < 0) {
					nowNum = JUMP_MAX - 1;
				}
				selectFlag = true;
			}
			else if ((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL&&
				selectFlag == false) {
				nowNum++;
				if (nowNum >= JUMP_MAX) {
					nowNum = 0;
				}
				selectFlag = true;
			}
			else if (!((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_UP) &&
				inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL) &&
				!((input->GetStickDir(inpInfo.L_Stick.lstick) == SD_DOWN) &&
					inpInfo.L_Stick.L_SensingFlag >= _minSensingValueL)) {
				selectFlag = false;
			}
			else {
				nowNum = nowNum;
			}
		}
		else {

			//�X�e�[�W�I��
			if (inpInfo.key.keybit.R_UP_BUTTON && !lastKey.keybit.R_UP_BUTTON) {
				nowNum--;
				if (nowNum < 0) {
					nowNum = JUMP_MAX - 1;
				}
			}
			else if (inpInfo.key.keybit.R_DOWN_BUTTON && !lastKey.keybit.R_DOWN_BUTTON) {
				nowNum++;
				if (nowNum >= JUMP_MAX) {
					nowNum = 0;
				}
			}
			else {
			}
		}
	}
	else {
	}
}
void ResultScene::Draw()
{
	dirMoveCnt++;
	ImageMgr& im = ImageMgr::Instance();
	if (clearFlag == true) {
		//�w�i
		DrawGraph(0, 0, im.ImageIdReturn("��image/result �̉��ł�/Result.png", SCENE_TITLE), true);
		DrawGraph(0, 0, im.ImageIdReturn("��image/result �̉��ł�/Result2.png", SCENE_TITLE), true);
		DrawGraph(150, 10, im.ImageIdReturn("��image/UI/clear.png", SCENE_TITLE), true);

		//�v���C���[ happy
		//�A�j���[�V�����̃t���[����i�߂�
		AnimNowTimeH += 0.5f;
		//���݂̃A�j���[�V�������ő�t���[���܂ł������烋�[�v����
		if (AnimNowTimeH >= AnimTotalTimeH)
		{
			AnimNowTimeH = 0;
		}
		//���f���̉�]�p�x�̐ݒ�(���W�A��)
		MV1SetRotationXYZ(playerModelHandle, VGet(0.f, AngleRad(45.f), 0.f));
		//�A�j���[�V�������A�^�b�`
		MV1SetAttachAnimTime(playerModelHandle, AnimIndexH, AnimNowTimeH);
		//���f����pos��ݒ�+���[���h���W����X�N���[���֕ϊ�
		MV1SetPosition(playerModelHandle, ConvWorldPosToScreenPos(VGet(600.f, 600, 0.f)));
		//���f���̊g��k���l�̐ݒ�
		MV1SetScale(playerModelHandle, VGet(4.0f, 4.0f, 4.0f));
		//��̃e�N�X�`�����Ί�̕��ɕύX
		MV1SetTextureGraphHandle(playerModelHandle, textureIndex, im.ImageIdReturn("player_model/face2.png", SCENE_TITLE), FALSE);
		//���f����֊s��0.0f�ŕ`�� 
		_modelmgr->Draw(playerModelHandle, 0.0f);

		//�X�R�A�^�C���̉摜�ǂݍ��݂ƕ\��
		numberImage = im.ImageIdReturn("��image/UI/NewNum.png", SCENE_RESULT);
		second = _rtData.goalTime % 10;
		tenex = (_rtData.goalTime / 10) % 10;
		hunex = _rtData.goalTime / 100;

		DrawRectExtendGraph(400, 150, 400 + (NUM_X / 2), 200 + (NUM_Y / 2), NUM_X * second, 0, NUM_X, NUM_Y, numberImage, true);
		DrawRectExtendGraph(400 - (NUM_X / 2) * 1, 150, 400 + (NUM_X / 2) - (NUM_X / 2), 200 + (NUM_Y / 2), NUM_X * tenex, 0, NUM_X, NUM_Y, numberImage, true);
		DrawRectExtendGraph(400 - (NUM_X / 2) * 2, 150, 400 + (NUM_X / 2) - (NUM_X / 2) * 2, 200 + (NUM_Y / 2), NUM_X * hunex, 0, NUM_X, NUM_Y, numberImage, true);

		//�Z���N�g�摜
		DrawGraph(500, 510, im.ImageIdReturn("��image/Bar_Menu/Select.png", SCENE_TITLE), true);
		DrawGraph(500, 430, im.ImageIdReturn("��image/Bar_Menu/Title.png", SCENE_TITLE), true);

	}
	else if (clearFlag == false) {
		//�w�i
		DrawGraph(0, 0, im.ImageIdReturn("��image/Over/GameOver.png", SCENE_TITLE), true);

		//�v���C���[
		AnimNowTimeS += 0.5f;
		if (AnimNowTimeS >= AnimTotalTimeS)
		{
			AnimNowTimeS = 0;
		}
		MV1SetAttachAnimTime(playerModelHandle, AnimIndexS, AnimNowTimeS);
		MV1SetPosition(playerModelHandle, ConvWorldPosToScreenPos(VGet(300.f, 580, 0.f)));
		MV1SetScale(playerModelHandle, VGet(5.0f, 5.0f, 5.0f));
		MV1SetTextureGraphHandle(playerModelHandle, textureIndex, im.ImageIdReturn("player_model/cryFace.png", SCENE_TITLE), FALSE);

		//���f����֊s��0.0f�ŕ`�� 
		_modelmgr->Draw(playerModelHandle, 0.0f);

		DrawGraph(0, 0, im.ImageIdReturn("��image/Over/Fence.png", SCENE_TITLE), true);

		//�Z���N�g�摜
		DrawGraph(500, 510, im.ImageIdReturn("��image/Bar_Menu/Select.png", SCENE_TITLE), true);
		DrawGraph(500, 430, im.ImageIdReturn("��image/Bar_Menu/Title.png", SCENE_TITLE), true);
		DrawGraph(510, 350, im.ImageIdReturn("��image/Bar_Menu/Retry.png", SCENE_TITLE), true);

	}

	//����nowNum�̒l�ɉ����ĕ`��
	DrawGraph(420 - abs(30 - (200 + (dirMoveCnt / 2 % 60)) % 59), dirNumY, im.ImageIdReturn("��image/UI/dirset1.png", SCENE_TITLE), true);
	switch (nowNum) {
	case 0:
		dirNumY = clearFlag ? 430 : 350;
		break;
	case 1:
		dirNumY = 430;
		break;
	case 2:
		dirNumY = 510;
		break;
	default:
		break;
	}
}
SCENE_TYPE ResultScene::GetScene()
{
	return SCENE_RESULT;
}
void ResultScene::Updata(Input* input)
{
	(this->*_updater)(input);
}