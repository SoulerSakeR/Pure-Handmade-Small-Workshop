#include "Property.h"
#include <string>

Property::Property(const std::string& name, void* data,PropertyType type, ISerializable* object,bool isVisible, bool isEditable) :name(name), data(data), type(type),object(object)
{
	is_visible = isVisible;
	is_editable = isEditable;
	get = nullptr;
	set = nullptr;
}

std::string Property::get_name()
{
	return name;
}

ISerializable* const Property::get_object()
{
	return object;
}

Property::~Property()
{

}
