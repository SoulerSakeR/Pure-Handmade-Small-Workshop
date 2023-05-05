#include "Component.h"

using namespace std;

void Component::set_enabled(bool value)
{
	enabled = value;
	onPropertyChanged(properties["Enabled"]);
}

Component::Component(GameObject* gameObj)
{
	gameObject = gameObj;
	enabled = true;
	componentType = UNKNOWN;
	auto enabled_property = new Property("Enabled", &this->enabled, Property::BOOL, this);
	enabled_property->set_property_func<bool>(&Component::get_enabled, &Component::set_enabled,this);
	properties.emplace(enabled_property);
}

Component::~Component()
{
	gameObject = nullptr;
	for (auto& property : properties)
	{
		delete property.second;
	}
}


std::string Component::getTypeName(ComponentType type)
{
	return componentTypeNames[type];
}

void Component::serialize(PHString& str)
{
	str.appendLine(to_string(enabled));
}

void Component::deserialize(std::stringstream& ss)
{
	string s;
	getline(ss, s);
	enabled = stoi(s);
}

void Component::serialize_1_0(PHString& str)
{
	str.appendLine("Component:", to_string(componentType));
	for (auto it = properties.vbegin();it < properties.vend(); it++)
	{
		str.appendLine("Property:", (*it)->get_name());
		(*it)->serialize_1_0(str);
	}
	str.appendLine("ComponentEnd");
}

void Component::deserialize_1_0(std::stringstream& ss)
{
	string line;
	do
	{
		getline(ss, line);
		auto index = line.find("Property:");
		if (index != string::npos)
		{
			auto property_name = line.substr(index + 9);
			if (properties.find(property_name))
			{
				properties[property_name]->deserialize(ss);
			}
		}
	} while (ss.good() && line != "ComponentEnd");

}
