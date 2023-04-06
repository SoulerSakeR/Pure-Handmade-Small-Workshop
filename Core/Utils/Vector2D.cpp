#include <string>

#include "Vector2D.h"
#include <qvectornd.h>

using namespace std;

Vector2D Vector2D::operator+(const Vector2D& b)
{
	return Vector2D(x+b.x,y+b.y);
}

Vector2D& Vector2D::operator=(const Vector2D& b)
{
	x = b.x;
	y = b.y;
	return *this;
}

const Vector2D Vector2D::zero()
{
	return Vector2D(0.0f,0.0f);
}

const Vector2D Vector2D::one()
{
	return Vector2D(1.f,1.f);
}

string Vector2D::tostring()
{
	string result = std::to_string(x);
	result.append(",");
	result.append(std::to_string(y));
	return result;
}

QVector3D Vector2D::toQVector3D(float z)
{
	return QVector3D(x,y,z);
}

QVector2D Vector2D::toQVector2D()
{
	return QVector2D(x,y);
}

