#include "HierarchyItem.h"
#include "HierarchyWidget.h"

HierarchyItem::HierarchyItem(GameObject* gameObject, HierarchyWidget* tree) :gameObject(gameObject), tree(tree)
{
	setText(0, QString::fromStdString(gameObject->name));
	auto& map = tree->gameobj_item_map;
	map.emplace(gameObject, this);
	for (auto transform : gameObject->transform->children)
	{
		auto item = new HierarchyItem(transform->gameObject,tree);
		addChild(item);
	}
}

HierarchyItem::~HierarchyItem()
{
	auto children = takeChildren();
	for (auto child : children)
	{
		delete child;
	}
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
	destroyGameObject();
	auto children = takeChildren();
	for (auto child : children)
	{
		((HierarchyItem*)child)->deleteItem();
	}
}

void HierarchyItem::destroyGameObject()
{
	if (gameObject != nullptr)
		gameObject->destroy();
	gameObject = nullptr;
}

void HierarchyItem::moveVeryUP(QPoint pos)
{
	auto tree = static_cast<HierarchyWidget*>(treeWidget());
	auto targetItem = tree->itemAt(pos);
	if (targetItem == nullptr || targetItem == this) {
		// Do nothing if target item is null, self, or descendant
		return;
	}
}

void HierarchyItem::moveVeryDown(QPoint pos)
{
	auto tree = static_cast<HierarchyWidget*>(treeWidget());
	auto targetItem = tree->itemAt(pos);
	if (targetItem == nullptr || targetItem == this) {
		// Do nothing if target item is null, self, or descendant
		return;
	}
}