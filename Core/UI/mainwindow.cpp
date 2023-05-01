#include "mainwindow.h"
#include "ui_mainwindow.h"
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
#include "Core/UI/ExportGameDialog.h"
#include <QMessageBox>
#include "Core/UI/AddSceneDialog.h"
#include "Core/GameLogic/GameLoop.h"
#include "RenderSettingDialog.h"
#include "Core/ResourceManagement/ResourceMgr.h"
#include "TextureEditorDialog.h"

MainWindow* MainWindow::mainwindow = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mainwindow = this;
    // 初始化菜单栏
    initMenuBar();

    // 将 sceneWidget 中的 hierarchyWidget 指针指向 ui->hierarchy
    ui->sceneWidget->hierarchyWidget = ui->hierarchy;
    ui->sceneWidget->isGameWidget = false;
    // 设置 gameWidget 的 isGameWidget 为 true
    ui->gameWidget->isGameWidget = true;
    

    // 绑定 RenderWidget 中的 sceneWidget 和 gameWidget 指针
    RenderWidget::sceneWidget = ui->sceneWidget;
    RenderWidget::gameWidget = ui->gameWidget;
    RenderWidget::currentWidget = ui->sceneWidget;

    // 将 hierarchy 指针指向 GameEngine 的 hierarchy，为后面操作 GameObject 做准备
    GameEngine::get_instance().hierarchy = ui->hierarchy;

    // 初始化控制台信息组件
    consoleTextEdit = ui->consoleTextEdit;
    consoleTextEdit->setReadOnly(true);// 只读
    
    // 当 tab 切换时，将 widgetChanged 置为 true
    connect(ui->tabWidget, &QTabWidget::currentChanged,&RenderWidget::on_widgetChanged);

    // 将 componentsDockWidget 指向 dockWidget_components
    ui->hierarchy->componentsDockWidget = ui->dockWidget_components;

    // 初始化 hierarchy 中的 contextMenu
    ui->hierarchy->initContextMenu();

    // 将 dockWidget_components 中的 hierarchy 指针指向 ui->hierarchy
    ui->dockWidget_components->hierarchy = ui->hierarchy;

    // 检查 gameWidget 是否开启了鼠标追踪
    auto x = ui->gameWidget->hasMouseTracking();

    // 根据组件类型动态生成组件按钮
    for (int i = 1;i < Component::componentTypeCount + 1;++i)
    {
        auto action = new QAction(QString::fromStdString(Component::getTypeName((Component::ComponentType)i)),this);
        ui->menu_component->addAction(action);
        connect(action, &QAction::triggered, [=]() {
            if (ui->hierarchy->selectedGameObject != nullptr)
            {
                ui->hierarchy->selectedGameObject->addComponent((Component::ComponentType)i);
                ui->dockWidget_components->refresh();
            }               
		});
    }
    // 将 dockWidget_components 中的 components_widget 指向 ui->scrollAreaWidgetContents
    ui->dockWidget_components->set_components_widget(ui->scrollAreaWidgetContents);

    // 连接 treeView 视图的右键点击事件
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::onTreeviewRightClick);

    // 连接 treeView 视图的左键双击事件
    connect(ui->treeView, &QTreeView::doubleClicked, this, &MainWindow::onListItemDoubleClicked);

    // 创建 新建项目 的 dialog，并绑定 actioncreatProject 的触发事件
    QDialog* createDialog = new CreateProjectDialog(this);
    connect(ui->actioncreatProject, &QAction::triggered, [=]() {
        createDialog->show();
        });

    // 打开项目，点击按钮出现资源对话框
    connect(ui->actionopenProject,&QAction::triggered,[=](){
        // 文件对话框  参数1 父亲 参数2 标题 参数3 默认打开路径 参数4 过滤文件格式
        QString FileAdress = QFileDialog::getOpenFileName(this,"打开项目","",QString("game project file (*.gameProject)"));// 可以重载第四个参数，意义是筛选文件类型  "(*.txt)"
        if (!FileAdress.isEmpty()) {
            GameEngine::get_instance().openGameProject(FileAdress.toStdString());
            // 获取上级目录
            QString parentDir = QFileInfo(FileAdress).dir().absolutePath();
            setupFileSystemTreeView(parentDir);
        }
        else {
            // 用户未选择文件
            QMessageBox::warning(this, "警告", "未选择文件！");
            return;
        }
    });

    // 绑定 保存项目 按钮的触发事件
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
    // 绑定 添加场景 的触发事件
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
        }
        else
        {
            auto& gameobj = GameEngine::get_instance().addGameObject("GameObject");
        }
    });

    // 绑定 actioncreatProject 的触发事件
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
            GameEngine::get_instance().addGameObject("Image", parent, Component::IMAGE,INSIDE);
        }
        else
        {
            GameEngine::get_instance().addGameObject("Image", nullptr, Component::IMAGE);
        }
        
    });
    // 创建 WaveFunctionCollapseDialog 对话框
    QDialog* wfcDialog = new WaveFunctionCollapseDialog(this);

    // 绑定 actionwfc 的触发事件，当 actionwfc 被触发时显示 wfcDialog 对话框
    connect(ui->actionwfc, &QAction::triggered, [=]() {
        wfcDialog->show();
        });

    // 隐藏 hierarchy 的表头
    ui->hierarchy->setHeaderHidden(true);

    // 绑定 HierarchyWidget 的 gameObjectSelected 信号与 ComponentsDockWidget 的 onGameObjectSelected 槽函数
    connect(ui->hierarchy, &HierarchyWidget::gameObjectSelected, ui->dockWidget_components, &ComponentsDockWidget::onGameObjectSelected);

    // 绑定 QListWidget 的 customContextMenuRequested 信号与 MainWindow 的 showContextMenu 槽函数
    QObject::connect(this, &QListWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);

    // 点击播放
    connect(ui->pushButton, &QPushButton::clicked, [=]() {
        if (GameEngine::get_instance().getCurrentGameProject() == nullptr)
        {
            Debug::warningBox(this, "No project is loaded!");
            return;
        }
        if (GameEngine::get_instance().needToSave())
        {
            QMessageBox::StandardButton button;
            button = QMessageBox::question(this, "Save", QString("Before playing game, you need to save the game project first !"), QMessageBox::Save | QMessageBox::Cancel, QMessageBox::Save);
            if (button == QMessageBox::Save)
            {
                GameEngine::get_instance().saveGameProject();
            }
            else
            {
				return;
            }
        }
        GameEngine::get_instance().gameLoop->setPlayingStatus(true);
        GameEngine::get_instance().gameLoop->shutdown();
        ui->hierarchy->setCurrentItem(nullptr);
        while (!GameEngine::get_instance().gameLoop->isClosed || RenderWidget::getCurrentWidget().isRendering)
        {
            // 等待Loop关闭
        }
        ui->tabWidget->setCurrentIndex(1);
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(true);       
        auto gameLoop = std::bind(&GameLoop::startGameLoop, GameEngine::get_instance().gameLoop);
        GameEngine::get_instance().pool.enqueue(gameLoop);
        });

    // 点击停止
    connect(ui->pushButton_2, &QPushButton::clicked, [=]() {
        GameEngine::get_instance().gameLoop->shutdown();
        ui->hierarchy->setCurrentItem(nullptr);
        while (!GameEngine::get_instance().gameLoop->isClosed || RenderWidget::getCurrentWidget().isRendering)
        {
            // 等待Loop关闭
        }
        GameEngine::get_instance().gameLoop->setPlayingStatus(false);
        ui->tabWidget->setCurrentIndex(0);
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(false);       
        auto renderLoop = std::bind(&GameLoop::startRenderLoop, GameEngine::get_instance().gameLoop);
        GameEngine::get_instance().pool.enqueue(renderLoop);
        });

    // 实现Window菜单控制Hierarchy
    connect(ui->actionHierarchy, &QAction::triggered, this, [=]() {
        if (ui->dockWidget_hierarchy->isVisible()) {
            ui->dockWidget_hierarchy->hide();
            ui->actionHierarchy->setChecked(false);
        }
        else {
            ui->dockWidget_hierarchy->show();
            ui->actionHierarchy->setChecked(true);
        }
        });

    // 关闭Hierarchy窗口时取消勾选Window菜单中的选项
    connect(ui->dockWidget_hierarchy, &QDockWidget::visibilityChanged, this, [=](bool visible) {
        if (!visible) {
            ui->actionHierarchy->setChecked(false);
        }
        });

    // 实现Window菜单控制Resource Manager
    connect(ui->actionResorce_Manager, &QAction::triggered, this, [=]() {
        if (ui->dockWidget_resourceManager->isVisible()) {
            ui->dockWidget_resourceManager->hide();
            ui->actionResorce_Manager->setChecked(false);
        }
        else {
            ui->dockWidget_resourceManager->show();
            ui->actionResorce_Manager->setChecked(true);
        }
        });

    // 关闭Resource Manager窗口时取消勾选Window菜单中的选项
    connect(ui->dockWidget_resourceManager, &QDockWidget::visibilityChanged, this, [=](bool visible) {
        if (!visible) {
            ui->actionResorce_Manager->setChecked(false);
        }
        });

    // 实现Window菜单控制Console
    connect(ui->actionConsole, &QAction::triggered, this, [=]() {
        if (ui->dockWidget_console->isVisible()) {
            ui->dockWidget_console->hide();
            ui->actionConsole->setChecked(false);
        }
        else {
            ui->dockWidget_console->show();
            ui->actionConsole->setChecked(true);
        }
        });

    // 关闭Console窗口时取消勾选Window菜单中的选项
    connect(ui->dockWidget_console, &QDockWidget::visibilityChanged, this, [=](bool visible) {
        if (!visible) {
            ui->actionConsole->setChecked(false);
        }
        });

    // 实现Window菜单控制Component
    connect(ui->actionComponents, &QAction::triggered, this, [=]() {
        if (ui->dockWidget_components->isVisible()) {
            ui->dockWidget_components->hide();
            ui->actionComponents->setChecked(false);
        }
        else {
            ui->dockWidget_components->show();
            ui->actionComponents->setChecked(true);
        }
        });

    // 关闭Component窗口时取消勾选Window菜单中的选项
    connect(ui->dockWidget_components, &QDockWidget::visibilityChanged, this, [=](bool visible) {
        if (!visible) {
            ui->actionComponents->setChecked(false);
        }
        });

    // 实现项目导出功能
    connect(ui->actionExport_current_project, &QAction::triggered, this, [=]() {
        if (GameEngine::get_instance().gameProject != nullptr)
        {
            
            QMessageBox::StandardButton button;
            button = QMessageBox::question(this, tr("关闭"), QString(tr("导出前需要保存当前项目，是否保存项目？")), QMessageBox::Save | QMessageBox::Cancel, QMessageBox::Save);
            if (button == QMessageBox::Save)
            {
                bool saveSuccess = GameEngine::get_instance().saveGameProject();
                if (saveSuccess) {
                    ExportGameDialog exportDialog(this);
                    exportDialog.exec();
                }
            }
        }
        else
        {
            Debug::warningBox(this, "你还没有打开项目!");
        }
        });
}

MainWindow::~MainWindow()
{
    delete ui;
}
// 显示右键菜单
void MainWindow::showContextMenu(const QPoint& pos)
{
    /*
    // 创建右键菜单
    QMenu contextMenu(tr("Context menu"), this);
    QAction action("Action", this);
    contextMenu.addAction(&action);

    // 显示右键菜单
    QPoint globalPos = this->mapToGlobal(pos);
    contextMenu.exec(globalPos);
    */
}

void MainWindow::resizeGL(QResizeEvent* event) {
    int width = ui->sceneWidget->width();
    int height = ui->sceneWidget->height();
    ui->sceneWidget->resize(width, height);
    event->accept();
}
// 创建QFileSystemModel并显示上级目录下的文件结构
void MainWindow::setupFileSystemTreeView(const QString& parentDir)
{
    fileModel = new QFileSystemModel(ui->treeView);
    fileModel->setRootPath(parentDir);
    QStringList filters;
    filters << "*.scene" << "*.png"<<"*.texture"<<"*.gameProject";
    fileModel->setNameFilters(filters);
    fileModel->setNameFilterDisables(false);

    ui->treeView->setModel(fileModel);
    ui->treeView->setRootIndex(fileModel->index(parentDir));

    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
}
// 场景的点击事件
void MainWindow::onTreeviewRightClick(const QPoint& pos) {
    QModelIndex index = ui->treeView->indexAt(pos);
    if (index.isValid()) {
        // 弹出一个弹框
        QMenu menu(this);
        auto openAction = menu.addAction("Open");
        auto createMenu = menu.addMenu("Create");
        auto addSceneAction = createMenu->addAction("New Scene");
        auto addTextureAction = createMenu->addAction("New Texture2D");
        auto importExistSceneAction = menu.addAction("Import Exist Scene");
        auto deleteSceneAction = menu.addAction("Delete Scene");
        auto reloadAssetAction = menu.addAction("Reload Asset");
        // 将QAction与槽函数绑定
        connect(openAction, &QAction::triggered, this, &MainWindow::open);
        connect(addSceneAction, &QAction::triggered, this, &MainWindow::addScene);
        connect(addTextureAction, &QAction::triggered, this, &MainWindow::addTexture);
        connect(deleteSceneAction, &QAction::triggered, this, &MainWindow::deleteScene);
        connect(importExistSceneAction, &QAction::triggered, this, &MainWindow::importScene);
        connect(reloadAssetAction, &QAction::triggered, this, &MainWindow::reloadAsset);
        menu.exec(ui->treeView->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::refreshHierachy()
{
    ui->hierarchy->refresh();
}

void MainWindow::onListItemDoubleClicked()
{
    // 处理双击事件
    open();
}

void MainWindow::open()
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
    else if(path.getFileType() == ".texture")
    {
        auto texture = ResourceMgr::get_instance().loadFromName<Texture2D>(path.getFileName(false));
        auto dialog = new TextureEditorDialog(texture, this);
        dialog->exec();
    }
    else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(currentPath));
    }   
}

void MainWindow::addScene()
{
    // 具体的代码，传入文件地址
    // 获取当前选择的项
    QDialog* createSceneDialog = new AddSceneDialog(this);
    createSceneDialog->show();
}

void MainWindow::addTexture()
{
	// 具体的代码，传入文件地址
	// 获取当前选择的项
	/*QDialog* createTextureDialog = new AddTextureDialog(this);
	createTextureDialog->show();*/
    auto res = ResourceMgr::get_instance().CreatNewTexture2D("","");
    auto dialog =new TextureEditorDialog(res, this);
    dialog->exec();
}
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

void MainWindow::deleteScene()
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
        QMessageBox::information(nullptr, "Success", "Scene deleted successfully!");
    }
    else {
        QMessageBox::information(nullptr, "Failed", "Failed to delete scene!");
    }
}

void MainWindow::reloadAsset()
{
    ResourceMgr::get_instance().loadAllAssets();
}

void MainWindow::importScene()
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
        return;
	}
}
// 重写关闭窗口事件
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (GameEngine::get_instance().gameProject == nullptr)
    {
         QMainWindow::closeEvent(event);
    }
    else if(GameEngine::get_instance().needToSave())
    {
        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("关闭"), QString(tr("是否保存项目？")), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
        if (button == QMessageBox::Save)
        {
            bool saveSuccess = GameEngine::get_instance().saveGameProject();
            if (saveSuccess) {
                GameEngine::get_instance().gameLoop->shutdown();
                while (!GameEngine::get_instance().readyToClose())
                {
                    // 等待关闭
                }
                QMainWindow::closeEvent(event);
            }
        }
        else if (button == QMessageBox::Discard)
        {
            GameEngine::get_instance().gameLoop->shutdown();
            while (!GameEngine::get_instance().readyToClose())
            {
                // 等待关闭
            }
            QMainWindow::closeEvent(event);
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
		GameEngine::get_instance().gameLoop->shutdown();
        while (!GameEngine::get_instance().readyToClose())
        {
            // 等待关闭
        }
		QMainWindow::closeEvent(event);
    }
}

void MainWindow::initMenuSettings()
{
    connect(ui->actionRenderSetting, &QAction::triggered, [=]()
    {
        if (GameEngine::get_instance().gameProject == nullptr)
        {
			Debug::warningBox(this,"Please create a project first!");
			return;
		}
        RenderSettingDialog* renderSettingDialog = new RenderSettingDialog(this);
		renderSettingDialog->show();
    });
}

void MainWindow::initMenuBar()
{
    initMenuSettings();
}
// 返回consoleTextEdit以供调用
QPlainTextEdit* MainWindow::getConsoleTextEdit()
{
    return mainwindow->consoleTextEdit;
}
