#include "HierarchyWidget.h"
#include "qmenu.h"

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
	initContextMenu();
	connect(this, SIGNAL(itemSelectionChanged()),this, SLOT(onSelectionChanged()));	
}

void HierarchyWidget::showContextMenu(const QPoint& pos)
{
	auto item = itemAt(pos);
	if (item) {
		QPoint globalPos = mapToGlobal(pos);
		contextMenu->exec(globalPos);
	}
}
