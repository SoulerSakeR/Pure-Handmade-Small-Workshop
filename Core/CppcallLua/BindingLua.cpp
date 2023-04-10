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


    //  Transform
    lua.new_usertype<Transform>(
        "Transform",
        sol::constructors<Transform(GameObject*)>(),
        sol::meta_function::garbage_collect, sol::destructor([](Transform& t) { t.~Transform(); }),

        // ��ȡ��������λ��
        "get_localPosition", &Transform::get_localPosition,

        // ��ȡ������ת�Ƕ�
        "get_localRotation", &Transform::get_localRotation,

        // ��ȡ��������
        "get_localScale", &Transform::get_localScale,

        // ���ñ�������λ��
        "set_localPosition", &Transform::set_localPosition,

        // ���ñ�����ת�Ƕ�
        "set_localRotation", &Transform::set_localRotation,

        // ���ñ�������
        "set_localScale", &Transform::set_localScale,

        // ��ȡ��������λ��
        "getWorldPosition", &Transform::getWorldPosition,

        // ��ȡ������ת�Ƕ�
        "getWorldRotation", &Transform::getWorldRotation,

        // ��ȡ��������
        "getWorldScale", &Transform::getWorldScale,

        // �����������ƶ�
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