#include "Transform.h"

string Transform::serialize()
{
	return string();
}

Transform::Transform()
{
	children = vector<Transform*>();
	parent = NULL;
}

Transform::~Transform()
{
}

Vector2D Transform::getWorldPosition()
{
	if (parent != NULL)
		return parent->localPosition + localPosition;
	return localPosition;
}

float Transform::getWorldRotation()
{
	if (parent != NULL)
		return parent->localRotation + localRotation;
	return localRotation;
}

Transform* Transform::translate(Vector2D value)
{
	localPosition = localPosition + value;
	return this;
}
