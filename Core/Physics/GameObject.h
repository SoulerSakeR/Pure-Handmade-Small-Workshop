#pragma once

#include "Vector2.h"
#include "Core/Core/Transform.h"


class Transform;

class GameObject {
public:
    Vector2 velocity;       // 物体速度
    Vector2 acceleration;   // 物体加速度
    float mass;             // 物体质量
    float width;            // 包围盒宽度
    float height;           // 包围盒高度

    GameObject() : velocity(Vector2()), acceleration(Vector2()), mass(1), width(0), height(0), transform(nullptr) {}
    GameObject(Transform* _transform, Vector2 _velocity, Vector2 _acceleration, float _mass, float _width, float _height)
        : velocity(_velocity), acceleration(_acceleration), mass(_mass), width(_width), height(_height), transform(_transform) {}

    // 获取物体的左上角位置
    Vector2 getTopLeft() const {
        return transform->getWorldPosition() - Vector2(width / 2, height / 2);
    }

    // 获取物体的右下角位置
    Vector2 getBottomRight() const {
        return transform->getWorldPosition() + Vector2(width / 2, height / 2);
    }

private:
    Transform* transform;   // 指向 Transform 组件的指针
};
