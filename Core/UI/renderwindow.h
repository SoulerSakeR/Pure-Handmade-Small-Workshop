#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QMainWindow>
#include "Core/Render/common.h"
#include <QLineEdit>
#include <QFileSystemModel>

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
    void open();
    void addScene();
    void deleteScene();  
    void importScene();

public slots:
    void showContextMenu(const QPoint&);
    void onTreeviewRightClick(const QPoint& pos);

private slots:
    friend class listbox;

private:
    Ui::RenderWindow *ui;
    QFileSystemModel* fileModel;
};
#endif // RENDERWINDOW_H
