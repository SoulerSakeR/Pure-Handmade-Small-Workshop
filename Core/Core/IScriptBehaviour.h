#pragma once
#include <vector>

struct CollisonInfo;

class IScriptBehaviour 
{
public:
	typedef IScriptBehaviour customType;
	// called when the object is created
	virtual void awake() {};

	// called when all objects are created (after awake)
	virtual void start() {};
	
	// called before beforeupdate
	virtual void onCollide(const std::vector<CollisonInfo>&) {};

	// called before update
	virtual void beforeUpdate() {};

	// called every frame
	virtual void update() {};

	// called after update
	virtual void afterUpdate() {};	
};

