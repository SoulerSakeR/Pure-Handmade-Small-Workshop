#include "lib/sol/sol.hpp"
#include "Core/Input/InputDetection.h"

#include "Core/Core/Debug.h"
#include "Core/Core/GameObject.h"
#include "Core/Core/Transform.h"
#include "Core/Core/Text.h"
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
        // ��̬��Ա����
        "getKey", &InputDetection::GetKey,
        "getKeyDown", &InputDetection::GetKeyDown,
        "getKeyUp", &InputDetection::GetKeyUp
        );


    /*
    Core
    */

    // Debug
    lua.new_usertype<Debug>("Debug",
        // ��̬��Ա����
        "log", &Debug::log,
        "logError", &Debug::logError
        );


    //  GameObject
    // �� ComponentType ö��
    lua.new_enum("ComponentType",
        "UNKNOWN", ComponentType::UNKNOWN,
        "TRANSFORM", ComponentType::TRANSFORM,
        "IMAGE", ComponentType::IMAGE,
        "CAMERA", ComponentType::CAMERA,
        "SCRIPT", ComponentType::SCRIPT,
        "RIGID_BODY", ComponentType::RIGID_BODY,
        "BOX_COLLIDER", ComponentType::BOX_COLLIDER,
        "TEXT", ComponentType::TEXT,
        "RENDERER", ComponentType::RENDERER);

    // �� GameObject ��
    lua.new_usertype<GameObject>("GameObject",
        // ���캯��
        sol::constructors<GameObject(std::string, bool)>(),

        // ����
        "isActive", &GameObject::isActive,
        "name", sol::property(&GameObject::name, &GameObject::set_name),
        "transform", &GameObject::transform,

        // ����
        "getID", &GameObject::getID,
        "addComponent", sol::overload(
            [](GameObject& obj, ComponentType type) { return obj.addComponent(type); },
            [](GameObject& obj) { return obj.template addComponent<Transform>(); },
            [](GameObject& obj) { return obj.template addComponent<Image>(); },
            [](GameObject& obj) { return obj.template addComponent<Camera>(); },
            [](GameObject& obj) { return obj.template addComponent<Script>(); },
            [](GameObject& obj) { return obj.template addComponent<Rigidbody>(); },
            [](GameObject& obj) { return obj.template addComponent<BoxCollider>(); },
            [](GameObject& obj) { return obj.template addComponent<Text>(); },
            [](GameObject& obj) { return obj.template addComponent<Renderer>(); }
        ),
        "getComponent", sol::overload(
            [](GameObject& obj, ComponentType type) { return obj.getComponent(type); },
            [](GameObject& obj) { return obj.template getComponent<Transform>(); },
            [](GameObject& obj) { return obj.template getComponent<Image>(); },
            [](GameObject& obj) { return obj.template getComponent<Camera>(); },
            [](GameObject& obj) { return obj.template getComponent<Script>(); },
            [](GameObject& obj) { return obj.template getComponent<Rigidbody>(); },
            [](GameObject& obj) { return obj.template getComponent<BoxCollider>(); },
            [](GameObject& obj) { return obj.template getComponent<Text>(); },
            [](GameObject& obj) { return obj.template getComponent<Renderer>(); }
        ),
        "getComponents", sol::overload(
            [](GameObject& obj) { return obj.template getComponents<Transform>(); },
            [](GameObject& obj) { return obj.template getComponents<Image>(); },
            [](GameObject& obj) { return obj.template getComponents<Camera>(); },
            [](GameObject& obj) { return obj.template getComponents<Script>(); },
            [](GameObject& obj) { return obj.template getComponents<Rigidbody>(); },
            [](GameObject& obj) { return obj.template getComponents<BoxCollider>(); },
            [](GameObject& obj) { return obj.template getComponents<Text>(); },
            [](GameObject& obj) { return obj.template getComponents<Renderer>(); }
        ),
        "removeComponent", &GameObject::removeComponent,
        "isRootGameObject", &GameObject::isRootGameObject,
        "destroy", &GameObject::destroy);


    //  Transform
    lua.new_usertype<Transform>("Transform",
        // ���캯��
        sol::constructors<Transform(GameObject*)>(),

        // ����
        "localPosition", sol::property(&Transform::get_localPosition, &Transform::set_localPosition),
        "localRotation", sol::property(&Transform::get_localRotation, &Transform::set_localRotation),
        "localScale", sol::property(&Transform::get_localScale, &Transform::set_localScale),
        "parent", &Transform::parent,
        "children", &Transform::children,

        // ����
        "reset", &Transform::reset,
        "set_property", &Transform::set_property,
        "getWorldPosition", &Transform::getWorldPosition,
        "getWorldRotation", &Transform::getWorldRotation,
        "getWorldScale", &Transform::getWorldScale,
        "translate", &Transform::translate);


    //Text
    lua.new_usertype<Text>("Text",
        // ���캯��
        sol::constructors<Text(GameObject*, const std::string&)>(),

        // ����
        "text", sol::property(&Text::get_text, &Text::set_text),

        // ����
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
//        // ��̬��Ա����
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
//        // ��̬��Ա����
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
//        // ���캯��
//        sol::constructors<GameObject(std::string, bool)>(),
//
//        // ��Ա����
//        "isActive", &GameObject::isActive,
//        "name", &GameObject::name,
//        "transform", &GameObject::transform,
//        "components", &GameObject::components,
//
//        // ��Ա����
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
//        // ���캯��
//        sol::constructors<Transform(GameObject*)>(),
//
//        // ��Ա����
//        "localPosition", &Transform::localPosition,
//        "localRotation", &Transform::localRotation,
//        "localScale", &Transform::localScale,
//        "children", &Transform::children,
//        "parent", &Transform::parent,
//
//        // ��Ա����
//        "getWorldPosition", &Transform::getWorldPosition,
//        "getWorldRotation", &Transform::getWorldRotation,
//        "getWorldScale", &Transform::getWorldScale,
//        "translate", &Transform::translate
//        );
//
//
//}