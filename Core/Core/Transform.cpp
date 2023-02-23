#include "Transform.h"

string& Transform::serialize()
{
	string result = "Transform\n";
	result.append(localPosition.toString());
	result.append("\n");
	result.append(to_string(localRotation));
	return result;
}

Transform::Transform()
{
	children = vector<Transform*>();
	parent = nullptr;
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



Transform* Transform::deserialize(stringstream ss)
{
	string s;
	getline(ss,s);	
	Transform* transform = new Transform();
	transform->children = vector<Transform*>();
	transform->parent = nullptr;
	size_t index = s.find(',');
	float x = stof(s.substr(0,index-1));
	float y = stof(s.substr(index+1,s.size()-1));
	transform->localPosition = Vector2D(x,y);
	getline(ss,s);
	float localRotation = stof(s);
    return transform;
}
