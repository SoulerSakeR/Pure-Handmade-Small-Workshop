#include <string>

#include "Vector2D.h"
#include <qvectornd.h>

using namespace std;

Vector2D Vector2D::operator+(const Vector2D& b)
{
	return Vector2D(x+b.x,y+b.y);
}

Vector2D Vector2D::operator-(const Vector2D& b)
{
	return Vector2D(x - b.x, y - b.y);
}

Vector2D Vector2D::operator/(float b)
{
	return Vector2D(x / b, y / b);
}

Vector2D& Vector2D::operator=(const Vector2D& b)
{
	x = b.x;
	y = b.y;
	return *this;
}

bool Vector2D::operator!=(const Vector2D& b) const
{
	if (x != b.x || y != b.y)
		return true;
	return false;
}

Vector2D Vector2D::operator*(float b)
{
	return Vector2D(x * b, y * b);
}

bool Vector2D::operator==(const Vector2D& b) const
{
	return x == b.x && y == b.y;
}

const Vector2D Vector2D::zero()
{
	return Vector2D(0.0f,0.0f);
}

const Vector2D Vector2D::one()
{
	return Vector2D(1.f,1.f);
}

const Vector2D Vector2D::fromString(const std::string& src)
{
	size_t index = src.find(',');
	float x = stof(src.substr(0, index));
	float y = stof(src.substr(index + 1));
	return Vector2D(x, y);
}

const Vector2D Vector2D::fromQString(const QString& src)
{
	return fromString(src.toStdString());
}

Vector2D& Vector2D::Scale(const Vector2D& scale)
{
	x = x * scale.x;
	y = y * scale.y;
	return *this;
}

string Vector2D::tostring()
{
	string result = std::to_string(x);
	result.append(",");
	result.append(std::to_string(y));
	return result;
}

QString Vector2D::toQString()
{
	return QString::fromStdString(tostring());
}

QVector3D Vector2D::toQVector3D(float z)
{
	return QVector3D(x,y,z);
}

QVector2D Vector2D::toQVector2D()
{
	return QVector2D(x,y);
}

