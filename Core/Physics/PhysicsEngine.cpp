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

        // 获取物体的Transform组件和RigidBody组件
        Transform* transform = getTransform(obj1);
        RigidBody* rigidBody = getRigidBody(obj1);

        // 更新物体的位置和速度
        if (rigidBody != nullptr) {
            // 计算物体所受的合力
            Vector2D force = rigidBody->get_acceleration() * rigidBody->get_mass();
            // 计算物体的新速度
            Vector2D newVelocity = rigidBody->get_velocity() + (force / rigidBody->get_mass()) * deltaTime;
            // 考虑摩擦力对速度的影响
            newVelocity = newVelocity * (1 - rigidBody->get_friction_ratio());
            // 更新物体的速度和位置
            rigidBody->set_velocity(newVelocity);
            transform->set_localPosition(transform->get_localPosition() + rigidBody->get_velocity() * deltaTime);
        }

        // 检测物体是否和其他物体碰撞
        for (int j = i + 1; j < objects.size(); j++) {
            GameObject* obj2 = objects[j];

            // 如果两个物体
            if (checkCollision(obj1, obj2)) {
                // TODO: 处理碰撞
            }
        }
    }
}

bool PhysicsEngine::checkCollision(GameObject* obj1, GameObject* obj2) {
    // 获取物体的包围盒
    BoxCollider* boxCollider1 = getBoxCollider(obj1);
    Vector2D obj1_size = boxCollider1->get_size();
    Vector2D obj1_localPos = getTransform(obj1)->get_localPosition();
    Vector2D obj1_topLeft = obj1_localPos + Vector2D(-obj1_size.x / 2, obj1_size.y / 2);
    Vector2D obj1_bottomRight = obj1_localPos + Vector2D(obj1_size.x / 2, -obj1_size.y / 2);
    BoxCollider* boxCollider2 = getBoxCollider(obj2);
    Vector2D obj2_size = boxCollider2->get_size();
    Vector2D obj2_localPos = getTransform(obj2)->get_localPosition();
    Vector2D obj2_topLeft = obj2_localPos + Vector2D(-obj2_size.x / 2, obj2_size.y / 2);
    Vector2D obj2_bottomRight = obj2_localPos + Vector2D(obj2_size.x / 2, -obj2_size.y / 2);

    // 检测碰撞
    if (obj1_bottomRight.x < obj2_topLeft.x || obj1_topLeft.x > obj2_bottomRight.x)
        return false;   // 未碰撞
    if (obj1_bottomRight.y < obj2_topLeft.y || obj1_topLeft.y > obj2_bottomRight.y)
        return false;   // 未碰撞

    // 计算碰撞力
    RigidBody* rigidBody1 = getRigidBody(obj1);
    RigidBody* rigidBody2 = getRigidBody(obj2);
    Vector2D force = (rigidBody1->get_velocity() * rigidBody1->get_mass() + rigidBody2->get_velocity() * rigidBody2->get_mass()) / 2;
    // 考虑摩擦力对碰撞力的影响
    force = force * (1 - (rigidBody1->get_friction_ratio() + rigidBody2->get_friction_ratio()) / 2);

    // 更新速度
    rigidBody1->set_velocity(rigidBody1->get_velocity() + force / rigidBody1->get_mass());
    rigidBody2->set_velocity(rigidBody2->get_velocity() + force / rigidBody2->get_mass());

    return true;        // 碰撞了
}