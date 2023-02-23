#include "renderwindow.h"

#include <QApplication>
#include "Core/SystemStatus/GameEngine.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    RenderWindow w;
    w.show();
    return a.exec();
}
