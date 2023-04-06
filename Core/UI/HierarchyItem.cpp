#include "HierarchyItem.h"

HierarchyItem::HierarchyItem(GameObject* gameObject):gameObject(gameObject)
{
	setText(0, QString::fromStdString(gameObject->name));
	for (auto transform : gameObject->transform->children)
	{
		addChild(new HierarchyItem(transform->gameObject));
	}
}
