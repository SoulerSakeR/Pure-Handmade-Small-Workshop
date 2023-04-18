#pragma once

#include <sstream>

#include "ISerializable.h"
#include <vector>
#include "Property.h"
#include <unordered_map>
#include "Core/Utils/Event.h"

class GameObject;

class Component : public ISerializable
{
	friend class GameObject;
	friend class ComponentsDockWidget;
public:
	enum ComponentType { UNKNOWN, TRANSFORM, IMAGE, CAMERA, SCRIPT, RIGID_BODY,BOX_COLLIDER,TEXT};
	typedef ComponentType customType;
	static const int componentTypeCount = 7;
	Event<void,Property*> onPropertyChange;
	Component(GameObject* gameObj) {
		gameObject = gameObj;
		enabled = true;
		componentType = UNKNOWN;
		properties.emplace("enabled", new Property("enabled", &enabled, Property::BOOL, this));
	};
	~Component() {
		gameObject = nullptr;
		for (auto& property : properties)
		{
			delete property.second;
		}
	};

	virtual void set_enabled(bool value);		
	virtual bool get_enabled() { return enabled; }
	virtual void reset() = 0;
	virtual void set_property(Property* property, void* value);
	static std::string getName(ComponentType type);
	
	GameObject* gameObject;
	ComponentType componentType;
	//std::unordered_map<std::string,Property*> properties;
	PropertiesQueue<std::string, Property*> properties;

protected:
	bool enabled;
	virtual void serialize(PHString&) = 0;
	virtual void deserialize(std::stringstream& ss) = 0;
};