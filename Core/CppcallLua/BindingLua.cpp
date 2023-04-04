#include "sol/sol.hpp"
#include "Input/InputDetection.h"

#include "Core/Debug.h"
#include "Core/GameObject.h"
#include "Core/Transform.h"

using namespace std;


/*
    Input
*/

//  InputDetection
void InputDetectionLua(sol::state lua) {
    lua.open_libraries(sol::lib::base, sol::lib::package);

    lua.new_usertype<InputDetection>("InputDetection",
        // ��̬��Ա����
        "getKey", &InputDetection::GetKey,
        "getKeyDown", &InputDetection::GetKeyDown,
        "getKeyUp", &InputDetection::GetKeyUp
        );
}




/*
    Core
*/

// Debug
void DebugLua(sol::state lua) {
    lua.open_libraries(sol::lib::base, sol::lib::package);

    lua.new_usertype<Debug>("Debug",
        // ��̬��Ա����
        "log", &Debug::log,
        "logError", &Debug::logError
        );

}

//  GameObject
void GameObjectLua(sol::state lua) {
    lua.open_libraries(sol::lib::base, sol::lib::package);

    lua.new_usertype<GameObject>("GameObject",
        // ���캯��
        sol::constructors<GameObject(std::string, bool)>(),

        // ��Ա����
        "isActive", &GameObject::isActive,
        "name", &GameObject::name,
        "transform", &GameObject::transform,
        "components", &GameObject::components,

        // ��Ա����
        "getID", &GameObject::getID,
        "isRootGameObject", &GameObject::isRootGameObject
        );

    lua["GameObject"]["addComponent"] = [](GameObject& obj, ComponentType type) {
        return obj.addComponent(type);
    };

    lua["GameObject"]["getComponent"] = [](GameObject& obj, ComponentType type) {
        return obj.getComponent(type);
    };

}

//  Transform
void TransformLua(sol::state lua) {
    lua.open_libraries(sol::lib::base, sol::lib::package);

    lua.new_usertype<Transform>("Transform",
        // ���캯��
        sol::constructors<Transform(GameObject*)>(),

        // ��Ա����
        "localPosition", &Transform::localPosition,
        "localRotation", &Transform::localRotation,
        "localScale", &Transform::localScale,
        "children", &Transform::children,
        "parent", &Transform::parent,

        // ��Ա����
        "getWorldPosition", &Transform::getWorldPosition,
        "getWorldRotation", &Transform::getWorldRotation,
        "getWorldScale", &Transform::getWorldScale,
        "translate", &Transform::translate
        );


}