#pragma once

#include "ISerializable.h"
#include "Transform.h"
#include "Core/Utils/Utils.h"
#include "Core/Core/Debug.h"

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
    ~GameObject();
    // TODO: 析构函数

    //method
    int getID();
    void serialize(PHString& str) override;
    void deserialize(std::stringstream& ss) override;
    Component* addComponent(ComponentType type);
    template <typename T>
    T* addComponent();
    template <typename T>
    T* getComponent();
    Component* getComponent(ComponentType type);
    void addComponent(Component* component);
    bool isRootGameObject();

private:  
    static int idCount;
    int id; //唯一id
};

template<typename T>
inline T* GameObject::addComponent()
{
    if (has_type_member<T>::value)
    {
        T* result = new T(this);
        if (result != nullptr)
        {
            if (typeid(T) == typeid(Transform))
                transform = (Transform*)result;
            result->gameObject = this;
            components.push_back(result);
        }
        return result;
    }
    return nullptr;
}

template<typename T>
inline T* GameObject::getComponent()
{
    if (has_type_member<T>::value)
    {
        for (auto component : components)
        {
            if (auto p = dynamic_cast<T*>(component); p != nullptr)
            {
                return (T*)component;
            }
                
        }
    }
    return nullptr;
}