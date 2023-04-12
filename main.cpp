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


int main(int argc, char *argv[])
{ 
    QLoggingCategory::setFilterRules(QStringLiteral("qt.gui.imageio=false")); // 关闭图片格式警告
    QApplication a(argc, argv);
    RenderWindow w;
    GameEngine::get_instance().initialize(&w);
    w.show();
    return a.exec();
}
