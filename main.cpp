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

class GameLoop;

int main(int argc, char *argv[])
{ 
    QLoggingCategory::setFilterRules(QStringLiteral("qt.gui.imageio=false")); // 关闭图片格式警告
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
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

    MainWindow w;
    GameEngine::get_instance().initialize(&w);
    w.show();

    return a.exec();
}
