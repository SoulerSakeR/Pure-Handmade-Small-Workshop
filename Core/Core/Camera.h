#pragma once
#include "Component.h"
#include "Core/Utils/Vector2D.h"
#include <qmatrix4x4.h>
class Camera :
    public Component
{
public:
    static constexpr int max_component_size=1;
    static constexpr float default_view_width =1000.f;
   
    Camera(GameObject* gameObj, float view_width = default_view_width);

    bool is_main_camera();
    void set_main_camera(bool value,bool refreshUI = true);
    void set_view_width(float value, bool refreshUI = true);
    float get_view_width();

    QMatrix4x4 CalculateProjectionMulViewMatrix();
    Vector2D screenToWorld(Vector2D screenPos);
    Vector2D WorldToScreen(Vector2D worldPos);

    void set_property(Property* property, void* value) override;
    void reset() override;
    
protected:
    void serialize(PHString&) override;
    void deserialize(std::stringstream& ss) override;
    
    bool is_main_camera_;
    float view_width;
};

