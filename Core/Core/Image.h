#pragma once
#include "Component.h"
#include "Core/Utils/Vector2D.h"
#include "Core/ResourceManagement/Texture2D.h"
#include "IRenderable.h"
#include "IResizable.h"

class Image : public IBoxResizable
{
public:
    static constexpr int MAX_COMPONENT_SIZE = -1;

    Image(GameObject* gameObj);

    // getters and setters
    std::string get_texture_name() const;
    bool set_texture_name(const std::string& name);

    // Inherited via Component
    void reset() override; 

protected:
    // Inherited via Component
    void serialize(PHString&) override; 
    void deserialize(std::stringstream& ss) override;     
    std::string texture_name;
};

