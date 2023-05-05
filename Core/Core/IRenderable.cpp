#include "IRenderable.h"
#include "Renderer.h"
#include "GameObject.h"

IRenderable::IRenderable(GameObject* gameobj) : Component(gameobj)
{
	if (gameobj == nullptr)
		return;
	vao = nullptr;
	vbo = nullptr;
	ibo = nullptr;
	vertices.clear();
	indices.clear();
	this->color = Color32{ 255,255,255,255 };
	auto color = new Property("Color", &this->color, Property::COLOR, this);
	color->set_property_func<Color32>(&IRenderable::get_color, &IRenderable::set_color, this);
	properties.emplace(color);	
	texture2D = nullptr;
}

IRenderable::~IRenderable()
{
	if (gameObject == nullptr)
		return;
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
	return nullptr;
}

Color32 IRenderable::get_color()
{
	return color;
}

void IRenderable::set_color(Color32 color)
{
	this->color = color;
	onPropertyChanged(properties["Color"]);
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
