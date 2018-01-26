#include <DxLib.h>
#include <list>
#include <memory>
#include <map>
#include <vector>
#include "Assert.h"
#include "Geometry.h"
#include "GimmickFactory.h"
#include "Gimmick.h"
#include "GimButton.h"
#include "GimDrop.h"
#include "SensorDoor.h"
#include "GimPull.h"
#include "Rope.h"

#include "Input.h"

using Gimmick_ptr = std::shared_ptr<Gimmick>;
typedef std::list<std::shared_ptr<Gimmick>> GimmickList_t;

GimmickFactory::GimmickFactory(Player& p, Rope& r) :_player(p), _rope(r)
{

}

GimmickFactory::~GimmickFactory()
{
}

std::weak_ptr<Gimmick>GimmickFactory::Create(CHIP_TYPE gt,Position2 pos) 
{
	switch (gt) {
	case CHIP_BLANK:
	case CHIP_N_CLIMB_WALL:
	case CHIP_CLIMB_WALL:
		break;
	case CHIP_TYPE::CHIP_DOOR:
		_gimmickList.push_back(Gimmick_ptr(new SensorDoor(pos,_player)));
		break;
	case CHIP_TYPE::CHIP_BUTTON_1:
		_gimmickList.push_back(Gimmick_ptr(new GimButton(pos,_player)));
		break;
	case CHIP_TYPE::CHIP_ROPE_ATTRACT:
		_gimmickList.push_back(Gimmick_ptr(new GimPull(pos,_rope,_player)));
		break;
	case CHIP_TYPE::CHIP_ROPE_FALL:
		_gimmickList.push_back(Gimmick_ptr(new GimDrop(pos,_rope,_player)));
		break;
	default:
		ASSERT();	//�z��O�������ꍇAssert���o��
		break;
	}
	return _gimmickList.back();
}
//���݁A����m�F�p��input�𗘗p���Ă��܂��B
//�f�o�b�N�p�Ƃ͂����ƂĂ������R�[�h�ɂȂ��Ă邯�ǐ\����Ȃ��B
void GimmickFactory::Updata(Input& input) 
{
	for (auto&gim : _gimmickList) {
		gim->Updata(input);
		gim->Updata();
	}
}
void GimmickFactory::Draw(Position2 offset)
{
	for (auto& gim : _gimmickList) {
		gim->Draw(offset);
	}
}
//���X�g��Ԃ�
GimmickList_t&  GimmickFactory::GimmickList() 
{
	return _gimmickList;
}