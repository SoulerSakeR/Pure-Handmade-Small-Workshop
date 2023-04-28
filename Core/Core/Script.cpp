#include "Script.h"

using namespace std;

Script::Script(GameObject* gameObj, const std::string& name, const std::string& path):Component(gameObj),name(name),path(path)
{
	componentType = SCRIPT;
	properties.emplace("name",new Property("name", &this->name,Property::STRING,this));
	properties.emplace("path",new Property("path", &this->path,Property::STRING,this));
	lua = nullptr;
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

void Script::awake()
{
	(*lua)["awake"] = nullptr;
	(*lua)["this"] = gameObject;
	(*lua).script_file(path.getNewPath());
	sol::protected_function f = (*lua)["awake"];
	if (!f.valid())
		return;
	auto result = f();
	if (!result.valid())
	{
		sol::error err = result;
		Debug::logError() << "lua runtime error: " << name << " at " << path.getNewPath() << "in awake function\n";
		Debug::logError() <<  err.what() << "\n";
	}
}

void Script::start()
{
	(*lua)["start"] = nullptr;
	(*lua)["this"] = gameObject;
	(*lua).script_file(path.getNewPath());
	sol::protected_function f = (*lua)["start"];
	if (!f.valid())
		return;
	auto result = f();
	if (!result.valid())
	{
		sol::error err = result;
		Debug::logError() << "lua runtime error: " << name << " at " << path.getNewPath() << "in start function\n";
		Debug::logError() << err.what() << "\n";
	}
}

void Script::onCollide(const std::vector<CollisonInfo>& collisionInfo)
{
	(*lua)["onCollide"] = nullptr;
	(*lua)["this"] = gameObject;
	(*lua).script_file(path.getNewPath());
	sol::protected_function f = (*lua)["onCollide"];
	if (!f.valid())
		return;
	auto result = f();
	if (!result.valid())
	{
		sol::error err = result;
		Debug::logError() << "lua runtime error: " << name << " at " << path.getNewPath() << "in onCollide function\n";
		Debug::logError() << err.what() << "\n";
	}
}

void Script::beforeUpdate()
{
	(*lua)["beforeUpdate"] = nullptr;
	(*lua)["this"] = gameObject;
	(*lua).script_file(path.getNewPath());
	sol::protected_function f = (*lua)["beforeUpdate"];
	if (!f.valid())
		return;
	auto result = f();
	if (!result.valid())
	{
		sol::error err = result;
		Debug::logError() << "lua runtime error: " << name << " at " << path.getNewPath() << "in beforeUpdate function\n";
		Debug::logError() << err.what() << "\n";
	}
}

void Script::update()
{
	(*lua)["update"] = nullptr;
	(*lua)["this"] = gameObject;
	(*lua).script_file(path.getNewPath());
	sol::protected_function f = (*lua)["update"];
	if (!f.valid())
		return;
	auto result = f();
	if (!result.valid())
	{
		sol::error err = result;
		Debug::logError() << "lua runtime error: " << name << " at " << path.getNewPath() << "in update function\n";
		Debug::logError()  << err.what() << "\n";
	}
}

void Script::afterUpdate()
{
	(*lua)["afterUpdate"] = nullptr;
	(*lua)["this"] = gameObject;
	(*lua).script_file(path.getNewPath());
	sol::protected_function f = (*lua)["afterUpdate"];
	if (!f.valid())
		return;
	auto result = f();
	if (!result.valid())
	{
		sol::error err = result;
		Debug::logError() << "lua runtime error: " << name << " at " << path.getNewPath() << "in afterUpdate function\n";
		Debug::logError() << err.what() << "\n";
	}
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


