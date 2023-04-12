#include "Property.h"
#include <string>

Property::Property(const std::string& name, void* data,PropertyType type, Component* compoennt) :name(name), data(data), type(type),component(compoennt)
{
}

std::string Property::get_name()
{
	return name;
}

Component* const Property::get_component()
{
	return component;
}

Property::~Property()
{
	//TO
}
