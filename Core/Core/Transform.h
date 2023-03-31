#pragma once

#include <vector>
#include <sstream>

#include "Component.h"
#include "Core/Utils/Vector2D.h"

class GameObject;

/// @brief transform component that controls the position and rotation of game obejct
class Transform : public Component
{
public:

    //field
    Vector2D localPosition = Vector2D::zero(); //本地坐标位置
    float localRotation = 0.0f; //本地旋转角度
    Vector2D localScale = Vector2D::zero();
    std::vector<Transform*> children ; //子节点
    Transform* parent; //父节点

    //constructor
    Transform(GameObject* gameObject);
    ~Transform();

    //method
    void serialize(PHString&) override; //序列化
    void deserialize(std::stringstream& ss) override; //反序列化  
    void reset() override;
    Vector2D getWorldPosition(); //获取世界坐标位置
    float getWorldRotation(); //获取世界旋转角度
    Vector2D getWorldScale(); 
    Transform* translate(Vector2D value); //朝给定向量移动
};

