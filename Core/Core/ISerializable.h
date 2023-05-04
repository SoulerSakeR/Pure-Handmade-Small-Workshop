#pragma once
#include <string>
#include "PHString.h"

class ISerializable
{
protected:
	virtual void serialize(PHString& str) = 0;
	virtual void deserialize(std::stringstream& ss) = 0;
	virtual void serialize_1_0(PHString& str) = 0;
	virtual void deserialize_1_0(std::stringstream& ss) = 0;
};

