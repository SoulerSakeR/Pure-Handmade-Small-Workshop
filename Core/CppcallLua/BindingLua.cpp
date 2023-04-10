#include "lib/sol/sol.hpp"
#include "Core/Input/InputDetection.h"

#include "Core/Core/Debug.h"
#include "Core/Core/GameObject.h"
#include "Core/Core/Transform.h"
#include "Core/Utils/Vector2D.h"

using namespace std;



void bindAllClasses(sol::state& lua) {
        
    lua.open_libraries(sol::lib::base, sol::lib::package);

    /*
    Input
    */

    //  InputDetection
    lua.new_usertype<InputDetection>("InputDetection",
        // 静态成员函数
        "getKey", &InputDetection::GetKey,
        "getKeyDown", &InputDetection::GetKeyDown,
        "getKeyUp", &InputDetection::GetKeyUp
        );


    /*
    Core
    */

    // Debug
    lua.new_usertype<Debug>("Debug",
        // 静态成员函数
        "log", &Debug::log,
        "logError", &Debug::logError
        );


    //  GameObject
    lua.new_usertype<GameObject>("GameObject",
        // 构造函数
        sol::constructors<GameObject(std::string, bool)>(),

        // 成员变量
        "isActive", &GameObject::isActive,
        "name", &GameObject::name,
        "transform", &GameObject::transform,
        "components", &GameObject::components,

        // 成员函数
        "getID", &GameObject::getID,
        "isRootGameObject", &GameObject::isRootGameObject
        );

    lua["GameObject"]["addComponent"] = [](GameObject& obj, ComponentType type) {
        return obj.addComponent(type);
    };

    lua["GameObject"]["getComponent"] = [](GameObject& obj, ComponentType type) {
        return obj.getComponent(type);
    };


    //  Transform
    lua.new_usertype<Transform>(
        "Transform",
        sol::constructors<Transform(GameObject*)>(),
        sol::meta_function::garbage_collect, sol::destructor([](Transform& t) { t.~Transform(); }),

        // 获取本地坐标位置
        "get_localPosition", &Transform::get_localPosition,

        // 获取本地旋转角度
        "get_localRotation", &Transform::get_localRotation,

        // 获取本地缩放
        "get_localScale", &Transform::get_localScale,

        // 设置本地坐标位置
        "set_localPosition", &Transform::set_localPosition,

        // 设置本地旋转角度
        "set_localRotation", &Transform::set_localRotation,

        // 设置本地缩放
        "set_localScale", &Transform::set_localScale,

        // 获取世界坐标位置
        "getWorldPosition", &Transform::getWorldPosition,

        // 获取世界旋转角度
        "getWorldRotation", &Transform::getWorldRotation,

        // 获取世界缩放
        "getWorldScale", &Transform::getWorldScale,

        // 朝给定向量移动
        "translate", &Transform::translate
        );
}











/*
    Input
*/

//  InputDetection
//void InputDetectionLua(sol::state lua) {
//    lua.open_libraries(sol::lib::base, sol::lib::package);
//
//    lua.new_usertype<InputDetection>("InputDetection",
//        // 静态成员函数
//        "getKey", &InputDetection::GetKey,
//        "getKeyDown", &InputDetection::GetKeyDown,
//        "getKeyUp", &InputDetection::GetKeyUp
//        );
//}




/*
    Core
*/

// Debug
//void DebugLua(sol::state lua) {
//    lua.open_libraries(sol::lib::base, sol::lib::package);
//
//    lua.new_usertype<Debug>("Debug",
//        // 静态成员函数
//        "log", &Debug::log,
//        "logError", &Debug::logError
//        );
//
//}

//  GameObject
//void GameObjectLua(sol::state lua) {
//    lua.open_libraries(sol::lib::base, sol::lib::package);
//
//    lua.new_usertype<GameObject>("GameObject",
//        // 构造函数
//        sol::constructors<GameObject(std::string, bool)>(),
//
//        // 成员变量
//        "isActive", &GameObject::isActive,
//        "name", &GameObject::name,
//        "transform", &GameObject::transform,
//        "components", &GameObject::components,
//
//        // 成员函数
//        "getID", &GameObject::getID,
//        "isRootGameObject", &GameObject::isRootGameObject
//        );
//
//    lua["GameObject"]["addComponent"] = [](GameObject& obj, ComponentType type) {
//        return obj.addComponent(type);
//    };
//
//    lua["GameObject"]["getComponent"] = [](GameObject& obj, ComponentType type) {
//        return obj.getComponent(type);
//    };
//
//}

//  Transform
//void TransformLua(sol::state lua) {
//    lua.open_libraries(sol::lib::base, sol::lib::package);
//
//    lua.new_usertype<Transform>("Transform",
//        // 构造函数
//        sol::constructors<Transform(GameObject*)>(),
//
//        // 成员变量
//        "localPosition", &Transform::localPosition,
//        "localRotation", &Transform::localRotation,
//        "localScale", &Transform::localScale,
//        "children", &Transform::children,
//        "parent", &Transform::parent,
//
//        // 成员函数
//        "getWorldPosition", &Transform::getWorldPosition,
//        "getWorldRotation", &Transform::getWorldRotation,
//        "getWorldScale", &Transform::getWorldScale,
//        "translate", &Transform::translate
//        );
//
//
//}