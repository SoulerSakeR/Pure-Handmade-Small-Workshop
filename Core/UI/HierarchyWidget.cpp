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

	//创建物体子菜�?
	QMenu *addGameobjectMenu = new QMenu("Creat GameObject", this);

	//创建空物体按�?
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

HierarchyWidget::HierarchyWidget(QWidget* parent):QTreeWidget(parent)
{
	connect(this, SIGNAL(itemSelectionChanged()),this, SLOT(onSelectionChanged()));	
}

void HierarchyWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		auto draggedItem = currentItem();
		if (draggedItem == nullptr)
			return;

		auto distance = (event->pos() - startPos).manhattanLength();
		if (distance >= QApplication::startDragDistance())
		{
			auto newIndex = indexOfTopLevelItem(draggedItem);
			auto moveDistance = event->pos() - startPos;
			if (moveDistance.y() < 0)
			{
				((HierarchyItem*)draggedItem)->moveVeryUP(event->pos());
			}
			else if (moveDistance.y() > 0)
			{
				((HierarchyItem*)draggedItem)->moveVeryDown(event->pos());
			}
			startPos = event->pos();
		}
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

