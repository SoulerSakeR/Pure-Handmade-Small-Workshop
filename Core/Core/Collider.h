#pragma once
#include "Component.h"
class Collider :
    public Component
{
public:   
    bool is_trigger() const;
    void set_trigger(bool value);
protected:
    Collider(GameObject* gameObj);
    bool is_trigger_;
};

