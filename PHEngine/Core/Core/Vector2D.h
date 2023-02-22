#pragma once
struct Vector2D
{
public:
	float x; //зјБъX
	float y; //зјБъY
	Vector2D(float x, float y) :x(x), y(y) {};
	Vector2D operator+(const Vector2D& b);
	static Vector2D zero();
}; 

