#pragma once

struct Vec2
{
	float x, y;

public:
	Vec2(float _x, float _y)
		:x(_x), y(_y)
	{}

	Vec2() :x(0.f), y(0.f) {

	}
};
