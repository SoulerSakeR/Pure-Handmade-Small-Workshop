#include "HierarchyWidget.h"
#include "qmenu.h"
#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include "Core/SystemStatus/GameEngine.h"

void HierarchyWidget::onSelectionChanged()
{
	if (selectedItems().size() <= 1)
	{
		if (selectedItems().size() == 0)
		{
			selectedGameObject = nullptr;
			emit gameObjectSelected(nullptr);
		}
			
		else
		{
			auto item = (HierarchyItem*)selectedItems()[0];
			selectedGameObject = item->gameObject;
			emit gameObjectSelected(item->gameObject);
		}		
	}
}

void HierarchyWidget::initContextMenu()
{
	contextMenu = new QMenu(this);

	//创建物体子菜单
	QMenu *addGameobjectMenu = new QMenu("Creat GameObject", this);

	//创建空物体按钮
	QAction* addEmptyBtn = new QAction("Empty Object", this);
	addGameobjectMenu->addAction(addEmptyBtn);
	connect(addEmptyBtn, &QAction::triggered, [=]() {
		if (selectedItems().size() == 1)
		{
			auto item = (HierarchyItem*)(selectedItems()[0]);
			auto parent = item->gameObject;
			auto& gameobj = GameEngine::get_instance().addGameObject("GameObject", parent, Component::TRANSFORM, INSIDE);
			item->addChild(new HierarchyItem(&gameobj));
			item->setExpanded(true);
		}
		else
		{
			auto& gameobj = GameEngine::get_instance().addGameObject("GameObject");
			addTopLevelItem(new HierarchyItem(&gameobj));
		}
	});

	//创建图像物体按钮
	QAction* addImageGameObjectBtn = new QAction("Image", this);
	addGameobjectMenu->addAction(addImageGameObjectBtn);
	connect(addImageGameObjectBtn, &QAction::triggered, [=]() {
		if (selectedItems().size() == 1)
		{
			auto item = (HierarchyItem*)(selectedItems()[0]);
			auto parent = item->gameObject;
			auto& gameobj = GameEngine::get_instance().addGameObject("Image", parent, Component::IMAGE, INSIDE);
			item->addChild(new HierarchyItem(&gameobj));
			item->setExpanded(true);
		}
		else
		{
			auto& gameobj = GameEngine::get_instance().addGameObject("Image", nullptr, Component::IMAGE);
			addTopLevelItem(new HierarchyItem(&gameobj));
		}
	});
	contextMenu->addMenu(addGameobjectMenu);

	//附加组件菜单
	QMenu *addComponentMenu = new QMenu("Attach Component", this);
	for (int i = 1;i < Component::componentTypeCount + 1;++i)
	{
		auto action = new QAction(QString::fromStdString(Component::getName((Component::ComponentType)i)), this);
		addComponentMenu->addAction(action);
		connect(action, &QAction::triggered, [=]() {
			if (selectedGameObject != nullptr)
			{
				selectedGameObject->addComponent((Component::ComponentType)i);
				componentsDockWidget->refresh();
			}
		});
	}
	contextMenu->addMenu(addComponentMenu);

	QAction* moveUpBtn = new QAction("move up", this);
	contextMenu->addAction(moveUpBtn);
	connect(moveUpBtn, &QAction::triggered, [=]() {
		if (selectedItems().size() == 1)
		{
			auto item = (HierarchyItem*)selectedItems()[0];
			item->moveUp();
		}
	});
	QAction* moveDownBtn = new QAction("move down", this);
	contextMenu->addAction(moveDownBtn);
	connect(moveDownBtn, &QAction::triggered, [=]() {
		if (selectedItems().size() == 1)
		{
			auto item = (HierarchyItem*)selectedItems()[0];
			item->moveDown();
		}
	});
	QAction* copyBtn = new QAction("copy", this);
	contextMenu->addAction(copyBtn);
	connect(copyBtn, &QAction::triggered, [=]() {
		if (selectedItems().size() == 1)
		{
			//TODO: copy
		}
	});
	QAction* pasteBtn = new QAction("paste", this);
	contextMenu->addAction(pasteBtn);
	connect(pasteBtn, &QAction::triggered, [=]() {
		if (selectedItems().size() == 1)
		{
			//TODO: paste
		}
	});
	QAction* deleteBtn = new QAction("delete", this);
	contextMenu->addAction(deleteBtn);
	connect(deleteBtn, &QAction::triggered, [=]() {
		if (selectedItems().size() == 1)
		{
			auto item = (HierarchyItem*)selectedItems()[0];
			setCurrentItem(0);
			item->deleteItem();
		}
	});
	connect(this, &HierarchyWidget::customContextMenuRequested, this, &HierarchyWidget::showContextMenu);
}

void HierarchyWidget::insertItem(QTreeWidgetItem* oldItem, QTreeWidgetItem* newItem)
{
	if (newItem->parent() == nullptr) {
		// 新的项目是根节点
		if (oldItem->parent() != nullptr) {// 老的不是根节点
			QTreeWidgetItem* parentItem = oldItem->parent(); // 获取老节点的父亲节点
			parentItem->insertChild(newItem->indexOfChild(oldItem), oldItem); // 将老节点插入到新节点上面
		}
		else {//老的是根节点
			QTreeWidget* treeWidget = newItem->treeWidget();
			int index = treeWidget->indexOfTopLevelItem(newItem);
			treeWidget->insertTopLevelItem(index, oldItem);
			treeWidget->takeTopLevelItem(index + 1);
		}
	}
	else {
		if (newItem->parent() != nullptr) { // newItem不是根节点
			if (oldItem->parent() != nullptr) { // oldItem不是根节点
				if (oldItem->parent() == newItem->parent()) { // 在同一分支
					int newIndex = newItem->parent()->indexOfChild(newItem);
					int oldIndex = oldItem->parent()->indexOfChild(oldItem);
					if (newIndex > oldIndex) {
						newIndex--;
					}
					newItem->parent()->insertChild(newIndex, oldItem);
					oldItem->parent()->removeChild(oldItem);
				}
				else { // 不在同一分支
					int newIndex = newItem->parent()->indexOfChild(newItem);
					newItem->parent()->insertChild(newIndex, oldItem);
					oldItem->parent()->removeChild(oldItem);
				}
			}
			else { // oldItem是根节点
				int newIndex = newItem->indexOfChild(newItem);
				newItem->insertChild(newIndex, oldItem);
				oldItem->treeWidget()->takeTopLevelItem(oldItem->treeWidget()->indexOfTopLevelItem(oldItem));
			}
		}
		else { // newItem是根节点
			if (oldItem->parent() != nullptr) { // oldItem不是根节点
				int newIndex = newItem->indexOfChild(newItem);
				newItem->insertChild(newIndex, oldItem);
				oldItem->parent()->removeChild(oldItem);
			}
			else { // oldItem是根节点
				int newIndex = newItem->treeWidget()->indexOfTopLevelItem(newItem);
				newItem->treeWidget()->insertTopLevelItem(newIndex, oldItem);
				oldItem->treeWidget()->takeTopLevelItem(oldItem->treeWidget()->indexOfTopLevelItem(oldItem));
			}
		}
	}
}

void HierarchyWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (dragMode)// 拖拽状态成立
	{
		QTreeWidgetItem* curentItem = itemAt(event->pos());
		if (curentItem)
			insertItem(draggedItem, curentItem);//插入到当前项的上面
		dragMode = false;
	}
}

HierarchyWidget::HierarchyWidget(QWidget* parent):QTreeWidget(parent)
{
	connect(this, SIGNAL(itemSelectionChanged()),this, SLOT(onSelectionChanged()));	
}

void HierarchyWidget::mouseMoveEvent(QMouseEvent* event)
{// 按下左键开始进入拖拽
	if (event->buttons() & Qt::LeftButton)
	{
		draggedItem = currentItem();
		if (draggedItem == nullptr)
			return;
		bool dragMode = true;
	}
	QTreeWidget::mouseMoveEvent(event);
}

void HierarchyWidget::showContextMenu(const QPoint& pos)
{
	auto item = itemAt(pos);
	if (item) {
		QPoint globalPos = mapToGlobal(pos);
		contextMenu->exec(globalPos);
	}
}

