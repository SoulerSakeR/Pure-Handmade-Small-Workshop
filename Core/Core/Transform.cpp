#include "Transform.h"
#include <Core/Core/GameObject.h>
#include <Core/SystemStatus/GameEngine.h>

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
	reset();
	properties.emplace("localPosition", new Property("localPosition", &localPosition, Property::VECTOR2D, this));
	properties.emplace("localRotation",new Property("localRotation", &localRotation, Property::FLOAT, this));
	properties.emplace("localScale",new Property("localScale", &localScale, Property::VECTOR2D, this));	
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

float Transform::getWorldPositionX()
{
	if (this->parent != nullptr)
		return parent->getWorldPosition().x + this->localPosition.x;
	return this->localPosition.x;
}


float Transform::getWorldPositionY()
{
	if (this->parent != nullptr)
		return parent->getWorldPosition().y + this->localPosition.y;
	return this->localPosition.y;
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
		return parent->getWorldScale().Scale(localScale);
	return localScale;
}

Transform* Transform::translate(Vector2D value)
{
	localPosition = localPosition + value;
	onPropertyChange(properties["localPosition"]);
	return this;
}

Transform* Transform::translate1(float x, float y)
{
	localPosition.y = localPosition.y + y;
	localPosition.x = localPosition.x + x;
	onPropertyChange(properties["localPosition"]);
	return this;
}

Vector2D Transform::get_localPosition()
{
	return localPosition;
}

float Transform::get_localRotation()
{
	return localRotation;
}

Vector2D Transform::get_localScale()
{
	return localScale;
}

void Transform::set_localPosition(Vector2D value)
{
	if(localPosition == value)
		return;
	localPosition = value;
	onPropertyChange(properties["localPosition"]);
}

void Transform::set_localRotation(float value)
{
	if (localRotation == value)
		return;
	localRotation = value;
	onPropertyChange(properties["localRotation"]);
}

void Transform::set_localScale(Vector2D value)
{
	if (localScale == value)
		return;
	localScale = value;
	onPropertyChange(properties["localScale"]);
}



void Transform::deserialize(stringstream& ss)
{
	string s;
	getline(ss,s);	
	localPosition = Vector2D::fromString(s);
	getline(ss,s);
	localRotation = stof(s);
	getline(ss, s);
	localScale = Vector2D::fromString(s);
}

void Transform::reset()
{
	localPosition = Vector2D::zero();
	localRotation = 0.0f;
	localScale = Vector2D::one();
}

void Transform::set_property(Property* property, void* value)
{
	if (property->get_name() == "localPosition")
	{
		set_localPosition(*(Vector2D*)value);
	}
	else if (property->get_name() == "localRotation")
	{
		set_localRotation(*(float*)value);
	}
	else if (property->get_name() == "localScale")
	{
		set_localScale(*(Vector2D*)value);
	}
}
