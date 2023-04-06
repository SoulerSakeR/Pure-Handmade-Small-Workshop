#include "renderwindow.h"
#include "ui_renderwindow.h"
#include <QDialog> // 模态对话框
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QListWidgetItem>
#include "Core/UI/listbox.h"
#include "Core/UI/listitem.h"
#include "Core/SystemStatus/GameEngine.h"

RenderWindow::RenderWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RenderWindow)
{
    ui->setupUi(this);

    // 点击按钮出现资源对话框
    connect(ui->actionopenProject,&QAction::triggered,[=](){
        // 文件对话框  参数1 父亲 参数2 标题 参数3 默认打开路径 参数4 过滤文件格式
        QString FileAdress = QFileDialog::getOpenFileName(this,"打开项目","");// 可以重载第四个参数，意义是筛选文件类型  "(*.txt)"

        // 设置文件打开方式
        QFile file(FileAdress);
        file.open(QIODevice::ReadOnly);

        // 暂时将读取的文件放入数组中
        QByteArray array = file.readAll();
    });
    // 点击创建项目按钮出现资源对话框
    connect(ui->actioncreatProject, &QAction::triggered, [=]() {
        // 文件对话框  参数1 父亲 参数2 标题 参数3 默认打开路径 参数4 过滤文件格式
        QString FileAdress = QFileDialog::getSaveFileName(this, "创建项目", "", "");// 可以重载第四个参数，意义是筛选文件类型  "(*.txt)"
        QString fileName = QFileInfo(FileAdress).fileName();
        GameEngine::get_instance().creatGameProject(fileName.toStdString(), FileAdress.toStdString());
        });

    QList<ListItem*> items;
    auto path = GameEngine::get_instance().getRootPath() + "\\resources\\0027.png";
    QString s = QString::fromStdString(path);
    for(int i=0;i<9;++i)
    {
        auto a = items.emplace_back(new ListItem(s, QString("item %0").arg(i), ui->listBox));
        ui->listBox->addItem(a);
    }
    QObject::connect(this, &QListWidget::customContextMenuRequested,this , &RenderWindow::showContextMenu);
    // 链接未完成4.4
    // QObject::connect(this, &QWidget::resizeEvent, ui->openGLWidget, &RenderWindow::resizeGL);
}

RenderWindow::~RenderWindow()
{
    delete ui;
}

// 显示右键菜单
void RenderWindow::showContextMenu(const QPoint& pos)
{
    // 创建右键菜单
    QMenu contextMenu(tr("Context menu"), this);
    QAction action("Action", this);
    contextMenu.addAction(&action);

    // 显示右键菜单
    QPoint globalPos = this->mapToGlobal(pos);
    contextMenu.exec(globalPos);
}

// 创建items变量
void RenderWindow::createItemsList()
{
    QList<ListItem*> items;
    auto path = GameEngine::get_instance().getRootPath() + "\\resources\\0027.png";
    QString s = QString::fromStdString(path);
    for(int i=0;i<3;++i)
  {
      auto a = items.emplace_back(new ListItem(s, QString("item %0").arg(i), ui->listBox));
      ui->listBox->addItem(a);
  }
}


void RenderWindow::resizeGL(QResizeEvent* event) {
    int width = ui->openGLWidget->width();
    int height = ui->openGLWidget->height();
    ui->openGLWidget->resize(width, height);
    event->accept();
}



// 以下是测试渲染的三个按钮
void RenderWindow::on_Render_action_triggered()
{
    ui->openGLWidget->drawShape(RenderWidget::Rect);

}

void RenderWindow::on_Clear_action_triggered()
{
    ui->openGLWidget->drawShape(RenderWidget::None);
}

void RenderWindow::on_actionWirefame_triggered()
{
    bool wirefame = ui->actionWirefame->isChecked();
    ui->openGLWidget->setWirefame(wirefame);
}
