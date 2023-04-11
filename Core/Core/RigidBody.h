#pragma once
#include "Component.h"
#include <Core/Utils/Vector2D.h>

class RigidBody :
    public Component
{
public:
	RigidBody(GameObject* gameObj);
	~RigidBody();

	float get_mass() const;
	void set_mass(float value);
	Vector2D get_velocity() const;
	void set_velocity(Vector2D value);
	Vector2D get_acceleration() const;
	void set_acceleration(Vector2D value);
	float get_friction_ratio() const;
	void set_friction_ratio(float value);
	
	void reset() override;
	void set_property(Property * property, void* value) override;

private:
	void serialize(PHString& str) override;
	void deserialize(std::stringstream& ss) override;

	float mass;
	Vector2D velocity;
	Vector2D acceleration;	
	float friction_ratio;
};

