#pragma once


#include "ISerializable.h"
#include "Transform.h"

using namespace std;
class GameObject :
    public ISerializable
{
public:
    bool isActive; //当前游戏对象的激活状态
    string name; //名字
    int id; //唯一id
    Transform* transform; //游戏对象的transform组件
    vector<Component*> components; //游戏对象的所有组件
    string& serialize();
    GameObject(string name);
    Component* addComponent(ComponentType type);
    void addComponent(Component* component);
};
