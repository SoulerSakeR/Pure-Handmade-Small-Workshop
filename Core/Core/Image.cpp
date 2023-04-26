#include "Image.h"
#include "GameObject.h"
#include "Core/FileIO/IO.h"
#include "Core/SystemStatus/GameEngine.h"
#include <Core/ResourceManagement/SceneMgr.h>
#include "Core/ResourceManagement/ResourceMgr.h"

using namespace std;

bool Image::set_texture2D(const std::string& name)
{
	if (name == "None")
	{
		IRenderable::texture2D = nullptr;
		this->texture2D = "None";
		return true;
	}
	IRenderable::texture2D = ResourceMgr::get_instance().loadFromName<Texture2D>(name);
	this->texture2D = IRenderable::texture2D->get_name();
	if (isTextureValid())
		set_size(Vector2D(get_texture()->width(), get_texture()->height()));
	return true;
}



Image::Image(GameObject* gameObj):IBoxResizable(gameObj)
{
	componentType = IMAGE;
	texture2D = "None";
	properties.emplace("texture2D", new Property("texture2D", &(this->texture2D), Property::TEXTURE2D, this));
}

void Image::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	IBoxResizable::serialize(str);
	str.appendLine(texture2D);
}


void Image::set_property(Property* property, void* value)
{
	IBoxResizable::set_property(property, value);
	if (property->get_name() == "texture2D")
	{
		set_texture2D(*(string*)value);
	}
}



void Image::deserialize(std::stringstream& ss)
{
	IBoxResizable::deserialize(ss);
	string s;
	getline(ss, s);
	set_texture2D(s);
}

void Image::reset()
{

}

