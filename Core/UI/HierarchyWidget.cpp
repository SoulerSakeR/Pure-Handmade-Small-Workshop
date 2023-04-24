#include "HierarchyWidget.h"
#include "qmenu.h"
#include <QMouseEvent>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include "Core/SystemStatus/GameEngine.h"
#include <Core/ResourceManagement/SceneMgr.h>

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
		}
		else
		{
			auto& gameobj = GameEngine::get_instance().addGameObject("GameObject");
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
		}
		else
		{
			auto& gameobj = GameEngine::get_instance().addGameObject("Image", nullptr, Component::IMAGE);
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
				switch (i)
				{
				case Component::IMAGE:
					selectedGameObject->addComponent<Renderer>();
					break;
				case Component::CAMERA:
					selectedGameObject->addComponent<Renderer>();
					break;
				case Component::BOX_COLLIDER:
					selectedGameObject->addComponent<Renderer>();
					break;
				case Component::TEXT:
					selectedGameObject->addComponent<Renderer>();
					break;
				default:
					break;
				}
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
			needToBeCopiedGameObject = ((HierarchyItem*)selectedItems()[0])->gameObject;
		}
	});
	QAction* pasteBtn = new QAction("paste", this);
	contextMenu->addAction(pasteBtn);
	connect(pasteBtn, &QAction::triggered, [=]() {
		if (selectedItems().size() == 1)
		{
			auto item = (HierarchyItem*)selectedItems()[0];
			if(needToBeCopiedGameObject == nullptr)
				return;
			auto gameobj = needToBeCopiedGameObject->clone(needToBeCopiedGameObject->get_name(),item->gameObject);
			item->addChild(new HierarchyItem(gameobj,this));
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
	if (auto scene = GameEngine::get_instance().getCurrentScene();scene != nullptr)
		scene->insertExistGameObject(((HierarchyItem*)oldItem)->gameObject, ((HierarchyItem*)newItem)->gameObject, BEFORE);
	// 将老节点从老的父亲节点上面移除
	if (auto oldParent = oldItem->parent();oldParent != nullptr)
	{
		auto index = oldParent->indexOfChild(oldItem);
		oldParent->takeChild(index);
	}
	else
	{
		auto index = indexOfTopLevelItem(oldItem);
		takeTopLevelItem(index);
	}

	// 将老节点插入到新节点上面
	// 新的项目是根节点
	if (newItem->parent() == nullptr) 
	{
		auto index = indexOfTopLevelItem(newItem);
		insertTopLevelItem(index, oldItem); // 将老节点插入到新节点上面
	}
	else { //新的不是根节点
		auto index = newItem->parent()->indexOfChild(newItem);
		newItem->parent()->insertChild(index, oldItem);
	}
}

void HierarchyWidget::refreshGameObject()
{
	if (selectedGameObject != nullptr)
	{
		auto item = gameobj_item_map[selectedGameObject];
		item->setText(0, QString::fromStdString(selectedGameObject->get_name()));
	}
}


void HierarchyWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (dragMode)// 拖拽状态成立
	{
		QTreeWidgetItem* item = itemAt(event->pos());
		if (item != nullptr&& item!=draggedItem)
		{
			Debug::log("drag item is " + ((HierarchyItem*)draggedItem)->gameObject->get_name() + "  current item is " + item->text(0).toStdString());
			insertItem(draggedItem, item);
		}
		dragMode = false;
	}
}

void HierarchyWidget::mousePressEvent(QMouseEvent* event)
{
	QTreeWidget::mousePressEvent(event);
	if (event->buttons() & Qt::LeftButton)
	{
		auto item = itemAt(event->pos());
		if (item != nullptr)
		{
			setCurrentItem(item);
		}
		else
		{
			setCurrentItem(nullptr);
		}
	}
}

void HierarchyWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		auto item = itemAt(event->pos());
		if (item != nullptr)
		{
			if(currentItem()!=item)
				setCurrentItem(item);
			RenderWidget::getSceneWidget().lookAt(((HierarchyItem*)item)->gameObject);
		}
	}
}

HierarchyWidget::HierarchyWidget(QWidget* parent):QTreeWidget(parent)
{
	gameobj_item_map = std::unordered_map<GameObject*, HierarchyItem*>();
	dragMode = false;
	connect(this, SIGNAL(itemSelectionChanged()),this, SLOT(onSelectionChanged()));	
}

void HierarchyWidget::refresh()
{
	clear();
	gameobj_item_map.clear();
	auto scene = GameEngine::get_instance().getCurrentScene();
	if (scene == nullptr)
		return;
	auto items = QList<QTreeWidgetItem*>();
	for (auto obj : scene->getRootGameObjs())
	{
		HierarchyItem* item = new HierarchyItem(obj,this);
		items.emplaceBack(item);
	}
	addTopLevelItems(items);
}

void HierarchyWidget::addGameObject(GameObject* gameobj, GameObject* parent, InsertMode insertMode)
{
	if (parent == nullptr)
	{
		auto item = new HierarchyItem(gameobj,this);
		addTopLevelItem(item);		
	}
	else
	{
		auto parentItem = gameobj_item_map[parent];
		switch (insertMode)
		{
		case BEFORE:
			if (parentItem->parent() == nullptr)
			{
				auto index = indexOfTopLevelItem(parentItem);
				auto item = new HierarchyItem(gameobj,this);
				insertTopLevelItem(index , item);
			}
			else
			{
				auto index = parentItem->parent()->indexOfChild(parentItem);
				parentItem->parent()->insertChild(index , new HierarchyItem(gameobj,this));
			}
			break;
		case AFTER:
			if (parentItem->parent() == nullptr)
			{
				auto index = indexOfTopLevelItem(parentItem);
				insertTopLevelItem(index + 1, new HierarchyItem(gameobj,this));
			}
			else
			{
				auto index = parentItem->parent()->indexOfChild(parentItem);
				parentItem->parent()->insertChild(index + 1, new HierarchyItem(gameobj,this));
			}
			break;
		case INSIDE:
			parentItem->addChild(new HierarchyItem(gameobj,this));
			parentItem->setExpanded(true);
			break;
		default:
			break;
		}
	}
}

void HierarchyWidget::mouseMoveEvent(QMouseEvent* event)
{// 按下左键开始进入拖拽
	if (event->buttons() & Qt::LeftButton && !dragMode)
	{
		draggedItem = currentItem();
		if (draggedItem == nullptr)
			return;
		dragMode = true;
	}
}

void HierarchyWidget::showContextMenu(const QPoint& pos)
{
	auto item = itemAt(pos);
	if (item) {
		QPoint globalPos = mapToGlobal(pos);
		contextMenu->exec(globalPos);
	}
}

