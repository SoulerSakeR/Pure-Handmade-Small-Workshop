#include "BoxCollider.h"
#include "Core/SystemStatus/GameEngine.h"

using namespace std;

BoxCollider::BoxCollider(GameObject* gameObj):Collider(gameObj)
{
	componentType = BOX_COLLIDER;
	reset();
	properties.emplace("Size", new Property("Size", &(this->size), Property::VECTOR2D, this));
}

const Vector2D& BoxCollider::get_size()
{
	return size;
}

void BoxCollider::set_size(Vector2D newSize, bool refreshUI)
{
	size = newSize;
	if (refreshUI && GameEngine::get_instance().getSelectedGameObject() == gameObject)
	{
		GameEngine::get_instance().onPropertyChange(properties["Size"]);
	}
}

void BoxCollider::reset()
{
	size = Vector2D::one();
}

void BoxCollider::set_property(Property* property, void* value)
{
	if (property->get_name() == "Size")
	{
		set_size(*(Vector2D*)(value));
	}
}

void BoxCollider::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	str.appendLine(size.tostring());
}

void BoxCollider::deserialize(std::stringstream& ss)
{
	string s;
	getline(ss, s);
	size = Vector2D::fromString(s);
}
