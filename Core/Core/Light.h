#pragma once
#include "Component.h"
#include <Core/ResourceManagement/Texture2D.h>

class Light :  public Component
{
public:
	Light(GameObject* gameobj);

	// getters and setters

	Color32 get_light_color() const;
	void set_light_color(const Color32& color);

	float get_intensity() const;
	void set_intensity(float intensity);

	float get_radius() const;
	void set_radius(float radius);

	// inherited via Component
	virtual void reset() override;

private:
	Color32 light_color;
	float intensity;
	float radius;
};

