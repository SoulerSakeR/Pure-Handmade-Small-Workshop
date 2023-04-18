#pragma once

#include "ISerializable.h"
#include "Transform.h"
#include "Core/Utils/Utils.h"
#include "Core/Utils/Result.h"
#include "Core/Core/Debug.h"

class GameObject :
    public ISerializable
{
    friend class Scene;
public:
    //field
    bool isActive; //当前游戏对象的激活状态 需要绑定
    std::string name; //名字 需要绑定
    Transform* transform; //游戏对象的transform组件 需要绑定
    std::vector<Component*> components; //游戏对象的所有组件 需要绑定

    //constructor
    GameObject(std::string name="",bool withTransform=true);   

    //method
    int getID(); //需要绑定
    Result<void*> set_name(const std::string& name); //需要绑定

    Component* addComponent(Component::ComponentType type); 
    template <typename T>
    T* addComponent(); //需要绑定
    template <typename T>
    T* getComponent(); //需要绑定
    template <typename T>
    std::vector<T*> getComponents(); //需要绑定
    Component* getComponent(Component::ComponentType type);
    void removeComponent(Component* component); //需要绑定
    bool isRootGameObject(); //需要绑定
    void destroy(); //需要绑定

private:  
    ~GameObject();
    void serialize(PHString& str) override;
    void deserialize(std::stringstream& ss) override;   
    static int idCount; //id计数器
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
        if(getComponent<T>() != nullptr)
			return nullptr;
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

template<typename T>
inline std::vector<T*> GameObject::getComponents()
{
    if (has_type_member<T>::value)
    {
		std::vector<T*> result;
        for (auto component : components)
        {
            if (auto p = dynamic_cast<T*>(component); p != nullptr)
            {
				result.push_back(p);
			}
		}
		return result;
	}
    return std::vector<T*>();
}
