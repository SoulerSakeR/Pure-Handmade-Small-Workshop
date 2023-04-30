#include "Script.h"
#include "GameObject.h"
#include "Core/Core/Debug.h"

#include "lib/sol/sol.hpp"

using namespace std;

Script::Script(GameObject* gameObj, const std::string& name, const std::string& path):Component(gameObj),name(name),path(path)
{
	componentType = SCRIPT;
	properties.emplace("name",new Property("name", &this->name,Property::STRING,this));
	properties.emplace("path",new Property("path", &this->path,Property::STRING,this));
	lua = nullptr;
}

Script::~Script()
{
	if (lua != nullptr)
	{
		delete awake_func;
		delete start_func;
		delete onCollide_func;
		delete beforeUpdate_func;
		delete update_func;
		delete afterUpdate_func;
	}
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
	onPropertyChanged(properties["name"]);
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
	if (lua == nullptr)
	{
		Debug::logError() <<"GameObjetc : " << gameObject->get_name() << ", Script : " << name << " at path : "<< path.getNewPath()<<" lua state is not initialized !\n";
		return;
	}
	if (gameObject->isActive && get_enabled() && awake_func!=nullptr&&awake_func->valid())
	{
		auto result = (*awake_func)((*lua)[sol_instance]);
		if (!result.valid())
		{
			sol::error err = result;
			Debug::logError() << "GameObjetc : " << gameObject->get_name() << ", Script : " << name << " at path : " << path.getNewPath() << " lua runtime error : awake() \n";
			Debug::logError() << err.what() << "\n";
		}
	}
}

void Script::start()
{
	if (lua == nullptr)
	{
		Debug::logError() << "GameObjetc : " << gameObject->get_name() << ", Script : " << name << " at path : " << path.getNewPath() << " lua state is not initialized !\n";
		return;
	}
	if (gameObject->isActive && get_enabled() && start_func &&(*start_func).valid())
	{
		auto result = (*start_func)((*lua)[sol_instance]);
		if (!result.valid())
		{
			sol::error err = result;
			Debug::logError() << "GameObjetc : " << gameObject->get_name() << ", Script : " << name << " at path : " << path.getNewPath() << " lua runtime error : start() \n";
			Debug::logError() << err.what() << "\n";
		}
	}
}

void Script::onCollide(const std::vector<CollisonInfo>& collisionInfo)
{
	if (lua == nullptr)
	{
		Debug::logError() << "GameObjetc : " << gameObject->get_name() << ", Script : " << name << " at path : " << path.getNewPath() << " lua state is not initialized !\n";
		return;
	}
	if (gameObject->isActive && get_enabled() && onCollide_func &&(*onCollide_func).valid())
	{
		auto result = (*onCollide_func)((*lua)[sol_instance]);
		if (!result.valid())
		{
			sol::error err = result;
			Debug::logError() << "GameObjetc : " << gameObject->get_name() << ", Script : " << name << " at path : " << path.getNewPath() << " lua runtime error : onCollide() \n";
			Debug::logError() << err.what() << "\n";
		}
	}
}

void Script::beforeUpdate()
{
	if (lua == nullptr)
	{
		Debug::logError() << "GameObjetc : " << gameObject->get_name() << ", Script : " << name << " at path : " << path.getNewPath() << " lua state is not initialized !\n";
		return;
	}
	if (gameObject->isActive && get_enabled() && beforeUpdate_func&& (*beforeUpdate_func).valid())
	{
		auto result = (*beforeUpdate_func)((*lua)[sol_instance]);
		if (!result.valid())
		{
			sol::error err = result;
			Debug::logError() << "GameObjetc : " << gameObject->get_name() << ", Script : " << name << " at path : " << path.getNewPath() << " lua runtime error : beforeUpdate() \n";
			Debug::logError() << err.what() << "\n";
		}
	}
}

void Script::update()
{
	if (lua == nullptr)
	{
		Debug::logError() << "GameObjetc : " << gameObject->get_name() << ", Script : " << name << " at path : " << path.getNewPath() << " lua state is not initialized !\n";
		return;
	}
	if (gameObject->isActive && get_enabled() && update_func&& (*update_func).valid())
	{
		auto result = (*update_func)((*lua)[sol_instance]);
		if (!result.valid())
		{
			sol::error err = result;
			Debug::logError() << "GameObjetc : " << gameObject->get_name() << ", Script : " << name << " at path : " << path.getNewPath() << " lua runtime error : update() \n";
			Debug::logError() << err.what() << "\n";
		}
	}
}

void Script::afterUpdate()
{
	if (lua == nullptr)
	{
		Debug::logError() << "GameObjetc : " << gameObject->get_name() << ", Script : " << name << " at path : " << path.getNewPath() << " lua state is not initialized !\n";
		return;
	}
	if (gameObject->isActive && get_enabled() && afterUpdate_func&& (*afterUpdate_func).valid())
	{
		auto result = (*afterUpdate_func)((*lua)[sol_instance]);
		if (!result.valid())
		{
			sol::error err = result;
			Debug::logError() << "GameObjetc : " << gameObject->get_name() << ", Script : " << name << " at path : " << path.getNewPath() << " lua runtime error : afterUpdate() \n";
			Debug::logError() << err.what() << "\n";
		}
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


