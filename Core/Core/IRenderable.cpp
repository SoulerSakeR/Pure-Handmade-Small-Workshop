#include "IRenderable.h"
#include "Renderer.h"
#include "GameObject.h"

IRenderable::IRenderable(GameObject* gameobj) : Component(gameobj)
{
	vao = nullptr;
	vbo = nullptr;
	ibo = nullptr;
	vertices.clear();
	indices.clear();
	properties.emplace("color", new Property("color", &color, Property::COLOR, this));
	color = Color32{ 255,255,255,255 };
	texture = nullptr;
	if (auto renderer = gameobj->getComponent<Renderer>(); renderer == nullptr)
	{
		gameobj->addComponent<Renderer>();
	}
}

IRenderable::~IRenderable()
{
	if (vao != nullptr)
	{
		vao->destroy();
		delete vao;
		vao = nullptr;
	}
	if (vbo != nullptr)
	{
		vbo->destroy();
		delete vbo;
		vbo = nullptr;
	}
	if (ibo != nullptr)
	{
		ibo->destroy();
		delete ibo;
		ibo = nullptr;
	}
	if (vertices.size()>0)
	{
		vertices.clear();
	}
}

void IRenderable::set_property(Property* property, void* value)
{
	Component::set_property(property, value);
	if (property->get_name() == "color")
	{
		color = *(Color32*)value;
	}
}

void IRenderable::reset()
{
	if (vao != nullptr)
	{
		vao->destroy();
		delete vao;
		vao = nullptr;
	}
	if (vbo != nullptr)
	{
		vbo->destroy();
		delete vbo;
		vbo = nullptr;
	}
	if (ibo != nullptr)
	{
		ibo->destroy();
		delete ibo;
		ibo = nullptr;
	}
	if (vertices.size()>0)
	{
		vertices.clear();
	}
}
