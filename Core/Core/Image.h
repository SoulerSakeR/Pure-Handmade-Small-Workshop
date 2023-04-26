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

    Image(GameObject* gameObj);

    // getters and setters
    bool set_texture2D(const std::string& name);

    // Inherited via Component
    void set_property(Property* property, void* value) override;
    void reset() override; 

protected:
    // Inherited via Component
    void serialize(PHString&) override; 
    void deserialize(std::stringstream& ss) override;     
    std::string texture2D;
};

