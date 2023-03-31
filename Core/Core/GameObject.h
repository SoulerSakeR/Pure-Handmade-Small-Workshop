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
    GameObject(std::string name=""); 
    ~GameObject();

    //method
    int getID(); //需要绑定
    void serialize(PHString& str) override;
    void deserialize(std::stringstream& ss) override;
    Component* addComponent(ComponentType type);
    template <typename T>
    T* addComponent(); //需要绑定
    template <typename T>
    T* getComponent(); //需要绑定
    Component* getComponent(ComponentType type);
    bool isRootGameObject(); //需要绑定

private:  
    static int idCount;
    int id; //唯一id
};

/// <summary>
/// 需要绑定，添加指定类型的组件
/// </summary>
/// <typeparam name="T">Component 子类</typeparam>
/// <returns>添加的组件指针，类型错误则为 nullptr</returns>
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

/// <summary>
/// 需要绑定，获取指定类型的组件
/// </summary>
/// <typeparam name="T">Component 子类</typeparam>
/// <returns>获取的组件指针，未找到则为 nullptr</returns>
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