#include "Collider.h"

bool Collider::is_trigger() const
{
    return is_trigger_;
}

void Collider::set_trigger(bool value)
{
    is_trigger_ = value;
}
