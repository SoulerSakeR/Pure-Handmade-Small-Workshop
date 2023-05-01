#pragma once
#include <iostream>
#include <qvectornd.h>

struct Vector2D
{
public:
	float x; 
	float y; 
	Vector2D(float x=0.0f, float y=0.0f) :x(x), y(y) {};
	Vector2D(const QVector2D& vec) :x(vec.x()), y(vec.y()) {};
	Vector2D operator+(const Vector2D& b);
	Vector2D operator-(const Vector2D& b);
	Vector2D operator/(float b);
	Vector2D& operator=(const Vector2D& b);
	bool operator!=(const Vector2D& b) const;
	bool operator==(const Vector2D& b) const;
	Vector2D operator*(float b);

	static const Vector2D zero();
	static const Vector2D one();
	static const Vector2D fromString(const std::string& src);
	static const Vector2D fromQString(const QString& src);

	Vector2D& Scale(const Vector2D& scale);
	std::string tostring() const;
	std::string tostringInt();
	QString toQString();
	QString toQStringInt();
	QVector3D toQVector3D(float z = 0.f);
	QVector2D toQVector2D();
}; 

