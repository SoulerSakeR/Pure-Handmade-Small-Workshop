#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QtWidgets/qmainwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class RenderWindow; }
QT_END_NAMESPACE

class RenderWindow : public QMainWindow
{
    Q_OBJECT

public:
    RenderWindow(QWidget *parent = nullptr);
    ~RenderWindow();

private slots:
    void on_Render_action_triggered();

    void on_Clear_action_triggered();



    void on_actionWirefame_triggered();

private:

    Ui::RenderWindow *ui;
};
#endif // RENDERWINDOW_H
