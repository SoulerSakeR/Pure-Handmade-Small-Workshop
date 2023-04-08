#pragma once
#include <iostream>
#include <qvectornd.h>

struct Vector2D
{
public:
	float x; 
	float y; 
	Vector2D(float x=0.0f, float y=0.0f) :x(x), y(y) {};
	Vector2D operator+(const Vector2D& b);
	Vector2D& operator=(const Vector2D& b);
	bool operator==(const Vector2D& b);

	static const Vector2D zero();
	static const Vector2D one();
	static const Vector2D fromString(const std::string& src);
	std::string tostring();
	QVector3D toQVector3D(float z = 0.f);
	QVector2D toQVector2D();
}; 

