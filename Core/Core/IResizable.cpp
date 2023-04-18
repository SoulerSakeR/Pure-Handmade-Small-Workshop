#include "IResizable.h"
#include "Core/ResourceManagement/SceneMgr.h"

using namespace std;

IBoxResizable::IBoxResizable(GameObject* gameobj):IRenderable(gameobj)
{	
	borderIbo = nullptr;
	properties.emplace("size", new Property("size", &(this->size), Property::VECTOR2D, this));
	set_size(Vector2D(100, 100));
	createIndices();
	createBorderIndices();
}

IBoxResizable::~IBoxResizable()
{
	borderIbo->destroy();
	delete borderIbo;
	borderIbo = nullptr;
}

void IBoxResizable::set_property(Property* property, void* value)
{
	IRenderable::set_property(property, value);
	if (property->get_name() == "size")
	{
		set_size(*(Vector2D*)value);
	}
}

void IBoxResizable::updateVertices()
{
	float half_width = size.x / 2;
	float half_height = size.y / 2;
	vertices =  {
		Vertex{{half_width, half_height, 0.0f},{color.r, color.g, color.b, color.a},{1.0f, 1.0f}},        // top right
		Vertex{{half_width, -half_height, 0.0f},{color.r, color.g, color.b, color.a},{1.0f, 0.0f}},	   // bottom right
		Vertex{{-half_width, -half_height, 0.0f},{color.r, color.g, color.b, color.a},{0.0f, 0.0f}}, 	   // bottom left
		Vertex{{-half_width, half_height, 0.0f},{color.r, color.g, color.b, color.a},{0.0f,1.0f}}         // top left
	};
	updateOperatorVertices();
}

void IBoxResizable::createIndices()
{
	indices = {
		0, 3, 2, // first triangle
		0, 2, 1  // second triangle
	};
}

Vector2D IBoxResizable::get_size()
{
	return size;
}

void IBoxResizable::set_size(Vector2D newSize)
{
	size = newSize;
	IRenderable::reset();
	updateVertices();
	onPropertyChange(properties["size"]);
}

void IBoxResizable::createBorderIndices()
{
	borderIndices = {
		0, 1, 1, 2, 2, 3, 3, 0
	};
}

void IBoxResizable::updateOperatorVertices()
{
	operatorVertices.clear();
	for (int i = 0;i < borderIndices.size();i = i + 2)
	{
		Vector2D position1(vertices[borderIndices[i]].position[0], vertices[borderIndices[i]].position[1]);
		Vector2D texcord1(vertices[borderIndices[i]].texCoord[0], vertices[borderIndices[i]].texCoord[1]);
		Vector2D position2(vertices[borderIndices[i + 1]].position[0], vertices[borderIndices[i + 1]].position[1]);
		Vector2D texcord2(vertices[borderIndices[i + 1]].texCoord[0], vertices[borderIndices[i + 1]].texCoord[1]);
		Vector2D center = (position1 + position2) / 2;
		Vector2D coord = (texcord1 + texcord2) / 2;
		Vertex vertex = { {center.x,center.y,0.0f},{color.r,color.g,color.b,color.a},{coord.x,coord.y}};
		operatorVertices.push_back(vertex);
	}
}

bool IBoxResizable::isHovered(Vector2D screenPos)
{
	if (auto cam = SceneMgr::get_instance().get_main_camera();cam != nullptr)
	{
		//TODO: implement this
	}
	return false;
}
