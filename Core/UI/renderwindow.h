#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QMainWindow>
#include "Core/Render/common.h"
#include <QLineEdit>

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
    void refreshHierachy();
    void setupFileSystemTreeView(const QString& parentDir);
    void openScene();
    void addScene();
    void deleteScene();  

public slots:
    void showContextMenu(const QPoint&);
    void onTreeviewRightClick(const QPoint& pos);

private slots:
    friend class listbox;

private:
    Ui::RenderWindow *ui;
};
#endif // RENDERWINDOW_H
