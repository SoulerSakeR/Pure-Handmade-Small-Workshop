#include "Core/UI/renderwindow.h"
#include "Core/Render/common.h"

#include <QApplication>
#include <Windows.h>
#include "Core/SystemStatus/GameEngine.h"
#include "Core/Utils/Test.h"
#include "Core/Input/InputDetection.h"
#include "Core/Core/Debug.h"
#include "Core/Core/Image.h"

int main(int argc, char *argv[])
{ 
#ifdef TEST
    auto& gp = GameEngine::getInstance().creatGameProject("Test", "E:/SourceCodes/Git/GroupProject/Pure-Handmade-Small-Workshop/debug/test");
    auto& gameObj1 = GameEngine::getInstance().addGameObject("testGameObejct1");
    auto img1 = gameObj1.addComponent<Image>();
    gameObj1.transform->localPosition = Vector2D(-0.5f, .0f);
    img1->imgPath  ="\\resources\\boss_hornet.png";
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
#endif // TEST

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



    return a.exec();
}
