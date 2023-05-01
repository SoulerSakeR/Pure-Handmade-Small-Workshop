#include "Image.h"
#include "GameObject.h"
#include "Core/FileIO/IO.h"
#include "Core/SystemStatus/GameEngine.h"
#include <Core/ResourceManagement/SceneMgr.h>
#include "Core/ResourceManagement/ResourceMgr.h"

using namespace std;

std::string Image::get_texture_name() const
{
	return texture_name;
}

bool Image::set_texture_name(const std::string& name)
{
	if (name == "None")
	{
		IRenderable::texture2D = nullptr;
		this->texture_name = "None";
		onPropertyChanged(properties["Texture Name"]);
		return true;
	}
	IRenderable::texture2D = ResourceMgr::get_instance().loadFromName<Texture2D>(name);
	if(IRenderable::texture2D != nullptr)
		this->texture_name = IRenderable::texture2D->get_name();
	else
		this->texture_name = name;
	if (isTextureValid())
		set_size(Vector2D(get_texture()->width(), get_texture()->height()));
	onPropertyChanged(properties["Texture Name"]);
	return true;
}



Image::Image(GameObject* gameObj):IBoxResizable(gameObj)
{
	componentType = IMAGE;
	texture_name = "None";
	auto texture_name_property = new Property("Texture Name", &(this->texture_name), Property::TEXTURE2D, this);
	texture_name_property->set_property_func<string>(&Image::get_texture_name, &Image::set_texture_name, this);
	properties.emplace(texture_name_property);
}

void Image::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	IBoxResizable::serialize(str);
	str.appendLine(texture_name);
}





void Image::deserialize(std::stringstream& ss)
{
	IBoxResizable::deserialize(ss);
	string s;
	getline(ss, s);
	set_texture_name(s);
}

void Image::reset()
{
	set_texture_name("None");
}

