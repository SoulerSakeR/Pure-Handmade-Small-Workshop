#include "HierarchyWidget.h"

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

HierarchyWidget::HierarchyWidget(QWidget* parent):QTreeWidget(parent)
{
	connect(this, SIGNAL(itemSelectionChanged()),this, SLOT(onSelectionChanged()));
}
