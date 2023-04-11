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

    GameObject* selectedGameObject;
private slots:
    void onSelectionChanged();
};

