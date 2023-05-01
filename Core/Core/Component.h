#pragma once

#include <sstream>

#include "ISerializable.h"
#include <vector>
#include "Property.h"
#include <unordered_map>
#include "Core/Utils/PHEvent.h"

class GameObject;

class Component : public ISerializable
{
	friend class GameObject;
	friend class ComponentsDockWidget;
public:
	enum ComponentType { UNKNOWN, TRANSFORM, IMAGE, CAMERA, SCRIPT, RIGID_BODY,BOX_COLLIDER,TEXT,RENDERER,SPINE_ANIMATOR};
	typedef ComponentType customType;
	static const int componentTypeCount = 9;
	PHEvent<void,Property*> onPropertyChanged;
	Component(GameObject* gameObj);
	virtual ~Component();

	virtual void set_enabled(bool value);		
	virtual bool get_enabled() { return enabled;}
	virtual void reset() = 0;

	static std::string getTypeName(ComponentType type);
	
	GameObject* gameObject;
	ComponentType componentType;
	PropertiesQueue<std::string, Property*> properties;

protected:
	bool enabled;
	virtual void serialize(PHString& str);
	virtual void deserialize(std::stringstream& ss);
};