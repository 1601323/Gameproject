#pragma once
#include "Geometry.h"

float AngleRad(float angle);

float RadAngle(float rad);

float VecLen(Position2 vec);

float DotProduct(Position2 pos1, Position2 pos2);

float AngleOf2Vector(Position2 pos1, Position2 pos2);

bool Circle_Dot(Position2 circle, float range, Position2 dot);
