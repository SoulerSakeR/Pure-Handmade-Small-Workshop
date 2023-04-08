#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector2.h"

class GameObject {
public:
    Vector2 position;       // 物体位置
    Vector2 velocity;       // 物体速度
    Vector2 acceleration;   // 物体加速度
    float mass;             // 物体质量
    float width;            // 包围盒宽度
    float height;           // 包围盒高度

    GameObject() : position(Vector2()), velocity(Vector2()), acceleration(Vector2()), mass(1), width(0), height(0) {}
    GameObject(Vector2 _position, Vector2 _velocity, Vector2 _acceleration, float _mass, float _width, float _height)
        : position(_position), velocity(_velocity), acceleration(_acceleration), mass(_mass), width(_width), height(_height) {}

    // 获取物体的左上角位置
    Vector2 getTopLeft() const {
        return position - Vector2(width / 2, height / 2);
    }

    // 获取物体的右下角位置
    Vector2 getBottomRight() const {
        return position + Vector2(width / 2, height / 2);
    }
};

#endif
