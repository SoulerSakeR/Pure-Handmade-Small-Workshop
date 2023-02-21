#include "../UI/renderwindow.h"

#include <QtWidgets/qapplication.h>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RenderWindow w;
    w.show();
    return a.exec();
}
