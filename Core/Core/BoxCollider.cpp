#include "BoxCollider.h"
#include "Core/SystemStatus/GameEngine.h"
#include "IResizable.h"

using namespace std;

BoxCollider::BoxCollider(GameObject* gameObj):IBoxResizable(gameObj)
{
	componentType = BOX_COLLIDER;
	properties.emplace("is trigger", new Property("is trigger", &is_trigger_, Property::VECTOR2D, this));
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
	str.appendLine(size.tostring());
	str.appendLine(to_string(is_trigger_));
}

void BoxCollider::deserialize(std::stringstream& ss)
{
	string s;
	getline(ss, s);
	size = Vector2D::fromString(s);
	getline(ss, s);
	is_trigger_ = stoi(s);
}
