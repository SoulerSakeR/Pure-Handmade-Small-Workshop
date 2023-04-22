#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QMainWindow>
#include "Core/Render/common.h"
#include <QLineEdit>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resizeGL(QResizeEvent*);
    void refreshHierachy();
    void setupFileSystemTreeView(const QString& parentDir);
    void open();
    void addScene();
    void deleteScene();  
    void importScene();
    void showContextMenu(const QPoint&);
    void onTreeviewRightClick(const QPoint& pos);
    void onListItemDoubleClicked();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void initMenuSettings();
    void initMenuBar();
    Ui::MainWindow *ui;
    QFileSystemModel* fileModel;
};
#endif // RENDERWINDOW_H
