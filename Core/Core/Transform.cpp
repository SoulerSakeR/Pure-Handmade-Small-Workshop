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
	properties["Enabled"]->is_editable = false;
	auto localPosition = new Property("Local Position", &this->localPosition, Property::VECTOR2D, this);
	localPosition->set_property_func<Vector2D>(&Transform::get_localPosition, &Transform::set_localPosition, this);
	properties.emplace(localPosition);
	auto localRotation = new Property("Local Rotation", &this->localRotation, Property::FLOAT, this);
	localRotation->set_property_func<float>(&Transform::get_localRotation, &Transform::set_localRotation, this);
	properties.emplace(localRotation);
	auto localScale = new Property("Local Scale", &this->localScale, Property::VECTOR2D, this);
	localScale->set_property_func<Vector2D>(&Transform::get_localScale, &Transform::set_localScale, this);
	properties.emplace(localScale);
}

Transform::~Transform()
{
	
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
	onPropertyChanged(properties["Local Position"]);
	return this;
}

Transform* Transform::translate1(float x, float y)
{
	localPosition.y = localPosition.y + y;
	localPosition.x = localPosition.x + x;
	onPropertyChanged(properties["Local Position"]);
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
	onPropertyChanged(properties["Local Position"]);
}

void Transform::set_localRotation(float value)
{
	if (localRotation == value)
		return;
	localRotation = value;
	onPropertyChanged(properties["Local Rotation"]);
}

void Transform::set_localScale(Vector2D value)
{
	if (localScale == value)
		return;
	localScale = value;
	onPropertyChanged(properties["Local Scale"]);
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

