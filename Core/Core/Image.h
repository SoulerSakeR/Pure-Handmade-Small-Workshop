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
    void set_property(Property * property, void* value) override;
    void set_render_order(int order);
    int get_render_order();
    std::string imgPath;
    Vector2D size;

private:   
    void serialize(PHString&) override; 
    void deserialize(std::stringstream& ss) override; 
    void reset() override;
    void set_render_order_directly(int order);
    Texture2D texture;
    int render_order;
};

