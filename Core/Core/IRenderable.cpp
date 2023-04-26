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
	texture2D = nullptr;
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

bool IRenderable::isTextureValid()
{
	if(texture2D!=nullptr&&texture2D->get_texture()!=nullptr)
		return true;
	return false;
}

QOpenGLTexture* IRenderable::get_texture()
{
	if(isTextureValid())
		return texture2D->get_texture();
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

void IRenderable::serialize(PHString& str)
{
	Component::serialize(str);
	str.appendLine(color.toString());
}

void IRenderable::deserialize(std::stringstream& ss)
{
	Component::deserialize(ss);
	std::string line;
	std::getline(ss, line);
	color = Color32(line);
}
