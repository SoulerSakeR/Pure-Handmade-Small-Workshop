#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QMainWindow>
#include "Core/Render/common.h"
#include <QLineEdit>
#include <QFileSystemModel>
#include <QPlainTextEdit>

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
    void setupFileSystemTreeView(const QString& parentDir);
    void open();
    void addScene();
    void addTexture();
    void deleteScene();  
    void reloadAsset();
    void importScene();
    void showContextMenu(const QPoint&);
    void onTreeviewRightClick(const QPoint& pos);
    void onListItemDoubleClicked();
    static QPlainTextEdit* getConsoleTextEdit();

public slots:
    void refreshHierachy();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    static MainWindow* mainwindow;
    void initMenuSettings();
    void initMenuBar();
    Ui::MainWindow *ui;
    QFileSystemModel* fileModel;
    QPlainTextEdit* consoleTextEdit;
};
#endif // RENDERWINDOW_H
