#ifndef AddSceneDialog_H
#define AddSceneDialog_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include "Core/SystemStatus/GameEngine.h"

class AddSceneDialog : public QDialog
{
    Q_OBJECT

public:
    AddSceneDialog(QWidget* parent = nullptr);

    QString getSceneName() const;

private slots:
    void createScene();

private:
    QLineEdit* m_SceneNameLineEdit;
    QPushButton* m_createButton;
};
#endif // AddSceneDialog_H
