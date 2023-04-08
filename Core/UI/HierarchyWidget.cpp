#include "HierarchyWidget.h"

void HierarchyWidget::onSelectionChanged()
{
	if (selectedItems().size() <= 1)
	{
		if(selectedItems().size() == 0)
			emit gameObjectSelected(nullptr);
		else
		{
			auto item = (HierarchyItem*)selectedItems()[0];
			emit gameObjectSelected(item->gameObject);
		}		
	}
}

HierarchyWidget::HierarchyWidget(QWidget* parent):QTreeWidget(parent)
{
	connect(this, SIGNAL(itemSelectionChanged()),this, SLOT(onSelectionChanged()));
}
