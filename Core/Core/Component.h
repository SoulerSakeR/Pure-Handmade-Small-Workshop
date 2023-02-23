#pragma once

#include <sstream>

#include "ISerializable.h"

enum ComponentType {TRANSFORM};

class Component : public ISerializable
{
public:
	GameObject* gameObject;  
	virtual std::string& serialize()=0;
	virtual Component* deserialize(std::stringstream ss) = 0;
	Component() {};
	~Component() {};
};