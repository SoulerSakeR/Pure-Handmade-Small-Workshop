#pragma once
#include "Component.h"

class Renderer : public Component
{
public:
	Renderer(GameObject* gameObject);
	~Renderer();
	// Inherited via Component
	void set_property(Property* property, void* value) override;
	void reset() override;

	// getters and setters
	int get_render_order();
	void set_render_order(int order);
protected:
	void set_render_order_directly(int order);
	// Inherited via Component
	void serialize(PHString&) override;
	void deserialize(std::stringstream& ss) override;

	int render_order;
};

