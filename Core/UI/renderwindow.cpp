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
#include "Core/UI/WaveFunctionCollapseDialog.h"
#include <QMessageBox>


RenderWindow::RenderWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RenderWindow)
{
    ui->setupUi(this);
    // ui->hierarchy->ui = this;
    RenderWidget::sceneWidget = ui->openGLWidget;
    RenderWidget::gameWidget = ui->openGLWidget_2;
    ui->hierarchy->componentsDockWidget = ui->dockWidget_components;
    ui->hierarchy->initContextMenu();
    auto x= ui->openGLWidget->hasMouseTracking();
    // 根据组件类型动态生成组件按钮
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
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &RenderWindow::onTreeviewRightClick);
    QDialog* createDialog = new CreateProjectDialog(this);
    //connect(createDialog, &CreateProjectDialog::accepted, this, &RenderWindow::on_saveBtn_clicked);
    // 点击创建项目按钮出现资源对话框
    connect(ui->actioncreatProject, &QAction::triggered, [=]() {
        createDialog->show();
        });
    //createDialog->setModal(true); // QDialog窗口模态属性

    // 打开项目，点击按钮出现资源对话框
    connect(ui->actionopenProject,&QAction::triggered,[=](){
        // 文件对话框  参数1 父亲 参数2 标题 参数3 默认打开路径 参数4 过滤文件格式
        QString FileAdress = QFileDialog::getOpenFileName(this,"打开项目","",QString("game project file (*.gameProject)"));// 可以重载第四个参数，意义是筛选文件类型  "(*.txt)"
        GameEngine::get_instance().openGameProject(FileAdress.toStdString());
        // 获取上级目录
        QString parentDir = QFileInfo(FileAdress).dir().absolutePath();

        setupFileSystemTreeView(parentDir);
    });
    // 连接保存按钮
    connect(ui->actionsaveProject, &QAction::triggered, [=]() {
        
        // 实现保存操作，返回保存是否成功的状态
        bool saveSuccess = GameEngine::get_instance().saveGameProject();

        // 创建一个消息框，提示保存是否成功
        QMessageBox msgBox;
        msgBox.setWindowTitle("保存项目");
        
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        QSpacerItem* horizontalSpacer = new QSpacerItem(100, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QVBoxLayout* layout = (QVBoxLayout*)msgBox.layout();
        layout->addItem(horizontalSpacer);
        if (saveSuccess) {
            msgBox.setText("项目保存成功！");
        }
        else {
            msgBox.setText("项目保存失败！");
        }
        msgBox.exec();
    });
    connect(ui->actionCreatEmptyGameObject, &QAction::triggered, [=]() {
        if (GameEngine::get_instance().getCurrentScene() == nullptr)
        {
            Debug::warningBox(this, "No scene is loaded!");
            return;
        }
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
        if (GameEngine::get_instance().getCurrentScene() == nullptr)
        {
            Debug::warningBox(this, "No scene is loaded!");
            return;
        }
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
    QDialog* wfcDialog = new WaveFunctionCollapseDialog(this);
    connect(ui->actionwfc, &QAction::triggered, [=]() {
        wfcDialog->show();
    });
    ui->hierarchy->setHeaderHidden(true);
    
    connect(ui->hierarchy, &HierarchyWidget::gameObjectSelected, ui->dockWidget_components, &ComponentsDockWidget::onGameObjectSelected);
    QObject::connect(this, &QListWidget::customContextMenuRequested,this , &RenderWindow::showContextMenu);
    
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

void RenderWindow::resizeGL(QResizeEvent* event) {
    int width = ui->openGLWidget->width();
    int height = ui->openGLWidget->height();
    ui->openGLWidget->resize(width, height);
    event->accept();
}

// 创建QFileSystemModel并显示上级目录下的文件结构
void RenderWindow::setupFileSystemTreeView(const QString& parentDir)
{
    fileModel = new QFileSystemModel(ui->treeView);
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
}

// 场景的点击事件
void RenderWindow::onTreeviewRightClick(const QPoint& pos) {
    QModelIndex index = ui->treeView->indexAt(pos);
    if (index.isValid()) {
        // 弹出一个弹框
        QMenu menu(this);
        auto openAction = menu.addAction("Open");
        auto createMenu = menu.addMenu("Create");
        auto addSceneAction = createMenu->addAction("New Scene");
        auto importExistSceneAction = menu.addAction("Import Exist Scene");
        auto deleteSceneAction = menu.addAction("Delete Scene");
        // 将QAction与槽函数绑定
        connect(openAction, &QAction::triggered, this, &RenderWindow::open);
        connect(addSceneAction, &QAction::triggered, this, &RenderWindow::addScene);
        connect(deleteSceneAction, &QAction::triggered, this, &RenderWindow::deleteScene);
        connect(importExistSceneAction, &QAction::triggered, this, &RenderWindow::importScene);
        menu.exec(ui->treeView->viewport()->mapToGlobal(pos));
    }
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
}

void RenderWindow::open()
{
    // 获取当前选择的项
    QModelIndex currentIndex = ui->treeView->currentIndex();
    // 获取当前项的路径
    QString currentPath = fileModel->filePath(currentIndex);
    PHPath path(currentPath.toStdString());
    if (path.getFileType() == ".scene")
    {
        GameEngine::get_instance().getCurrentGameProject()->openScene(path.getFileName(false));
    }
    else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(currentPath));
    }   
}

void RenderWindow::addScene()
{
    // QString FileAdress = QFileDialog::getSaveFileName(this, "保存文件", "", "");
    // TODO
    // 具体的代码，传入文件地址
    // 获取当前选择的项
    QModelIndex currentIndex = ui->treeView->currentIndex();
    // 获取当前项的路径
    QString currentPath = fileModel->filePath(currentIndex);
    qDebug() << currentPath;
    auto& instance = GameEngine::get_instance();
    instance.getCurrentGameProject()->openScene(GameEngine::get_instance().getCurrentGameProject()->creatNewScene("1"));// 需要一个弹框输入名字
}

// 传入需要删除的文件的绝对路径 
// 删除需要确认   标记完善一下***************************************************
bool deleteFile(const QString& filePath)
{
    QFile file(filePath);
    if (file.exists()) {
        if (file.remove()) {
            QMessageBox::information(nullptr, "Success", "File deleted successfully!");
            return true;
        }
        else {
            QMessageBox::information(nullptr, "Failed", "Failed to delete file!");
        }
    }
    return false;
}
void RenderWindow::deleteScene()
{
    // TODO
    // 获取当前选择的项
    QModelIndex currentIndex = ui->treeView->currentIndex();
    // 获取当前项的路径
    QString currentPath = fileModel->filePath(currentIndex);
    PHPath path(currentPath.toStdString());
    GameEngine::get_instance().getCurrentGameProject()->deleteScene(path.getFileName(false));
    qDebug() << currentPath;
    // 调用删除文件
    if (deleteFile(currentPath)) {
        qDebug() << "File deleted successfully!";
    }
    else {
        qDebug() << "Failed to delete file!";
    }
}

void RenderWindow::importScene()
{
    QString FileAdress = QFileDialog::getOpenFileName(this, "打开场景", "", QString("Scene file (*.scene)"));// 可以重载第四个参数，意义是筛选文件类型  "(*.txt)"
    if (FileAdress.isEmpty())
    {
		return;
	}
    else
    {
		// TODO
		// 具体的代码，传入文件地址
	}
}

