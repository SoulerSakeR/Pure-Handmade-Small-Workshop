#pragma once
#include <iostream>

struct Vector2D
{
public:
	float x; //坐标X
	float y; //坐标Y
	Vector2D(float x=0.0f, float y=0.0f) :x(x), y(y) {};
	Vector2D operator+(const Vector2D& b);
	Vector2D& operator=(const Vector2D& b);
	const static Vector2D zero();
	std::string tostring();
}; 

