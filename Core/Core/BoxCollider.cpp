#include "BoxCollider.h"
#include "Core/SystemStatus/GameEngine.h"
#include "IResizable.h"
#include "Core/Physics/PhysicsEngine.h"

using namespace std;

BoxCollider::BoxCollider(GameObject* gameObj):IBoxResizable(gameObj)
{
	componentType = BOX_COLLIDER;
	properties.emplace("is trigger", new Property("is trigger", &is_trigger_, Property::VECTOR2D, this));
	PhysicsEngine::get_instance()->AddObject(gameObj);
}

BoxCollider::~BoxCollider()
{
	PhysicsEngine::get_instance()->RemoveObject(gameObject);
}


void BoxCollider::reset()
{
	set_size(Vector2D(100, 100));
}

void BoxCollider::set_property(Property* property, void* value)
{
	IBoxResizable::set_property(property, value);
	if (property->get_name() == "is trigger")
	{
		is_trigger_ = *(bool*)value;
	}
}


void BoxCollider::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	IBoxResizable::serialize(str);
	str.appendLine(to_string(is_trigger_));
}

void BoxCollider::deserialize(std::stringstream& ss)
{
	IBoxResizable::deserialize(ss);
	string s;
	getline(ss, s);
	is_trigger_ = stoi(s);
}
