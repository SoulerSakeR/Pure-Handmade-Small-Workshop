#pragma once

#include <vector>

#include "Component.h"
#include "Vector2D.h"
#include "GameObject.h"


class Transform : public Component
{
public:
    Vector2D localPosition = Vector2D::zero(); //本地坐标位置
    float localRotation = 0.0f; //本地旋转角度
    vector<Transform*> children ; //子节点
    Transform* parent; //父节点
    string& serialize();
    //Transform();
    Transform(GameObject* gameObject);
    ~Transform();
    Vector2D getWorldPosition(); //获取世界坐标位置
    float getWorldRotation(); //获取世界旋转角度
    Transform* translate(Vector2D value); //朝给定向量移动
};

