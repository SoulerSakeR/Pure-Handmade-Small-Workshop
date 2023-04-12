#include "HierarchyItem.h"
#include "HierarchyWidget.h"

HierarchyItem::HierarchyItem(GameObject* gameObject):gameObject(gameObject)
{
	setText(0, QString::fromStdString(gameObject->name));
	for (auto transform : gameObject->transform->children)
	{
		addChild(new HierarchyItem(transform->gameObject));
	}
}

HierarchyItem::~HierarchyItem()
{
	auto children = takeChildren();
	for (auto child : children)
	{
		delete child;
	}
	if(gameObject!=nullptr)
		gameObject->destroy();
	gameObject = nullptr;
}

void HierarchyItem::moveUp()
{
	if(parent()==0)
	{
		auto tree =(HierarchyWidget*) treeWidget();		
		auto index = tree->indexOfTopLevelItem(this);
		if (index <= 0)
			return;
		tree->takeTopLevelItem(index);
		tree->insertTopLevelItem(index-1, this);
		tree->setCurrentItem(this);
	}
	else
	{
		auto parentItem = parent();
		auto index = parentItem->indexOfChild(this);
		if (index > 0)
		{
			parentItem->takeChild(index);
			parentItem->insertChild(index - 1, this);
			auto parentTransform = gameObject->transform->parent;
			parentTransform->children.erase(parentTransform->children.begin() + index);
			parentTransform->children.insert(parentTransform->children.begin() + index - 1, gameObject->transform);
			treeWidget()->setCurrentItem(this);
		}
	}
}

void HierarchyItem::moveDown()
{
	if (parent() == 0)
	{
		auto tree = (HierarchyWidget*)treeWidget();
		auto index = tree->indexOfTopLevelItem(this);
		if (index >= tree->topLevelItemCount() - 1)
			return;
		tree->takeTopLevelItem(index);
		tree->insertTopLevelItem(index + 1, this);
		tree->setCurrentItem(this);
	}
	else
	{
		auto parentItem = parent();
		auto index = parentItem->indexOfChild(this);
		if (index < parentItem->childCount() - 1)
		{
			parentItem->takeChild(index);
			parentItem->insertChild(index + 1, this);
			auto parentTransform = gameObject->transform->parent;
			parentTransform->children.erase(parentTransform->children.begin() + index);
			parentTransform->children.insert(parentTransform->children.begin() + index + 1, gameObject->transform);
			treeWidget()->setCurrentItem(this);
		}
	}
}

void HierarchyItem::deleteItem()
{
	if (parent() == 0)
	{
		auto tree = (HierarchyWidget*)treeWidget();
		auto index = tree->indexOfTopLevelItem(this);
		tree->takeTopLevelItem(index);
	}
	else
	{
		auto parentItem = parent();
		auto index = parentItem->indexOfChild(this);
		parentItem->takeChild(index);
	}
	delete this;
}
