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
        "logInfo", &Debug::logInfo,
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
        "tag", sol::property(&GameObject::get_tag, &GameObject::set_tag),
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
        "destroy", &GameObject::destroy,
        "clone", & GameObject::clone,
        "onComponentPropertyChangedHandler", & GameObject::onComponentPropertyChangedHandler,
        "find", & GameObject::find,
        "findTag", & GameObject::findTag);




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
        "getWorldPositionX", & Transform::getWorldPositionX,
        "getWorldPositionY", & Transform::getWorldPositionY,
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

    /*
        Utils
    */

    //Vector2D

    lua.new_usertype<Vector2D>("Vector2D",
        // 构造函数
        sol::constructors<Vector2D(float, float)>(),

        // 属性
        "x", & Vector2D::x,
        "y", & Vector2D::y,

        // 方法
        "tostring", & Vector2D::tostring,
        "tostringInt", & Vector2D::tostringInt,
        "toQString", & Vector2D::toQString,
        "toQStringInt", & Vector2D::toQStringInt,
        "toQVector3D", & Vector2D::toQVector3D,
        "toQVector2D", & Vector2D::toQVector2D,

        // 静态方法
        "zero", & Vector2D::zero,
        "one", & Vector2D::one,
        "fromString", & Vector2D::fromString,
        "fromQString", & Vector2D::fromQString,

        // 运算符
        sol::meta_function::addition, & Vector2D::operator+,
        sol::meta_function::subtraction, & Vector2D::operator-,
        sol::meta_function::multiplication, & Vector2D::operator*,
        sol::meta_function::division, & Vector2D::operator/,
        sol::meta_function::equal_to, & Vector2D::operator==,
        sol::meta_function::not_equal_to, & Vector2D::operator!=
        );
}







