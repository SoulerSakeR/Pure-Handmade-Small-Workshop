#pragma once
#include "Component.h"

class Collider
{
public:   
    bool is_trigger() const;
    void set_trigger(bool value);
protected:
    bool is_trigger_;
};

