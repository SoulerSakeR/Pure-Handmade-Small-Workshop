#pragma once
#include "ISerializable.h"
class Component : public ISerializable
{
public:
	string serialize();
	Component() {};
	~Component() {};
};

