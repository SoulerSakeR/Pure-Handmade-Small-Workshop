#include "BoxCollider.h"
#include "Core/SystemStatus/GameEngine.h"

using namespace std;

BoxCollider::BoxCollider(GameObject* gameObj):Collider(gameObj)
{
	componentType = BOX_COLLIDER;
	reset();
	createIndices();
	updateVertices();
	properties.emplace("Size", new Property("Size", &(this->size), Property::VECTOR2D, this));
}

const Vector2D& BoxCollider::get_size()
{
	return size;
}

void BoxCollider::set_size(Vector2D newSize, bool refreshUI)
{
	size = newSize;
	IRenderable::reset();
	updateVertices();
	if (refreshUI && GameEngine::get_instance().getSelectedGameObject() == gameObject)
	{
		GameEngine::get_instance().onPropertyChange(properties["Size"]);
	}
}

void BoxCollider::reset()
{
	size = Vector2D(50.f, 50.f);
}

void BoxCollider::set_property(Property* property, void* value)
{
	if (property->get_name() == "Size")
	{
		set_size(*(Vector2D*)(value));
	}
}

void BoxCollider::updateVertices()
{
	float half_width = size.x / 2;
	float half_height = size.y / 2;
	vertices = new float[12]
	{
		-half_width, -half_height, 0.0f,
		half_width, -half_height, 0.0f,
		half_width, half_height, 0.0f,
		-half_width, half_height, 0.0f
	};
}

void BoxCollider::createIndices()
{
	indices = new unsigned int[8] {
		0, 1, 1, 2, 2, 3, 3, 0
	};
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
