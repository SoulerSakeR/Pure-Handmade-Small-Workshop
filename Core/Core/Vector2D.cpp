#include <string>

#include "Vector2D.h"

using namespace std;

Vector2D Vector2D::operator+(const Vector2D& b)
{
	return Vector2D(x+b.x,y+b.y);
}

Vector2D Vector2D::zero()
{
	return Vector2D(0.0f,0.0f);
}

string Vector2D::tostring()
{
	string result = std::to_string(x);
	result.append(",");
	result.append(std::to_string(y));
	return result;
}

