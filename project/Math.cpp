#include <cmath>
#include "Math.h"
#include "Geometry.h"
#include "Input.h"

//度数法を弧度法にして返す
//度数法をangleに渡す
//float型で弧度法で返す
float AngleRad(float angle)
{
	return (angle * 3.1f) / 180.f;
}

//弧度法を度数法にして返す
//弧度法をradに渡す
//float型で度数法で返す
float RadAngle(float rad)
{
	return (rad * 180.f) / 3.1f;
}

//ベクトルの長さを返す
//POS型をベクトルの長さに見なして渡す
//float型で長さを返す
float  VecLen(Position2 vec)
{
	return pow((vec.x*vec.x) + (vec.y*vec.y), 0.5f);
}

//２つのベクトルの内積を返す
//2つのベクトルPOSを渡す
//float型で内積を返す
float  DotProduct(Position2 pos1, Position2 pos2)
{
	return pos1.x*pos2.x + pos1.y*pos2.y;
}

// 2のﾍﾞｸﾄﾙのX,Yのなす角を求める
float AngleOf2Vector(Position2 pos1, Position2 pos2)
{
	// 長さ
	float lengthA = VecLen(pos1);
	float lengthB = VecLen(pos2);

	// cosθ(内積ﾍﾞｸﾄﾙ使用)
	if (lengthA != 0.0f && lengthB != 0.0f) {
		float cos_sita = DotProduct(pos1, pos2) / (lengthA*lengthB);
		// cosθ→ θ
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

//円と点のあたり判定(比較の仕方が外円と中の点との比較なので注意)
//円の座標をcircle,半径をrange,点の座標をdotに入れる
//当たっていればtrue 当たっていなければfalseを返す
bool Circle_Dot(Position2 circle, float range, Position2 dot)
{
	if (pow((circle.x - dot.x), 2.0f) + pow((circle.y - dot.y), 2.0f) >= pow(range, 2.0f)) {
		return true;
	}
	else {
		return false;
	}
}