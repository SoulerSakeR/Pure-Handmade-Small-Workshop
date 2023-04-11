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

    const std::string& get_imgPath();
    void set_imgPath(const std::string& imgPath,bool refreshUI = true);
    const Vector2D& get_size();
    void set_size(Vector2D newSize,bool refreshUI = true);
    void set_size(float  width, float height, bool refreshUI = true);    
    int get_render_order();
    void set_render_order(int order,bool refreshUI = true);

    void set_property(Property* property, void* value) override;
    void reset() override; 

protected:
    ~Image();
    void serialize(PHString&) override; 
    void deserialize(std::stringstream& ss) override; 
    
    void set_render_order_directly(int order);
   
    std::string imgPath;
    Vector2D size;
    int render_order;   
    Texture2D texture;
};

