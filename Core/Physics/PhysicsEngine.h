#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <vector>
#include "GameObject.h"

class PhysicsEngine {
public:
    std::vector<GameObject*> objects;   // 存储所有物体的指针

    void AddObject(GameObject* object);     // 添加物体
    void RemoveObject(GameObject* object);  // 移除物体

    void update(float deltaTime);   // 更新物理引擎

    bool checkCollision(GameObject* obj1, GameObject* obj2);    // 检测碰撞
};

#endif
