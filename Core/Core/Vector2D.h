#pragma once

using namespace std;
struct Vector2D
{
public:
	float x; //坐标X
	float y; //坐标Y
	Vector2D(float x, float y) :x(x), y(y) {};
	Vector2D operator+(const Vector2D& b);
	static Vector2D zero();
	string toString();
}; 

