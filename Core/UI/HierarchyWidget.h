#pragma once
#include <qtreewidget.h>
#include "HierarchyItem.h"

class HierarchyWidget :
    public QTreeWidget
{
    Q_OBJECT
signals:
    void gameObjectSelected(GameObject* gameobj);
public:
    HierarchyWidget(QWidget* parent);

    QMenu* contextMenu;
    GameObject* selectedGameObject;
public slots:
    void showContextMenu(const QPoint& pos);
    void onSelectionChanged();
       
private:
    void initContextMenu();
};

