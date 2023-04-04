#pragma once
#include "Component.h"
#include <Core/Utils/Vector2D.h>

class Image :
    public Component
{
public:
    std::string imgPath;
    Vector2D size = Vector2D::zero();

    Image(GameObject* gameObj,const std::string& imgPath ="",Vector2D size = Vector2D::zero()); 
    void serialize(PHString&) override; 
    void deserialize(std::stringstream& ss) override; 
    void reset() override;
};

