#pragma once

#include <sstream>

#include "ISerializable.h"
#include <vector>
#include "Property.h"
#include <unordered_map>
#include "Core/Utils/PHEvent.h"

class GameObject;

const std::string componentTypeNames[12] = { "Unknown","Transform","Image","Camera","Script","Rigid Body","Box Collider","Text","Renderer","Spine Animator","Audio Source","Light" };

class Component : public ISerializable
{
	friend class GameObject;
	friend class ComponentsDockWidget;
public:
	enum ComponentType { 
		UNKNOWN = 0, 
		TRANSFORM =1,
		IMAGE =2,
		CAMERA = 3,
		SCRIPT =4,
		RIGID_BODY = 5,
		BOX_COLLIDER = 6,
		TEXT = 7,
		RENDERER =8,
		SPINE_ANIMATOR = 9,
		AUDIO_SOURCE =10,
		LIGHT =11
	};
	
	typedef ComponentType customType;
	static const int componentTypeCount = 11;
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
	virtual void serialize(PHString& str) override;
	virtual void deserialize(std::stringstream& ss) override;
	virtual void serialize_1_0(PHString& str) override;
	virtual void deserialize_1_0(std::stringstream& ss) override;
};