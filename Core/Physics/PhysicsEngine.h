﻿#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "Core/Core/GameObject.h"
#include "Core/Utils/Macro.h"

class RigidBody;
class BoxCollider;

struct CollisonInfo
{
    GameObject* firstObj;
    GameObject* secondObj;

};

class PhysicsEngine {
    SINGLETON(PhysicsEngine)
public:

    std::vector<GameObject*> objects;   // 存储所有物体的指针

    void AddObject(GameObject* object);     // 添加物体
    void RemoveObject(GameObject* object);  // 移除物体

    void update(float deltaTime);   // 更新物理引擎
    void clear() { objects.clear();collisonInfo.clear(); }   // 清空物理引擎
    bool checkCollision(GameObject* obj1, GameObject* obj2);    // 检测碰撞

    // 添加获取组件的代码
    Transform* getTransform(GameObject* object) { return object->getComponent<Transform>(); }
    RigidBody* getRigidBody(GameObject* object) { return object->getComponent<RigidBody>(); }
    BoxCollider* getBoxCollider(GameObject* object) { return object->getComponent<BoxCollider>(); }

    std::vector<CollisonInfo> collisonInfo;
};

#endif
