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
    auto& gameObject1 = GameEngine::getInstance().addGameObject("testGameObejct1");
    gameObject1.addComponent<Image>()->imgPath ="adsadsad";    
    GameEngine::getInstance().addGameObject("testGameObejct2", &gameObject1).addComponent<Image>()->imgPath = "qwewqe";
    auto save = gp.saveTest();
    const std::string** scenes = new const std::string * [1];
    scenes[0] = &((save[1]).str());
    GameEngine::getInstance().openGameProjectTest(save->str(), scenes);
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
