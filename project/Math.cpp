#include <cmath>
#include "Math.h"
#include "Geometry.h"
#include "Input.h"

//�x���@���ʓx�@�ɂ��ĕԂ�
//�x���@��angle�ɓn��
//float�^�Ōʓx�@�ŕԂ�
float AngleRad(float angle)
{
	return (angle * 3.1f) / 180.f;
}

//�ʓx�@��x���@�ɂ��ĕԂ�
//�ʓx�@��rad�ɓn��
//float�^�œx���@�ŕԂ�
float RadAngle(float rad)
{
	return (rad * 180.f) / 3.1f;
}

//�x�N�g���̒�����Ԃ�
//POS�^���x�N�g���̒����Ɍ��Ȃ��ēn��
//float�^�Œ�����Ԃ�
float  VecLen(Position2 vec)
{
	return pow((vec.x*vec.x) + (vec.y*vec.y), 0.5f);
}

//�Q�̃x�N�g���̓��ς�Ԃ�
//2�̃x�N�g��POS��n��
//float�^�œ��ς�Ԃ�
float  DotProduct(Position2 pos1, Position2 pos2)
{
	return pos1.x*pos2.x + pos1.y*pos2.y;
}

// 2���޸�ق�X,Y�̂Ȃ��p�����߂�
float AngleOf2Vector(Position2 pos1, Position2 pos2)
{
	// ����
	float lengthA = VecLen(pos1);
	float lengthB = VecLen(pos2);

	// cos��(�����޸�َg�p)
	if (lengthA != 0.0f && lengthB != 0.0f) {
		float cos_sita = DotProduct(pos1, pos2) / (lengthA*lengthB);
		// cos�Ɓ� ��
		float sita = acos(cos_sita);
		if (pos2.y < pos1.y) {
			return (AngleRad(360.f) - sita);
		}
		else {
			return sita;
		}
	}
	return 0.0;
}

//�~�Ɠ_�̂����蔻��(��r�̎d�����O�~�ƒ��̓_�Ƃ̔�r�Ȃ̂Œ���)
//�~�̍��W��circle,���a��range,�_�̍��W��dot�ɓ����
//�������Ă����true �������Ă��Ȃ����false��Ԃ�
bool Circle_Dot(Position2 circle, float range, Position2 dot)
{
	if (pow((circle.x - dot.x), 2.0f) + pow((circle.y - dot.y), 2.0f) >= pow(range, 2.0f)) {
		return true;
	}
	else {
		return false;
	}
}