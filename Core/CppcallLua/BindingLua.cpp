#include "lib/sol/sol.hpp"
#include "Core/Input/InputDetection.h"

#include "Core/Core/Debug.h"
#include "Core/Core/GameObject.h"
#include "Core/Core/Transform.h"
#include "Core/Core/Text.h"
#include "Core/Core/Image.h"
#include "Core/Core/Camera.h"
#include "Core/Core/Script.h"
#include "Core/Core/BoxCollider.h"
#include "Core/Core/Renderer.h"
#include "Core/Core/RigidBody.h"

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
    /*
    
    */
    lua.new_usertype<Debug>("Debug",
        // 静态成员函数
        "log", &Debug::logInfo,
        "logWarning", &Debug::logWarning,
        "logError", &Debug::logError
        );


    //  GameObject
    // 绑定 ComponentType 枚举
    lua.new_enum("ComponentType",
        "UNKNOWN", Component::ComponentType::UNKNOWN,
        "TRANSFORM", Component::ComponentType::TRANSFORM,
        "IMAGE", Component::ComponentType::IMAGE,
        "CAMERA", Component::ComponentType::CAMERA,
        "SCRIPT", Component::ComponentType::SCRIPT,
        "RIGID_BODY", Component::ComponentType::RIGID_BODY,
        "BOX_COLLIDER", Component::ComponentType::BOX_COLLIDER,
        "TEXT", Component::ComponentType::TEXT,
        "RENDERER", Component::ComponentType::RENDERER);

    // 绑定 GameObject 类
    lua.new_usertype<GameObject>("GameObject",
        // 构造函数
        sol::constructors<GameObject(std::string, bool)>(),

        // 属性
        "isActive", &GameObject::isActive,
        "name", sol::property(&GameObject::get_name, &GameObject::set_name),
        "transform", &GameObject::transform,

        // 方法
        "getID", &GameObject::getID,
        "addComponent", sol::overload(
            [](GameObject& obj, Component::ComponentType type) { return obj.addComponent(type); },
            [](GameObject& obj) { return obj.template addComponent<Transform>(); },
            [](GameObject& obj) { return obj.template addComponent<Image>(); },
            [](GameObject& obj) { return obj.template addComponent<Camera>(); },
            [](GameObject& obj) { return obj.template addComponent<Script>(); },
            [](GameObject& obj) { return obj.template addComponent<RigidBody>(); },
            [](GameObject& obj) { return obj.template addComponent<BoxCollider>(); },
            [](GameObject& obj) { return obj.template addComponent<Text>(); },
            [](GameObject& obj) { return obj.template addComponent<Renderer>(); }
        ),
        "getComponent", sol::overload(
            [](GameObject& obj, Component::ComponentType type) { return obj.getComponent(type); },
            [](GameObject& obj) { return obj.template getComponent<Transform>(); },
            [](GameObject& obj) { return obj.template getComponent<Image>(); },
            [](GameObject& obj) { return obj.template getComponent<Camera>(); },
            [](GameObject& obj) { return obj.template getComponent<Script>(); },
            [](GameObject& obj) { return obj.template getComponent<RigidBody>(); },
            [](GameObject& obj) { return obj.template getComponent<BoxCollider>(); },
            [](GameObject& obj) { return obj.template getComponent<Text>(); },
            [](GameObject& obj) { return obj.template getComponent<Renderer>(); }
        ),
        "getComponents", sol::overload(
            [](GameObject& obj) { return obj.template getComponents<Transform>(); },
            [](GameObject& obj) { return obj.template getComponents<Image>(); },
            [](GameObject& obj) { return obj.template getComponents<Camera>(); },
            [](GameObject& obj) { return obj.template getComponents<Script>(); },
            [](GameObject& obj) { return obj.template getComponents<RigidBody>(); },
            [](GameObject& obj) { return obj.template getComponents<BoxCollider>(); },
            [](GameObject& obj) { return obj.template getComponents<Text>(); },
            [](GameObject& obj) { return obj.template getComponents<Renderer>(); }
        ),
        "removeComponent", &GameObject::removeComponent,
        "isRootGameObject", &GameObject::isRootGameObject,
        "destroy", &GameObject::destroy);


    //  Transform
    lua.new_usertype<Transform>("Transform",
        // 构造函数
        sol::constructors<Transform(GameObject*)>(),

        // 属性
        "localPosition", sol::property(&Transform::get_localPosition, &Transform::set_localPosition),
        "localRotation", sol::property(&Transform::get_localRotation, &Transform::set_localRotation),
        "localScale", sol::property(&Transform::get_localScale, &Transform::set_localScale),
        "parent", &Transform::parent,
        "children", &Transform::children,

        // 方法
        "reset", &Transform::reset,
        "set_property", &Transform::set_property,
        "getWorldPosition", &Transform::getWorldPosition,
        "getWorldRotation", &Transform::getWorldRotation,
        "getWorldScale", &Transform::getWorldScale,
        "translate", &Transform::translate,
        "translate1", &Transform::translate1);


    //Text
    lua.new_usertype<Text>("Text",
        // 构造函数
        sol::constructors<Text(GameObject*, const std::string&)>(),

        // 属性
        "text", sol::property(&Text::get_text, &Text::set_text),

        // 方法
        "reset", & Text::reset,
        "set_property", & Text::set_property);


    //RigidBody
    lua.new_usertype<RigidBody>("RigidBody",
        sol::constructors<RigidBody(GameObject*)>(),
        "mass", sol::property(&RigidBody::get_mass, &RigidBody::set_mass),
        "velocity", sol::property(&RigidBody::get_velocity, &RigidBody::set_velocity),
        "acceleration", sol::property(&RigidBody::get_acceleration, &RigidBody::set_acceleration),
        "friction_ratio", sol::property(&RigidBody::get_friction_ratio, &RigidBody::set_friction_ratio)
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