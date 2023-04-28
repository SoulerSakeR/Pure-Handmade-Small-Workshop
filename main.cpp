#include "Core/WaveFunctionCollapse/WaveFunctionCollapse.h"
#include <QLoggingCategory>
#include <QApplication>
#include <Windows.h>
#include <thread>
#include "Core/UI/mainwindow.h"
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
#include "Core/GameLogic/GameLoop.h"
#include "core/UI/GameWindow.h"

class GameLoop;

void showMainWindow()
{
    MainWindow* w = new MainWindow;
    GameEngine::get_instance().initialize(w);
    w->show();
}

void showGameWindow() {
    // Todo
    GameWindow* gameWindow = new GameWindow;
    GameEngine::get_instance().initializeGame(gameWindow);
}
int main(int argc, char *argv[])
{ 

    QLoggingCategory::setFilterRules(QStringLiteral("qt.gui.imageio=false")); // 关闭图片格式警告
    //QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);

    //// 加载样式表文件
    //QFile file("./style.qss");
    //if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //    QTextStream stream(&file);
    //    a.setStyleSheet(stream.readAll());
    //    file.close();
    //}
    //else {
    //    qDebug() << "Failed to load stylesheet file!";
    //}

    auto flag = a.testAttribute(Qt::AA_ShareOpenGLContexts);

    int ret = 0;
    std::ifstream file("config.txt");
    int value = 0;
    std::string path = "";
    if (file >> value >> path) {
        if (value == 0) {
            // 进入引擎界面
            QMetaObject::invokeMethod(qApp, &showMainWindow, Qt::QueuedConnection);
            std::cout << "Engine Start" << std::endl;
            // 等待程序结束并释放资源
            ret = a.exec();

            // 释放 MainWindow 对象的内存
            MainWindow* w = GameEngine::get_instance().getWindow();
            delete w;

        }
        else if (value == 1) {
            // 进入游戏界面
            std::cout << "Game Start" << std::endl;
            GameEngine::get_instance().initGamePathForExport(path);
            QMetaObject::invokeMethod(qApp, &showGameWindow, Qt::QueuedConnection);

            // 等待程序结束并释放资源
            ret = a.exec();

            GameWindow* g = GameEngine::get_instance().getGameWindow();
            delete g;
            
        }
        else {
            std::cerr << "Invalid value in config file." << std::endl;
        }
    }
    else {
        // 进入引擎界面
        QMetaObject::invokeMethod(qApp, &showMainWindow, Qt::QueuedConnection);
        std::cout << "Engine Start" << std::endl;
        // 等待程序结束并释放资源
        ret = a.exec();

        // 释放 MainWindow 对象的内存
        MainWindow* w = GameEngine::get_instance().getWindow();
        delete w;
    }

    

   
    return ret;
}
