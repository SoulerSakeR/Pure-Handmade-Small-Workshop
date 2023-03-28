#include "renderwindow.h"
#include "ui_renderwindow.h"
#include <QDialog> // 模态对话框
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QListWidgetItem>>
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

    QList<ListItem*> items;
    auto path = GameEngine::getInstance()->getRootPath() + "\\resources\\0027.png";
    QString s = QString::fromStdString(path);
    for(int i=0;i<3;++i)
    {
        auto a = items.emplace_back(new ListItem(s, QString("item %0").arg(i), ui->listBox));
        ui->listBox->addItem(a);
    }
}

RenderWindow::~RenderWindow()
{
    delete ui;
}

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
