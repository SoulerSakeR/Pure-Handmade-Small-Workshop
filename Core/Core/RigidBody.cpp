#include "RigidBody.h"

using namespace std;

RigidBody::RigidBody(GameObject* gameObj):Component(gameObj)
{	
	componentType = RIGID_BODY;	
	reset();
	properties.emplace("velocity",new Property("velocity", &(this->velocity), Property::VECTOR2D, this));
	properties.emplace("acceleration",new Property("acceleration", &(this->acceleration), Property::VECTOR2D, this));
	properties.emplace("mass",new Property("mass", &(this->mass), Property::FLOAT, this));
}

RigidBody::~RigidBody()
{
}

void RigidBody::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	str.appendLine(to_string( mass));
	str.appendLine(velocity.tostring());
	str.appendLine(acceleration.tostring());
}

void RigidBody::deserialize(std::stringstream& ss)
{
	string s;
	getline(ss, s);
	mass = stof(s);
	getline(ss, s);
	velocity = Vector2D::fromString(s);
	getline(ss, s);
	acceleration = Vector2D::fromString(s);
}

void RigidBody::reset()
{
	mass = 1;
	velocity = Vector2D(0, 0);
	acceleration = Vector2D(0, 0);
}

void RigidBody::set_property(Property* property, void* value)
{
	if (property->get_name() == "velocity")
	{
		velocity = *(Vector2D*)value;
	}
	else if (property->get_name() == "acceleration")
	{
		acceleration = *(Vector2D*)value;
	}
	else if (property->get_name() == "mass")
	{
		mass = *(float*)value;
	}
}
