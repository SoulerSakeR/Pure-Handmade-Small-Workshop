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
        Transform* transform = getTransform(obj1);
        RigidBody* rigidBody = getRigidBody(obj1);
        
        // 更新物体的位置和速度
        if (rigidBody != nullptr) {
            rigidBody->set_velocity(rigidBody->get_velocity() + rigidBody->get_acceleration() * deltaTime);
            transform->set_localPosition(transform->get_localPosition() + rigidBody->get_velocity() * deltaTime);
        }

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
    BoxCollider* boxCollider1 = getBoxCollider(obj1);
    Vector2D obj1_topLeft = getTransform(obj1)->get_localPosition() + boxCollider1->getBoundingBoxTopLeft();
    Vector2D obj1_bottomRight = getTransform(obj1)->get_localPosition() + boxCollider1->getBoundingBoxBottomRight();

    BoxCollider* boxCollider2 = getBoxCollider(obj2);
    Vector2D obj2_topLeft = getTransform(obj2)->get_localPosition() + boxCollider2->getBoundingBoxTopLeft();
    Vector2D obj2_bottomRight = getTransform(obj2)->get_localPosition() + boxCollider2->getBoundingBoxBottomRight();

    // 检测碰撞
    if (obj1_bottomRight.x < obj2_topLeft.x || obj1_topLeft.x > obj2_bottomRight.x)
        return false;   // 未碰撞
    if (obj1_bottomRight.y < obj2_topLeft.y || obj1_topLeft.y > obj2_bottomRight.y)
        return false;   // 未碰撞

    return true;        // 碰撞了
}
