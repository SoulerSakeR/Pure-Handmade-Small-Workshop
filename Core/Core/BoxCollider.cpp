#include "BoxCollider.h"
#include "Core/SystemStatus/GameEngine.h"
#include "IResizable.h"
#include "Core/Physics/PhysicsEngine.h"

using namespace std;

BoxCollider::BoxCollider(GameObject* gameObj):IBoxResizable(gameObj)
{
	componentType = BOX_COLLIDER;
	auto is_trigger = new Property("Is Trigger", &is_trigger_, Property::BOOL, this);
	is_trigger->set_property_func<bool>(&BoxCollider::is_trigger, &BoxCollider::set_trigger,this);
	properties.emplace(is_trigger);
	PhysicsEngine::get_instance()->AddObject(gameObj);
}

BoxCollider::~BoxCollider()
{
	PhysicsEngine::get_instance()->RemoveObject(gameObject);
}

void BoxCollider::set_trigger(bool value)
{
	is_trigger_ = value;
	onPropertyChanged(properties["Is Trigger"]);
}


void BoxCollider::reset()
{
	set_size(Vector2D(100, 100));
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
