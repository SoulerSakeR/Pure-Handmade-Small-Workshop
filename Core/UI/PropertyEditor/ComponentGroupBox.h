#pragma once
#include <qgroupbox.h>
#include "Core/Core/Component.h"
class ComponentGroupBox : public QGroupBox
{
	Q_OBJECT
public:
	ComponentGroupBox(QWidget* parent = nullptr,Component* component = nullptr);
	~ComponentGroupBox();
private:
	Component* component;
};

