#pragma once
#include <string>
#include "PHString.h"

class ISerializable
{
public:
	virtual void serialize(PHString& str) = 0;
	virtual void deserialize(std::stringstream& ss) = 0;
};

