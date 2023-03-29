#include "Core/UI/renderwindow.h"
#include "Core/Render/common.h"

#include <QApplication>
#include "Core/SystemStatus/GameEngine.h"
#include "Core/Core/Test.h"
#include "Core/Input/InputDetection.h"
#include <Windows.h>

int main(int argc, char *argv[])
{ 
#ifdef TEST
    GameProject* gp = GameEngine::getInstance()->creatGameProject("Test", "E:/SourceCodes/Git/GroupProject/Pure-Handmade-Small-Workshop/debug/test");
    GameObject* gameObject1 = GameEngine::getInstance()->addGameObject("testGameObejct1");
    GameEngine::getInstance()->addGameObject("testGameObejct2", gameObject1);
    gp->save();
    GameEngine::getInstance()->openGameProject("");
#endif // TEST
    QApplication a(argc, argv);
    RenderWindow w;
    w.show();
   /* while (true)
    {
        if (InputDetection::GetMouseButtonDown(VK_LBUTTON))
        {
            std::cout << "Left mouse button down!" << std::endl;
        }
        if (InputDetection::GetMouseButton(VK_LBUTTON))
        {
            std::cout << "Left mouse button is hold!" << std::endl;
        }
        if (InputDetection::GetMouseButtonUp(VK_LBUTTON))
        {
            std::cout << "Left mouse button is up!" << std::endl;
        }

    }*/
    std::string path = get_Project_Path();

#ifndef NDEBUG 
    OutputDebugString(L"sadsad");
#endif
    return a.exec();
}
