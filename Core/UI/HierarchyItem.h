#pragma once
#include <qtreewidget.h>
#include <Core/Core/GameObject.h>
class HierarchyItem :
    public QTreeWidgetItem
{
public:
    GameObject* gameObject;
    HierarchyItem(GameObject* gameObject);
};

