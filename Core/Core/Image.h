#pragma once
#include "Component.h"
#include "Core/Utils/Vector2D.h"
#include "Core/FileIO/Texture2D.h"

class Image :
    public Component
{
public:
    static constexpr int max_component_size = 1;

    Image(GameObject* gameObj, const std::string& imgPath = "", Vector2D size = Vector2D::zero());

    void set_imgPath(const std::string& imgPath);
    void set_size(Vector2D newSize);
    void set_size(float  width, float height);

    std::string imgPath;
    Vector2D size;
private:   
    void serialize(PHString&) override; 
    void deserialize(std::stringstream& ss) override; 
    void reset() override;

    Texture2D texture;
};

