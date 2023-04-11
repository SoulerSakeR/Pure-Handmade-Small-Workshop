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
#include "HierarchyItem.h"
#include "HierarchyWidget.h"
#include "ComponentsDockWidget.h"
#include "Core/UI/CreateProjectDialog.h"
//#include <QLineEdit>
//#include <qlabel.h>
//#include <QPushButton>

RenderWindow::RenderWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RenderWindow)
{
    ui->setupUi(this);
    for (int i = 1;i < Component::componentTypeCount + 1;++i)
    {
        auto action = new QAction(QString::fromStdString(Component::getName((Component::ComponentType)i)),this);
        ui->menu_component->addAction(action);
        connect(action, &QAction::triggered, [=]() {
            if (ui->hierarchy->selectedGameObject != nullptr)
            {
                ui->hierarchy->selectedGameObject->addComponent((Component::ComponentType)i);
                ui->dockWidget_components->refresh();
            }
                
		});
    }
    ui->dockWidget_components->set_components_widget(ui->scrollAreaWidgetContents);
    /*// 以下是Dialog界面的设计
    QDialog* createDialog = new QDialog(this);
    createDialog->setWindowTitle("创建项目");
    createDialog->setFixedSize(300, 200);
    createDialog->setModal(true);

    // 在createDialog窗口中添加文本输入框和保存按钮
    projectNameLineEdit = new QLineEdit;
    createAddressLineEdit = new QLineEdit;
    QLabel* projectNameLabel = new QLabel(tr("项目名称："));
    QLabel* createAddressLabel = new QLabel(tr("创建地址："));
    QPushButton* saveBtn = new QPushButton(tr("创建项目"));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(projectNameLabel);
    layout->addWidget(projectNameLineEdit);
    layout->addWidget(createAddressLabel);
    layout->addWidget(createAddressLineEdit);
    layout->addWidget(saveBtn);

    createDialog->setLayout(layout);
    */

    QDialog* createDialog = new CreateProjectDialog(this);
    //connect(createDialog, &CreateProjectDialog::accepted, this, &RenderWindow::on_saveBtn_clicked);
    // 点击创建项目按钮出现资源对话框
    connect(ui->actioncreatProject, &QAction::triggered, [=]() {
        // 文件对话框  参数1 父亲 参数2 标题 参数3 默认打开路径 参数4 过滤文件格式
        //QString FileAdress = QFileDialog::getSaveFileName(this, "创建项目", "", "");// 可以重载第四个参数，意义是筛选文件类型  "(*.txt)"
        //QString fileName = QFileInfo(FileAdress).fileName();
        //GameEngine::get_instance().creatGameProject(fileName.toStdString(), FileAdress.toStdString());
        createDialog->show();
        });
    //createDialog->setModal(true); // QDialog窗口模态属性

    // 点击按钮出现资源对话框
    connect(ui->actionopenProject,&QAction::triggered,[=](){
        // 文件对话框  参数1 父亲 参数2 标题 参数3 默认打开路径 参数4 过滤文件格式
        QString FileAdress = QFileDialog::getOpenFileName(this,"打开项目","");// 可以重载第四个参数，意义是筛选文件类型  "(*.txt)"
        GameEngine::get_instance().openGameProject(FileAdress.toStdString());
        /*
         // 设置文件打开方式
        QFile file(FileAdress);
        file.open(QIODevice::ReadOnly);
        // 暂时将读取的文件放入数组中
        QByteArray array = file.readAll();
        */


        // 获取上级目录
        QString parentDir = QFileInfo(FileAdress).dir().absolutePath();

        // 创建QFileSystemModel并显示上级目录下的文件结构
        QFileSystemModel* fileModel = new QFileSystemModel(ui->treeView);
        fileModel->setRootPath(parentDir);

        QStringList filters;
        filters << "*.scene" << "*.png";
        fileModel->setNameFilters(filters);
        fileModel->setNameFilterDisables(false);

        ui->treeView->setModel(fileModel);
        ui->treeView->setRootIndex(fileModel->index(parentDir));

        ui->treeView->hideColumn(1);
        ui->treeView->hideColumn(2);
        ui->treeView->hideColumn(3);
    });
    connect(ui->actionsaveProject, &QAction::triggered, [=]() {
        GameEngine::get_instance().saveGameProject();
    });
    connect(ui->actionCreatEmptyGameObject, &QAction::triggered, [=]() {
        if (ui->hierarchy->selectedItems().size() == 1)
        {
            auto item = (HierarchyItem*)(ui->hierarchy->selectedItems()[0]);
            auto parent = item->gameObject;
            auto& gameobj = GameEngine::get_instance().addGameObject("GameObject", parent, Component::TRANSFORM, INSIDE);
            item->addChild(new HierarchyItem(&gameobj));
            item->setExpanded(true);
        }            
        else
        {
            auto& gameobj = GameEngine::get_instance().addGameObject("GameObject");
            ui->hierarchy->addTopLevelItem(new HierarchyItem(&gameobj));
        }
    });
    connect(ui->actionCreatGameObjectWithImage, &QAction::triggered, [=]() {
        if (ui->hierarchy->selectedItems().size() == 1)
        {
            auto item = (HierarchyItem*)(ui->hierarchy->selectedItems()[0]);
            auto parent = item->gameObject;
            auto& gameobj = GameEngine::get_instance().addGameObject("Image", parent, Component::IMAGE,INSIDE);
            item->addChild(new HierarchyItem(&gameobj));
            item->setExpanded(true);
        }
        else
        {
            auto& gameobj = GameEngine::get_instance().addGameObject("Image", nullptr, Component::IMAGE);
            ui->hierarchy->addTopLevelItem(new HierarchyItem(&gameobj));
        }
        
    });
    ui->hierarchy->setHeaderHidden(true);
    /*
    QList<ListItem*> items;
    auto path = GameEngine::get_instance().getRootPath() + "\\resources\\0027.png";
    QString s = QString::fromStdString(path);
    for(int i=0;i<9;++i)
    {
        auto a = items.emplace_back(new ListItem(s, QString("item %0").arg(i), ui->listBox));
        ui->listBox->addItem(a);
    }
    */
    connect(ui->hierarchy, &HierarchyWidget::gameObjectSelected, ui->dockWidget_components, &ComponentsDockWidget::onGameObjectSelected);
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

/*
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
*/

void RenderWindow::resizeGL(QResizeEvent* event) {
    int width = ui->openGLWidget->width();
    int height = ui->openGLWidget->height();
    ui->openGLWidget->resize(width, height);
    event->accept();
}

void RenderWindow::refreshHierachy()
{
    ui->hierarchy->clear();    
    auto scene = GameEngine::get_instance().getCurrentScene();
    if (scene == nullptr)
        return;
    auto items = QList<QTreeWidgetItem*>();
    for (auto obj : scene->getRootGameObjs())
    {
        HierarchyItem* item = new HierarchyItem(obj);
        items.emplaceBack(item);
    }
    ui->hierarchy->addTopLevelItems(items);
}/*
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
}*/
