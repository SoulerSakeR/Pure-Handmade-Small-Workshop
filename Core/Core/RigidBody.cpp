#include "RigidBody.h"

using namespace std;

RigidBody::RigidBody(GameObject* gameObj):Component(gameObj)
{	
	componentType = RIGID_BODY;	
	reset();
	auto velocity = new Property("Velocity", &this->velocity, Property::VECTOR2D, this);
	velocity->set_property_func<Vector2D>(&RigidBody::get_velocity, &RigidBody::set_velocity, this);
	properties.emplace(velocity);
	auto acceleration = new Property("Acceleration", &this->acceleration, Property::VECTOR2D, this);
	acceleration->set_property_func<Vector2D>(&RigidBody::get_acceleration, &RigidBody::set_acceleration, this);
	properties.emplace(acceleration);
	auto mass = new Property("Mass", &this->mass, Property::FLOAT, this);
	mass->set_property_func<float>(&RigidBody::get_mass, &RigidBody::set_mass, this);
	properties.emplace(mass);
}

RigidBody::~RigidBody()
{
}

float RigidBody::get_mass() const
{
	return mass;
}

void RigidBody::set_mass(float value)
{
	mass = value;
	onPropertyChanged(properties["Mass"]);
}

Vector2D RigidBody::get_velocity() const
{
	return velocity;
}

void RigidBody::set_velocity(Vector2D value)
{
	velocity = value;
	onPropertyChanged(properties["Velocity"]);
}

Vector2D RigidBody::get_acceleration() const
{
	return acceleration;
}

void RigidBody::set_acceleration(Vector2D value)
{
	acceleration = value;
	onPropertyChanged(properties["Acceleration"]);
}

float RigidBody::get_friction_ratio() const
{
	return friction_ratio;
}

void RigidBody::set_friction_ratio(float value)
{
	friction_ratio = value;
}

void RigidBody::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	Component::serialize(str);
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

