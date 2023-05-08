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

    bool get_keep_aspect_ratio() const;
    void set_keep_aspect_ratio(bool keep);

    // Inherited via Component
    void reset() override; 

    // inherited via IBoxResizable
    virtual void set_size(Vector2D newSize) override;

protected:
    // Inherited via Component
    void serialize(PHString&) override; 
    void deserialize(std::stringstream& ss) override;     
    std::string texture_name;
    bool keep_aspect_ratio = true;
};

