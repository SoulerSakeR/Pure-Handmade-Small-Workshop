#pragma once

#include <sstream>
#include "ISerializable.h"
#include "GameObject.h"

enum ComponentType {TRANSFORM};

class Component : public ISerializable
{
public:
	GameObject* gameObject;  
	virtual string& serialize()=0;
	virtual Component* deserialize(stringstream ss) = 0;
	Component() {};
	~Component() {};
};