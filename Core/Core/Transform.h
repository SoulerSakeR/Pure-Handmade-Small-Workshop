#pragma once

#include <vector>
#include <sstream>

#include "Component.h"
#include "Vector2D.h"


class Transform : public Component
{
public:

    //field
    Vector2D localPosition = Vector2D::zero(); //本地坐标位置
    float localRotation = 0.0f; //本地旋转角度
    std::vector<Transform*> children ; //子节点
    Transform* parent; //父节点

    //constructor
    Transform();
    Transform(GameObject* gameObject);
    ~Transform();

    //method
    std::string& serialize(); //序列化
    Transform* deserialize(std::stringstream ss); //反序列化  
    Vector2D getWorldPosition(); //获取世界坐标位置
    float getWorldRotation(); //获取世界旋转角度
    Transform* translate(Vector2D value); //朝给定向量移动
};

