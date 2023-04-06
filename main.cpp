#include <QLoggingCategory>
#include <QApplication>
#include <Windows.h>
#include <thread>
#include "Core/UI/renderwindow.h"
#include "lib/sol/sol.hpp"
#include "Core/Render/common.h"
#include "Core/SystemStatus/GameEngine.h"
#include "Core/Utils/Test.h"
#include "Core/Input/InputDetection.h"
#include "Core/Core/Debug.h"
#include "Core/Core/Image.h"
#include "Core/Core/Camera.h"
#include <Core/ResourceManagement/SceneMgr.h>
#include "Core/ThreadPool/ThreadPool.h"

void InputDetectionLua(sol::state& lua) {
    lua.open_libraries(sol::lib::base, sol::lib::package);

    lua.new_usertype<InputDetection>("InputDetection",
        // 静态成员函数
        "getKey", &InputDetection::GetKey,
        "getKeyDown", &InputDetection::GetKeyDown,
        "getKeyUp", &InputDetection::GetKeyUp
    );
}

void GameObjectLua(sol::state& lua) {
    lua.open_libraries(sol::lib::base);

    /*
    绑定一个c++类
    */

    /*
    lua.new_usertype<func_plus>("func_plus",
        //构造函数
        sol::constructors<func_plus(int, int)>(),
        //其他函数
        "plus", &func_plus::plus
        );

    //在lua中创造一个func_plus示例
    lua.script("obj=func_plus.new(1,2)");

    // 在 Lua 中调用成员函数
    lua.script("print(obj:plus())");

    */

    //绑定GameObject.hpp类
    lua.new_usertype<GameObject>("GameObject",
        //构造函数
        sol::constructors<GameObject(), GameObject(std::string name)>(),
        //公共参数
        "isActive", &GameObject::isActive,
        "name", &GameObject::name,
        "transform", &GameObject::transform,
        "components", &GameObject::components,
        //私有参数

        //普通函数
        /*"addComponent", sol::overload<Component* (GameObject::*)(ComponentType), void(GameObject::*)(Component*)>(
            &GameObject::addComponent, &GameObject::addComponent),*/
        "serialize", &GameObject::serialize,
        "deserialize", &GameObject::deserialize
    );
}
void task()
{
    sol::state sol_state;
    sol_state.open_libraries(sol::lib::base);
    InputDetectionLua(sol_state);
    GameObjectLua(sol_state);
    sol_state.script_file("test.lua");
    //std::cout << "Task " << i << " is running on thread " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char *argv[])
{ 
#ifdef TEST
    auto& gp = GameEngine::get_instance().creatGameProject("Test1", "E:\\SourceCodes\\Git\\GroupProject\\Pure-Handmade-Small-Workshop\\debug");
    auto& gameObj1 = GameEngine::get_instance().addGameObject("testGameObejct1");
    gameObj1.addComponent<Camera>();
    auto img1 = gameObj1.addComponent<Image>();
    img1->set_imgPath("\\resources\\boss_hornet.png");
    gameObj1.transform->localRotation += 45.f;
    auto& gameObj2 = GameEngine::get_instance().addGameObject("testGameObejct2");
    gameObj2.transform->localPosition = Vector2D(.0f, .0f);
    auto img2 = gameObj2.addComponent<Image>();
    img2->set_imgPath("\\resources\\container.png");
    auto save = gp.save();
    auto open = GameEngine::get_instance().openGameProject("E:\\SourceCodes\\Git\\GroupProject\\Pure-Handmade-Small-Workshop\\debug\\Test1\\Test1.gameProject");
    ThreadPool pool;

#endif // TEST

    
    QLoggingCategory::setFilterRules(QStringLiteral("qt.gui.imageio=false")); // 关闭图片格式警告

    QApplication a(argc, argv);
    RenderWindow w;
    w.show();
    return a.exec();
}
