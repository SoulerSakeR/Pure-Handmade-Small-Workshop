#pragma once

#include "ISerializable.h"
#include "Transform.h"

class GameObject :
    public ISerializable
{
public:
    //field
    bool isActive; //当前游戏对象的激活状态
    std::string name; //名字
    Transform* transform; //游戏对象的transform组件
    std::vector<Component*> components; //游戏对象的所有组件

    //constructor
    GameObject();
    GameObject(std::string name);
    // TODO: 析构函数

    //method
    int getID();
    void serialize(PHString& str) override;
    void deserialize(std::stringstream& ss) override;
    Component* addComponent(ComponentType type);
    void addComponent(Component* component);
    bool isRootGameObject();

private:  
    static int idCount;
    int id; //唯一id
};
