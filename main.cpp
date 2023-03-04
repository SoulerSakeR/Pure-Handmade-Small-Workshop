#include "renderwindow.h"

#include <QApplication>
#include "Core/SystemStatus/GameEngine.h"
#include "Core/Core/Test.h"

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
    return a.exec();
}
