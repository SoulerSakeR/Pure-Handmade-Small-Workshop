#pragma once
#include "Component.h"

class Renderer : public Component
{
	friend class RenderSetting;
public:
	Renderer(GameObject* gameObject);
	~Renderer();
	// Inherited via Component
	void set_property(Property* property, void* value) override;
	void reset() override;

	// getters and setters
	int get_render_order();
	//void set_render_order(int order);
	int get_render_layer();
	void set_render_layer(int layer);
protected:
	// Inherited via Component
	void serialize(PHString&) override;
	void deserialize(std::stringstream& ss) override;
	int render_layer;
};

