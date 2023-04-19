#include "Core/UI/AddSceneDialog.h"

AddSceneDialog::AddSceneDialog(QWidget* parent)
    : QDialog(parent)
{
    QLabel* sceneNameLabel = new QLabel(tr("Scene name:"));
    m_SceneNameLineEdit = new QLineEdit;

    m_createButton = new QPushButton(tr("Create Scene"));

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(sceneNameLabel, 0, 0);
    layout->addWidget(m_SceneNameLineEdit, 0, 1);
    layout->addWidget(m_createButton, 1, 1);

    connect(m_createButton, &QPushButton::clicked, this, &AddSceneDialog::createScene);

    setLayout(layout);
}


QString AddSceneDialog::getSceneName() const
{
    return m_SceneNameLineEdit->text();
}

void AddSceneDialog::createScene()
{
    auto sceneName = getSceneName();
    auto& instance = GameEngine::get_instance();
    instance.getCurrentGameProject()->openScene(GameEngine::get_instance().getCurrentGameProject()->creatNewScene(sceneName.toStdString()));// 需要一个弹框输入名字
    close();
}