#include "DeleteComponentButton.h"
#include "Core/Core/GameObject.h"
#include "ComponentsDockWidget.h"

DeleteComponentButton::DeleteComponentButton(QWidget* parent,Component* component): QPushButton(parent),component(component)
{
	setText("Delete");
	connect(this, &QPushButton::clicked, this, &DeleteComponentButton::deleteComponent);
}

void DeleteComponentButton::deleteComponent()
{
	auto gameobject = component->gameObject;
	gameobject->removeComponent(component);
	ComponentsDockWidget::get_instance()->refresh();
}
