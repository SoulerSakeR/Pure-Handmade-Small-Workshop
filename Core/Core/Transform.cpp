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
	properties = vector<Property*>();
	localPosition = Vector2D::zero();
	properties.push_back(new Property("localPosition", &localPosition,Property::VECTOR2D,this));
	localRotation = 0.0f;
	properties.push_back(new Property("localRotation", &localRotation, Property::FLOAT,this));
	localScale = Vector2D::one();
	properties.push_back(new Property("localScale", &localScale, Property::VECTOR2D,this));
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
	if (GameEngine::get_instance().getSelectedGameObject() == gameObject)
	{
		GameEngine::get_instance().onPropertyChange(properties[0]);
	}
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
	if (GameEngine::get_instance().needToRefeshUI())
	{
		GameEngine::get_instance().onPropertyChange(properties[0]);
	}
}

void Transform::set_localRotation(float value)
{
	if (localRotation == value)
		return;
	localRotation = value;
	if (GameEngine::get_instance().needToRefeshUI())
	{
		GameEngine::get_instance().onPropertyChange(properties[1]);
	}
}

void Transform::set_localScale(Vector2D value)
{
	if (localScale == value)
		return;
	localScale = value;
	if (GameEngine::get_instance().needToRefeshUI())
	{
		GameEngine::get_instance().onPropertyChange(properties[2]);
	}
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

void Transform::set_property(Property* property, void* value)
{
	if (property->get_name() == "localPosition")
	{
		set_localPosition(Vector2D::fromString(*(string*)value));
	}
	else if (property->get_name() == "localRotation")
	{
		set_localRotation(*(float*)value);
	}
	else if (property->get_name() == "localScale")
	{
		set_localScale(Vector2D::fromString(*(string*)value));
	}
}
