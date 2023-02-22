#include "GameObject.h"

string GameObject::serialize()
{
    // TODO: 序列化
    return string();
}

GameObject::GameObject(string name)
{
    this->name = name;
    isActive = false;
    // TODO: 随机生成uuid
    Components = vector<Component*>();
    
}
