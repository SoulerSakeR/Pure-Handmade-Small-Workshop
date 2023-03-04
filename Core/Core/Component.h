#pragma once

#include <sstream>

#include "ISerializable.h"

class GameObject;

enum ComponentType {TRANSFORM,IMAGE,CAMERA};

class Component : public ISerializable
{
public:
	GameObject* gameObject;
	ComponentType componentType;
	virtual void serialize(PHString&)=0;
	virtual void deserialize(std::stringstream& ss) = 0;
	Component() {
		gameObject = nullptr;
	};
	~Component() {
		//TODO: 析构函数
	};
};