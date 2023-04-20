#pragma once
#include <qtreewidget.h>
#include <Core/Core/GameObject.h>

class HierarchyWidget;

class HierarchyItem : public QTreeWidgetItem
{
public:
    GameObject* gameObject;
    HierarchyWidget* tree;
    HierarchyItem(GameObject* gameObject, HierarchyWidget* tree);
    ~HierarchyItem();
    void moveUp();
    void moveDown();
    void deleteItem();
    void destroyGameObject();
};

