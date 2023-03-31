#include "Transform.h"
#include <Core/Core/GameObject.h>

using namespace std;

void Transform::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	str.appendLine(localPosition.tostring());
	str.appendLine(to_string(localRotation));
	str.appendLine(localScale.tostring());
}


Transform::Transform(GameObject* gameObject):Component(gameObject)
{
	componentType = TRANSFORM;
	children = vector<Transform*>();
	parent = nullptr;
}

Transform::~Transform()
{
	// TODO: 析构函数
}

Vector2D Transform::getWorldPosition()
{
	if (this->parent != nullptr)
		return parent->getWorldPosition() + this->localPosition;
	return this->localPosition;
}

float Transform::getWorldRotation()
{
	if (parent != nullptr)
		return parent->getWorldRotation() + localRotation;
	return localRotation;
}

Vector2D Transform::getWorldScale()
{
	if (parent != nullptr)
		return parent->getWorldScale() + localScale;
	return localScale;
}

Transform* Transform::translate(Vector2D value)
{
	localPosition = localPosition + value;
	return this;
}



void Transform::deserialize(stringstream& ss)
{
	string s;
	getline(ss,s);	
	size_t index = s.find(',');
	float x = stof(s.substr(0,index-1));
	float y = stof(s.substr(index+1,s.size()-1));
	localPosition = Vector2D(x,y);
	getline(ss,s);
	localRotation = stof(s);
	getline(ss, s);
	index = s.find(',');
	x = stof(s.substr(0, index - 1));
	y = stof(s.substr(index + 1, s.size() - 1));
	localScale = Vector2D(x, y);
}

void Transform::reset()
{
	localPosition = Vector2D::zero();
	localRotation = 0.0f;
	localScale = Vector2D::zero();
}
