#include "PhysicsEngine.h"

void PhysicsEngine::AddObject(GameObject* object) {
    objects.push_back(object);
}

void PhysicsEngine::RemoveObject(GameObject* object) {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i] == object) {
            objects.erase(objects.begin() + i);
            break;
        }
    }
}

void PhysicsEngine::update(float deltaTime) {
    // 遍历所有物体
    for (int i = 0; i < objects.size(); i++) {
        GameObject* obj1 = objects[i];

        // 获取物体的Transform组件
        Transform* transform = obj1->transform;

        // 更新物体的位置和速度
        obj1->velocity += obj1->acceleration * deltaTime;
        transform->set_localPosition(transform->get_localPosition() + obj1->velocity * deltaTime);

        // 检测物体是否和其他物体碰撞
        for (int j = i + 1; j < objects.size(); j++) {
            GameObject* obj2 = objects[j];

            // 如果两个物体碰撞了
            if (checkCollision(obj1, obj2)) {
                // TODO: 处理碰撞
            }
        }
    }
}

bool PhysicsEngine::checkCollision(GameObject* obj1, GameObject* obj2) {
    // 获取物体的包围盒
    Vector2 obj1_topLeft = obj1->getTopLeft();
    Vector2 obj1_bottomRight = obj1->getBottomRight();
    Vector2 obj2_topLeft = obj2->getTopLeft();
    Vector2 obj2_bottomRight = obj2->getBottomRight();

    // 检测碰撞
    if (obj1_bottomRight.x < obj2_topLeft.x || obj1_topLeft.x > obj2_bottomRight.x)
        return false;   // 未碰撞
    if (obj1_bottomRight.y < obj2_topLeft.y || obj1_topLeft.y > obj2_bottomRight.y)
        return false;   // 未碰撞

    return true;        // 碰撞了
}
