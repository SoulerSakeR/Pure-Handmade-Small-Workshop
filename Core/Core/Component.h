#pragma once

#include <sstream>

#include "ISerializable.h"
#include <vector>
#include "Property.h"

class GameObject;

enum ComponentType {UNKNOWN,TRANSFORM,IMAGE,CAMERA,SCRIPT};

class Component : public ISerializable
{
public:
	GameObject* gameObject;
	ComponentType componentType;
	std::vector<Property*> properties;

	void set_enabled(bool value);
	typedef ComponentType customType;
	virtual void serialize(PHString&)=0;
	virtual void deserialize(std::stringstream& ss) = 0;
	virtual void reset() = 0;
	virtual void set_property(Property* property, void* value) = 0;
	std::string getName(ComponentType type);
	Component(GameObject* gameObj) {
		gameObject = gameObj;
		enabled = true;
		componentType = UNKNOWN;
	};
	~Component() {
		//TODO: 析构函数
	};
private:
	bool enabled;
};