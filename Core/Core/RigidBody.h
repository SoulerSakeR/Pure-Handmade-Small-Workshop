#pragma once
#include "Component.h"
#include <Core/Utils/Vector2D.h>

class RigidBody :
    public Component
{
public:
	RigidBody(GameObject* gameObj);
	~RigidBody();
	void serialize(PHString& str) override;
	void deserialize(std::stringstream& ss) override;
	void reset() override;
	void set_property(Property * property, void* value) override;
private:
	float mass;
	Vector2D velocity;
	Vector2D acceleration;	
};

