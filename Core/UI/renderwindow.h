#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QMainWindow>
#include "Core/Render/common.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RenderWindow; }
QT_END_NAMESPACE

class RenderWindow : public QMainWindow
{
    Q_OBJECT

public:
    RenderWindow(QWidget *parent = nullptr);
    ~RenderWindow();
    void resizeGL(QResizeEvent*);
public slots:
    void showContextMenu(const QPoint&);

private slots:
    friend class listbox;
    void on_Render_action_triggered();

    void on_Clear_action_triggered();


    void createItemsList();

    void on_actionWirefame_triggered();

private:

    Ui::RenderWindow *ui;
};
#endif // RENDERWINDOW_H
