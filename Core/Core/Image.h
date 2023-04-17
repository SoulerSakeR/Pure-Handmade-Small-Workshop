#pragma once
#include "Component.h"
#include "Core/Utils/Vector2D.h"
#include "Core/ResourceManagement/Texture2D.h"
#include "IRenderable.h"
#include "IResizable.h"

class Image : public IBoxResizable
{
public:
    static constexpr int max_component_size = 1;

    Image(GameObject* gameObj, const std::string& imgPath = "", Vector2D size = Vector2D::zero());

    // getters and setters
    const std::string& get_imgPath();
    void set_imgPath(const std::string& imgPath,bool refreshUI = true);
    int get_render_order();
    void set_render_order(int order,bool refreshUI = true);

    // Inherited via Component
    void set_property(Property* property, void* value) override;
    void reset() override; 

protected:
    ~Image();

    void set_render_order_directly(int order);

    // Inherited via Component
    void serialize(PHString&) override; 
    void deserialize(std::stringstream& ss) override; 
     
    std::string imgPath;
    int render_order;
};

