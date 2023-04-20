#include "Component.h"

using namespace std;

void Component::set_enabled(bool value)
{
	enabled = value;
	onPropertyChange(properties["enabled"]);
}

void Component::set_property(Property* property, void* value)
{
	if(property->get_name()=="enabled")
		set_enabled(*(bool*)value);
}

std::string Component::getName(ComponentType type)
{
	switch (type)
	{
	case UNKNOWN:
		return "Unknown";
		break;
	case TRANSFORM:
		return "Transform";
		break;
	case IMAGE:
		return "Image";
		break;
	case CAMERA:
		return "Camera";
		break;
	case SCRIPT:
		return "Script";
		break;
	case RIGID_BODY:
		return "Rigid Body";
	case BOX_COLLIDER:
		return "Box Collider";
	case TEXT:
			return "Text";
	case RENDERER:
		return "Renderer";
	default:
		return "Unknown";
		break;
	}
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
