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
	case SPINE_ANIMATOR:
		return "Spine Animator";
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
