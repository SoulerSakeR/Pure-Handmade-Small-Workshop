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
#include "Core/Utils/PHPath.h"
#include "Core/Utils/Event.h"

void InputDetectionLua(sol::state& lua) {
    lua.open_libraries(sol::lib::base, sol::lib::package);

    lua.new_usertype<InputDetection>("InputDetection",
        // 静态成员函数
        "getKey", &InputDetection::GetKey,
        "getKeyDown", &InputDetection::GetKeyDown,
        "getKeyUp", &InputDetection::GetKeyUp
    );
}

void task()
{
    sol::state sol_state;
    sol_state.open_libraries(sol::lib::base);
    InputDetectionLua(sol_state);
    sol_state.script_file("test.lua");
    //std::cout << "Task " << i << " is running on thread " << std::this_thread::get_id() << std::endl;
}

int add(int a, int b)
{
    std::cout << "Task " << a << " is running on thread " << std::this_thread::get_id()<<std::endl;
    return a + b;
}


int main(int argc, char *argv[])
{ 
#ifdef TEST
    auto& gp = GameEngine::get_instance().creatGameProject("Test1", "D:\\Dates\\PHE");
    auto& gameObj1 = GameEngine::get_instance().addGameObject("testGameObejct1");
    gameObj1.addComponent<Camera>()->set_main_camera(true);
    auto img1 = gameObj1.addComponent<Image>();
    img1->set_imgPath("\\resources\\boss_hornet.png");
    gameObj1.transform->localRotation += 45.f;
    auto& gameObj2 = GameEngine::get_instance().addGameObject("testGameObejct2");
    gameObj2.transform->localPosition = Vector2D(.0f, .0f);
    auto img2 = gameObj2.addComponent<Image>();
    img2->set_imgPath("\\resources\\container.png");
    // auto save = gp.save();
    // auto open = GameEngine::get_instance().openGameProject("E:\\SourceCodes\\Git\\GroupProject\\Pure-Handmade-Small-Workshop\\debug\\Test1\\Test1.gameProject");
    ThreadPool pool;
    for (int i = 0; i < 10; i++) {
        auto result = pool.enqueue(add, i, 2);
        auto s = result.get();
        std::cout << "Result: " << s << std::endl;
    }   
#endif // TEST

    ThreadPool pool;
    for (int i = 0; i < 10; i++) {
        PHPath path = PHPath("C:/Program Files/My/App.exe");
        auto getOldPath = std::bind(&PHPath::getOldPath, &path);
        auto oldpath = pool.enqueue(getOldPath).get();// pool.enqueue(函数的地址，对象实例的地址，参数1，参数2 ...), 然后用.get()获取返回值
        std::cout <<i<< "-OldPath: " << oldpath << std::endl;
        std::cout <<i<< "-NewPath: " << path.getNewPath() << std::endl;
    }
    //PHPath apath = PHPath("C:/Program Files/My/App.exe");
    PHPath apath = PHPath("E:\\DATA\\GroupProject\\pro");
    std::cout << "OldPath: " << apath.getOldPath() << std::endl;
    std::cout << "NewPath: " << apath.getNewPath() << std::endl;
    if(apath.getIsFile())
        std::cout << "FileName: " << apath.getFileName() << std::endl;
    else
        std::cout << "DirectoryName: " << apath.getFileName() << std::endl;
    QLoggingCategory::setFilterRules(QStringLiteral("qt.gui.imageio=false")); // 关闭图片格式警告

    QApplication a(argc, argv);
    RenderWindow w;
    GameEngine::get_instance().initialize(&w);
    w.show();
    return a.exec();
}
