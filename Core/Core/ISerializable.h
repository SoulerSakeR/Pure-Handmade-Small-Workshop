#pragma once
#include <string>

class ISerializable
{
public:
	virtual std::string& serialize() = 0;
};

