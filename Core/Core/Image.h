#pragma once
#include "Component.h"
#include <Core/Core/Vector2D.h>

class Image :
    public Component
{
public:
    std::string imgPath;
    Vector2D size = Vector2D::zero();

    Image(const std::string& imgPath="");
    void serialize(PHString&) override; //序列化
    void deserialize(std::stringstream& ss) override; //反序列化  
};

