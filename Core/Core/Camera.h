#pragma once
#include "Component.h"
#include "Core/Utils/Vector2D.h"
#include <qmatrix4x4.h>
class Camera :
    public Component
{
public:
    static constexpr int max_component_size=1;
    static constexpr float default_view_width =10.f;
   
    Camera(GameObject* gameObj, float view_width = default_view_width);

    bool is_main_camera();
    void set_main_camera(bool value);
    QMatrix4x4 CalculateProjectionMulViewMatrix();
    void set_property(Property* property, void* value) override;
    void set_view_width(float value);
    float get_view_width();
private:
    void serialize(PHString&) override;
    void deserialize(std::stringstream& ss) override;
    void reset() override;

    bool is_main_camera_;
    float view_width;
};

