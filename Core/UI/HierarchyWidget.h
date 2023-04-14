#pragma once
#include "qtreewidget.h"
#include "HierarchyItem.h"
#include "Core/UI/renderwindow.h"

class HierarchyWidget :
    public QTreeWidget
{
    Q_OBJECT
signals:
    void gameObjectSelected(GameObject* gameobj);
public:
    HierarchyWidget(QWidget* parent);

    void mouseMoveEvent(QMouseEvent* event) override;

    //void mouseReleaseEvent(QMouseEvent* event) override;

    QMenu* contextMenu;
    GameObject* selectedGameObject;
    // RenderWindow* ui;

public slots:
    void showContextMenu(const QPoint& pos);
    void onSelectionChanged();
       
private:
    void initContextMenu();
    QPoint startPos;
 
};

