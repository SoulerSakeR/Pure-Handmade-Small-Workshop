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
void task(int i)
{
    std::cout << "Task " << i << " is running on thread " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char *argv[])
{ 
#ifdef TEST
    auto& gp = GameEngine::getInstance().creatGameProject("Test1", "E:/SourceCodes/Git/GroupProject/Pure-Handmade-Small-Workshop/debug/test");
    auto& gameObj1 = GameEngine::getInstance().addGameObject("testGameObejct1");
    auto img1 = gameObj1.addComponent<Image>();
    gameObj1.transform->localPosition = Vector2D(-0.5f, .0f);
    img1->imgPath  ="/resources/boss_hornet.png";
    img1->size = Vector2D(0.2f, 0.2f);
    auto& gameObj2 = GameEngine::getInstance().addGameObject("testGameObejct2");
    gameObj2.transform->localPosition = Vector2D(0.5f, .0f);
    auto img2 = gameObj2.addComponent<Image>();
    img2 ->imgPath = "\\resources\\0027.png";
    img2->size = Vector2D(0.1f, 0.2f);
    auto save = gp.saveTest();
    const std::string** scenes = new const std::string * [1];
    scenes[0] = &((save[1]).str());
    //GameEngine::getInstance().openGameProjectTest(save->str(), scenes);
    sol::state sol_state;
    sol_state.open_libraries(sol::lib::base);
    InputDetectionLua(sol_state);
    GameObjectLua(sol_state);
    //sol_state.script_file("test.lua");
#endif // TEST

    
    QLoggingCategory::setFilterRules(QStringLiteral("qt.gui.imageio=false")); // 关闭图片格式警告

    QApplication a(argc, argv);
    RenderWindow w;
    w.show();
    /*while (true)
    {
        if (InputDetection::GetKeyDown('B'))
        {
            std::cout << "B down!" << std::endl;
        }
        if (InputDetection::GetKey('B'))
        {
            std::cout << "B hold!" << std::endl;
        }
        if (InputDetection::GetKeyUp('B'))
        {
            std::cout << "B up!" << std::endl;
        }
        if (InputDetection::GetKeyDown('A'))
        {
            std::cout << "A down!" << std::endl;
        }
        if (InputDetection::GetKey('A'))
        {
            std::cout << "A hold!" << std::endl;
        }
        if (InputDetection::GetKeyUp('A'))
        {
            std::cout << "A up!" << std::endl;
        }
    }*/

    //ThreadPool pool;

    //pool.test();
    //for (int i = 0; i < 10; i++)
    //{
    //    pool.enqueue(task, i);
    //}

    return a.exec();
}
