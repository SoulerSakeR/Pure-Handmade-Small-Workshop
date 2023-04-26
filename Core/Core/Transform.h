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
    //constructor
    Transform(GameObject* gameObject);
    ~Transform();

    //method    
    void reset() override; //重置
    void set_property(Property* property, void* value) override; //设置属性

    Vector2D getWorldPosition(); //获取世界坐标位置 需要绑定
    float getWorldRotation(); //获取世界旋转角度 需要绑定
    Vector2D getWorldScale(); //需要绑定
    Transform* translate(Vector2D value); //朝给定向量移动 需要绑定
    Transform* translate1(float x, float y);//朝给定xx移动

    Vector2D get_localPosition(); //获取本地坐标位置 需要绑定
    void set_localPosition(Vector2D value); //设置本地坐标位置 需要绑定
    float get_localRotation(); //获取本地旋转角度 需要绑定
    void set_localRotation(float value); //设置本地旋转角度 需要绑定
    Vector2D get_localScale(); //获取本地缩放 需要绑定
    void set_localScale(Vector2D value); //设置本地缩放 需要绑定
 
    //field
    std::vector<Transform*> children; //子节点
    Transform* parent; //父节点

protected:
    void serialize(PHString&) override; //序列化
    void deserialize(std::stringstream& ss) override; //反序列化  

    Vector2D localPosition = Vector2D::zero(); //本地坐标位置
    float localRotation = 0.0f; //本地旋转角度
    Vector2D localScale = Vector2D::one();
};
