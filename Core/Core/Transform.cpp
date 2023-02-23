#include "Transform.h"

string& Transform::serialize()
{
	string result = "Transform\n";
	result.append(localPosition.toString());
	result.append("\n");
	result.append(to_string(localRotation));
	return result;
}


Transform::Transform(GameObject* gameObject)
{
	children = vector<Transform*>();
	parent = nullptr;
	this->gameObject = gameObject;
}

Transform::~Transform()
{
}

Vector2D Transform::getWorldPosition()
{
	if (parent != nullptr)
		return parent->localPosition + localPosition;
	return localPosition;
}

float Transform::getWorldRotation()
{
	if (parent != nullptr)
		return parent->localRotation + localRotation;
	return localRotation;
}

Transform* Transform::translate(Vector2D value)
{
	localPosition = localPosition + value;
	return this;
}


