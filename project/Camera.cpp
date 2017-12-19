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

// �ݽ�׸�
Camera::Camera()
{
	_player = nullptr;
	_mapCtl = nullptr;
	this->SetGameScrSize({ SCREEN_SIZE_X,SCREEN_SIZE_Y });
	this->SetCamAreaOffset({ MAP_CHIP_SIZE_X * 6,MAP_CHIP_SIZE_Y * 6 });
}

// �޽�׸�
Camera::~Camera()
{
}

//// �����ɕ`�悵�Ăق�����ǉ�
//bool Camera::AddObject(Object * obj)
//{
//	return true;
//}

Camera * Camera::GetInstance(void)
{
	// null�̏ꍇnew����
	if (cam_ptr == nullptr)
	{
		cam_ptr = new Camera();
	}

	return cam_ptr;
}

// ���ޯĂ̌���
bool Camera::SetTarget(Player* player)
{
	_player = player;
	return _player;
}

// ϯ�ߺ��۰ق̐ݒ�
bool Camera::SetMapCtl(MapCtl* mapCtl) {
	_mapCtl = mapCtl;
	return _mapCtl;
}

// �X�V
void Camera::Update()
{
	//��ڲ԰�̍��W�擾
	auto pPos = _player->GetPos();

	// �����̂��炳�Ȃ��Ƃ�
	if (pPos.x < CAMERA_MOVE_POS) 
	{
		tmpPos.x = CAMERA_LEFT_POS;	
	}
	// �^�񒆂̂��炷�Ƃ�
	else if ((pPos.x >= CAMERA_MOVE_POS) && (pPos.x <  CAMERA_MOVE_LIMIT_POS)) 
	{
		tmpPos.x = pPos.x - CAMERA_MOVE_POS;
	}
	// �E���̂��炳�Ȃ��Ƃ�
	else if (pPos.x >= CAMERA_MOVE_LIMIT_POS) 
	{
		tmpPos.x = CAMERA_RIGHT_POS;
	}
}

// �Q�[����ʂɉf��͈̓T�C�Y
bool Camera::SetGameScrSize(Position2 gmScrSize)
{
	GameScrSize = gmScrSize;

	return true;
}

// �̾�ď��
bool Camera::SetCamAreaOffset(Position2 offset)
{
	CamOffset = offset;

	return true;
}

// ��׻��ނ̎擾
Position2 Camera::GetGameScrSize(void)
{
	return GameScrSize;
}

// �`��
void Camera::Draw()
{
	//ϯ�߂̕`��
	//_mapCtl->Draw(tmpPos);

	//��ڲ԰�̕`��
	//_player->Draw(tmpPos);

	//�����ɑ��̂�������Ă����Ă�
	//_rope->DrawRopeRect(tmpPos);

	//���X�g�����ĕ\�����������Ƀ\�[�g���ā@Draw����
	//���X�g�悤�̊֐�����Ă������肷��ƈ�����Objg*�@_obj�݂����Ȃ̂�public�ł��΂悫�悫
}

Camera::Camera(const Camera &)
{
}

Position2& Camera::ReturnOffset()
{
	return tmpPos;
}