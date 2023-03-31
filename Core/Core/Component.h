#pragma once

#include <sstream>

#include "ISerializable.h"

class GameObject;

enum ComponentType {UNKNOWN,TRANSFORM,IMAGE,CAMERA};

class Component : public ISerializable
{
public:
	GameObject* gameObject;
	ComponentType componentType;
	typedef ComponentType customType;
	virtual void serialize(PHString&)=0;
	virtual void deserialize(std::stringstream& ss) = 0;
	virtual void reset() = 0;
	Component(GameObject* gameObj) {
		gameObject = gameObj;
		componentType = UNKNOWN;
	};
	~Component() {
		//TODO: 析构函数
	};
};