#include "BoxCollider.h"
#include "Core/SystemStatus/GameEngine.h"
#include "IResizable.h"

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
	vertices = {
		Vertex{{half_width, half_height, 0.0f},{color.r, color.g, color.b, color.a},{1.0f, 1.0f}},        // top right
		Vertex{{half_width, -half_height, 0.0f},{color.r, color.g, color.b, color.a},{1.0f, 0.0f}},	   // bottom right
		Vertex{{-half_width, -half_height, 0.0f},{color.r, color.g, color.b, color.a},{0.0f, 0.0f}}, 	   // bottom left
		Vertex{{-half_width, half_height, 0.0f},{color.r, color.g, color.b, color.a},{0.0f,1.0f}}         // top left
	};
}

void BoxCollider::createIndices()
{
	indices = {
		0, 3, 2, // first triangle
		0, 2, 1  // second triangle
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
