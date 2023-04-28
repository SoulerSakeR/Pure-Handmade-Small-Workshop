#pragma once
#include "Component.h"
#include "lib/sol/sol.hpp"

class IScriptBehaviour 
{
public:
	typedef IScriptBehaviour customType;
	// called when the object is created
	virtual void awake() {};

	// called when all objects are created (after awake)
	virtual void start() {};
	
	// called before update
	virtual void beforeUpdate() {};

	// called every frame
	virtual void update() {};

	// called after update
	virtual void afterUpdate() {};
};

