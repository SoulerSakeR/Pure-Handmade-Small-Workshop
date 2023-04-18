#include "Script.h"

using namespace std;

Script::Script(GameObject* gameObj, const std::string& name, const std::string& path):Component(gameObj),name(name),path(path)
{
	componentType = SCRIPT;
	properties.emplace("name",new Property("name", &this->name,Property::STRING,this));
	properties.emplace("path",new Property("path", &this->path,Property::STRING,this));
}

void Script::reset()
{
	name = "";
	path = PHPath("");
}

void Script::set_property(Property* property, void* value)
{
	Component::set_property(property, value);
	if (property->get_name() == "name")
	{
		name = (*(string*)value);
	}
	else if (property->get_name() == "path")
	{
		path = PHPath(*(string*)value);
	}
}

std::string Script::get_name()
{
	return name;
}

void Script::set_name(const std::string& name)
{
	this->name = name;
	onPropertyChange(properties["name"]);
}

std::string Script::get_path()
{
	return path.getNewPath();
}

void Script::set_path(const std::string& path)
{
	this->path = PHPath(path);
}

void Script::serialize(PHString& str)
{
	str.appendLine(to_string((int)componentType));
	str.appendLine(name);
	str.appendLine(path.getNewPath());
}

void Script::deserialize(std::stringstream& ss)
{
	string s;
	getline(ss, s);
	name = s;
	getline(ss, s);
	path = PHPath(s);
}


