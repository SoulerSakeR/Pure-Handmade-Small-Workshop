#include "Component.h"

void Component::set_enabled(bool value)
{
	enabled = value;
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
	default:
		return "Unknown";
		break;
	}
}
