#pragma once
#include "qtreewidget.h"
#include "HierarchyItem.h"
#include "Core/UI/renderwindow.h"
#include "ComponentsDockWidget.h"

class HierarchyWidget :
    public QTreeWidget
{
    Q_OBJECT
signals:
    void gameObjectSelected(GameObject* gameobj);
public:
    HierarchyWidget(QWidget* parent);

    void mouseMoveEvent(QMouseEvent* event) override;

    void initContextMenu();
    //void mouseReleaseEvent(QMouseEvent* event) override;

    QMenu* contextMenu;
    GameObject* selectedGameObject;
    ComponentsDockWidget* componentsDockWidget;
    // RenderWindow* ui;

public slots:
    void showContextMenu(const QPoint& pos);
    void onSelectionChanged();

private:
    
    QPoint startPos;
};

