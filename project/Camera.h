#pragma once

#include <memory>
#include "Object.h"
#include "Player.h"

using namespace std;

class Player;
class MapCtl;

class Camera :
	public Object
{
public:
	Position2 tmpPos;
	Position2 GetGameScrSize(void);
	bool SetTarget(Player* _player);			// ���ޯĂ����߂�֐�
	bool SetMapCtl(MapCtl* mapCtl);				// ϯ�ߺ��۰ٓ����֐�
	bool SetGameScrSize(Position2 gmScrSize);	// �ްщ�ʂɉf��͈ͻ���
	bool SetCamAreaOffset(Position2 offset);	// �̾�ď���ێ�����										
	void Update();
	void Draw();
	Camera(const Camera&);						// �V���O���g��
	const Camera& operator=(const Camera&);
	//Camera(Player* player);
	virtual~Camera();
	//bool AddObject(Object* obj);				// �J�����Ɉڂ�������ǉ��p
	static Camera* GetInstance(void);

	Position2& ReturnOffset();					//offset�̒l��Ԃ��܂�
private:
	Camera();
	MapCtl* _mapCtl;
	Player* _player;
	Rope* _rope;
	Position2 CamOffset;
	Position2 GameScrSize;
	static Camera* cam_ptr;		// �J�����̃|�C���^
};

