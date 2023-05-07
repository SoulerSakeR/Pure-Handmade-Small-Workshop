#include "Light.h"
#include "Core/ResourceManagement/SceneMgr.h"

Light::Light(GameObject* gameobj): Component(gameobj)
{
	componentType = LIGHT;
	light_color = { 255,255,255,255 };
	auto light_color_property = new Property("Light Color", &this->light_color, Property::COLOR, this);
	light_color_property->set_property_func<Color32>(&Light::get_light_color, &Light::set_light_color, this);
	properties.emplace(light_color_property);
	intensity = 1.f;
	auto intensity_property = new Property("Intensity", &this->intensity, Property::FLOAT, this);
	intensity_property->set_property_func<float>(&Light::get_intensity, &Light::set_intensity, this);
	properties.emplace(intensity_property);
	radius = 1000.f;
	auto radius_property = new Property("Radius", &this->radius, Property::FLOAT, this);
	radius_property->set_property_func<float>(&Light::get_radius, &Light::set_radius, this);
	properties.emplace(radius_property);

	if (SceneMgr::get_instance().hasCurrentScene())
	{
		SceneMgr::get_instance().lights.push_back(this);
	}

}

Light::~Light()
{
	if (SceneMgr::get_instance().hasCurrentScene())
	{
		auto& lights = SceneMgr::get_instance().lights;
		if(auto it = std::find(lights.begin(), lights.end(), this); it != lights.end())
			lights.erase(it);
	}
}

Color32 Light::get_light_color() const
{
	return light_color;
}

void Light::set_light_color(const Color32& color)
{
	this->light_color = color;
	onPropertyChanged(properties["Light Color"]);
}

float Light::get_intensity() const
{
	return intensity;
}

void Light::set_intensity(float intensity)
{
	this->intensity = std::clamp(intensity,0.f,1.f);
	onPropertyChanged(properties["Intensity"]);
}

float Light::get_radius() const
{
	return radius;
}

void Light::set_radius(float radius)
{
	this->radius = radius;
	onPropertyChanged(properties["Radius"]);
}

void Light::reset()
{
	light_color = { 255,255,255,255 };
	intensity = 1.f;
	radius = 1000.f;
}
