#pragma once

#include <sstream>

#include "ISerializable.h"

class GameObject;

enum ComponentType {TRANSFORM,IMAGE,CAMERA};

class Component : public ISerializable
{
public:
	GameObject* gameObject;  
	virtual std::string& serialize()=0;
	virtual Component* deserialize(std::stringstream ss) = 0;
	Component() {
		gameObject = nullptr;
	};
	~Component() {
		//TODO: 析构函数
	};
};