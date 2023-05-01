#pragma once
#include "Component.h"
#include "Core/Utils/Vector2D.h"
#include <qmatrix4x4.h>
#include "IResizable.h"

const float CAMERA_DEFAULT_VIEW_WIDTH = 1000.f;

class Camera : public IBoxResizable
{
public:
    static constexpr int MAX_COMPONENT_SIZE = 1;
   
    Camera(GameObject* gameObj, float view_width = CAMERA_DEFAULT_VIEW_WIDTH);
    ~Camera();
    bool is_main_camera();
    void set_main_camera(bool value);
    void set_view_width(float value);
    float get_view_width();
    bool is_overlay();
    void set_overlay(bool value);

    QMatrix4x4 CalculateProjectionMulViewMatrix();
    Vector2D screenToWorld(Vector2D screenPos);
    Vector2D WorldToScreen(Vector2D worldPos);
    void lookAt(Vector2D worldPos);

    void reset() override;
    
protected:
    void serialize(PHString&) override;
    void deserialize(std::stringstream& ss) override;
    
    bool is_main_camera_;
    float view_width;
    bool is_overlay_;
};

