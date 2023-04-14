#pragma once
#include "Component.h"
#include "IRenderable.h"
class Collider : public IRenderable
{
public:   
    bool is_trigger() const;
    void set_trigger(bool value);
protected:
    Collider(GameObject* gameObj);
    bool is_trigger_;
};

