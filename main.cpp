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
    

    //std::chrono::time_point<std::chrono::system_clock> start, end;
    //start = std::chrono::system_clock::now();

    //WaveFunctionCollapse::read_config_file("samples.xml");

    //end = std::chrono::system_clock::now();
    //auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //std::cout << "All samples done in " << elapsed_ms << "ms.\n";

	//WaveFunctionCollapse::run_overlapping("samples/Flowers.png", "wfc_results/", 256, 32, 3, true, true, true, 2, 1);


    QLoggingCategory::setFilterRules(QStringLiteral("qt.gui.imageio=false")); // 关闭图片格式警告
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);
    auto flag = a.testAttribute(Qt::AA_ShareOpenGLContexts);

    MainWindow w;
    GameEngine::get_instance().initialize(&w);
    w.show();

    QObject::connect(&a, &QCoreApplication::aboutToQuit, []() {
        GameEngine::get_instance().gameLoop->shutdown();
        });

    return a.exec();
}
