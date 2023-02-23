#pragma once
#include "ISerializable.h"
#include "GameObject.h"

enum ComponentType {TRANSFORM};

class Component : public ISerializable
{
public:
	GameObject* gameObject;  
	virtual string& serialize()=0;
	Component() {};
	~Component() {};
};