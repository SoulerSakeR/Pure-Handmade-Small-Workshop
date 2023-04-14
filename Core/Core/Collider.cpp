#include "Collider.h"

Collider::Collider(GameObject* gameObj):IRenderable(gameObj)
{
	properties.emplace("is_trigger",new Property("is_trigger", &(this->is_trigger_), Property::BOOL, this));
}

bool Collider::is_trigger() const
{
    return is_trigger_;
}

void Collider::set_trigger(bool value)
{
    is_trigger_ = value;
}
