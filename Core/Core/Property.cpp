#include "Property.h"
#include <string>
#include <Core/Utils/Vector2D.h>
#include <Core/ResourceManagement/Texture2D.h>

using namespace std;

Property::Property(const std::string& name, void* data,PropertyType type, ISerializable* object,bool isVisible, bool isEditable) :name(name), data(data), type(type),object(object)
{
	is_visible = isVisible;
	is_editable = isEditable;
	get = nullptr;
	set = nullptr;
	asset_type = PHAsset::UNKNOWN;
}

std::string Property::get_name()
{
	return name;
}

ISerializable* const Property::get_object()
{
	return object;
}

void Property::serialize(PHString& str)
{
	switch (type)
	{
	case Property::INT:
		str.appendLine(to_string(get_data<int>()));
		break;
	case Property::FLOAT:
		str.appendLine(to_string(get_data<float>()));
		break;
	case Property::STRING:
		str.appendLine(get_data<std::string>());
		break;
	case Property::BOOL:
		str.appendLine(to_string(get_data<bool>()));
		break;
	case Property::VECTOR2D:
		str.appendLine(get_data<Vector2D>().tostring());
		break;
	case Property::COLOR:
		str.appendLine(get_data<Color32>().toString());
		break;
	case Property::COMBO_BOX:
		str.appendLine(to_string(get_data<int>()));
		break;
	case Property::TEXTURE2D:
		str.appendLine(get_data<string>());
		break;
	case Property::ANIMATION_COMBOBOX:
		str.appendLine(to_string(get_data<int>()));
		break;
	case Property::ANIMATION:
		str.appendLine(get_data<string>());
		break;
	case Property::SKIN_COMBOBOX:
		str.appendLine(to_string(get_data<int>()));
		break;
	case Property::SCRIPT_LINEEDIT:
		str.appendLine(get_data<string>());
		break;
	case Property::ASSET:
		str.appendLine(get_data<string>());
		break;
	default:
		break;
	}
}

void Property::deserialize(std::stringstream& ss)
{
	string line;
	getline(ss, line);
	switch (type)
	{
	case Property::INT:
		set_data<int>(stoi(line));
		break;
	case Property::FLOAT:
		set_data<float>(stof(line));
		break;
	case Property::STRING:
		set_data<std::string>(line);
		break;
	case Property::BOOL:
		set_data<bool>(stoi(line));
		break;
	case Property::VECTOR2D:
		set_data<Vector2D>(Vector2D::fromString(line));
		break;
	case Property::COLOR:
		set_data<Color32>(Color32(line));
		break;
	case Property::COMBO_BOX:
		set_data<int>(stoi(line));
		break;
	case Property::TEXTURE2D:
		set_data<string>(line);
		break;
	case Property::ANIMATION_COMBOBOX:
		set_data<int>(stoi(line));
		break;
	case Property::ANIMATION:
		set_data<string>(line);
		break;
	case Property::SKIN_COMBOBOX:
		set_data<int>(stoi(line));
		break;
	case Property::SCRIPT_LINEEDIT:
		set_data<string>(line);
		break;
	case Property::ASSET:
		set_data<string>(line);
		break;
	default:
		break;
	}
}

void Property::serialize_1_0(PHString& str)
{
	serialize(str);
}

void Property::deserialize_1_0(std::stringstream& ss)
{
	deserialize(ss);
}

Property::~Property()
{

}
