#pragma once
#include "qtreewidget.h"
#include "HierarchyItem.h"
#include "Core/UI/mainwindow.h"
#include "ComponentsDockWidget.h"
#include "Core/UI/HierarchyItem.h"
#include "Core/Core/Scene.h"

class HierarchyWidget : public QTreeWidget
{
    friend class HierarchyItem;
    Q_OBJECT
signals:
    void gameObjectSelected(GameObject* gameobj);
public:
    HierarchyWidget(QWidget* parent);
    void refresh();
    void addGameObject(GameObject* gameobj,GameObject* parent = nullptr,InsertMode insertMode = INSIDE);

    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

    void initContextMenu();
    void refreshGameObject();
    

    QMenu* contextMenu;
    GameObject* selectedGameObject;
    ComponentsDockWidget* componentsDockWidget;
    std::unordered_map<GameObject*, HierarchyItem*> gameobj_item_map;
    // RenderWindow* ui;
    void insertItem(QTreeWidgetItem* oldItem, QTreeWidgetItem* newItem);

public slots:
    void showContextMenu(const QPoint& pos);
    void onSelectionChanged();

private:
    bool dragMode;
private:    
    QPoint startPos;
    QTreeWidgetItem* draggedItem;
    GameObject* needToBeCopiedGameObject;
};

