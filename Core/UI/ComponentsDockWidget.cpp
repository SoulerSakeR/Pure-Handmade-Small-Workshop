#include "ComponentsDockWidget.h"
#include "qtreewidget.h"
#include "qlayout.h"
#include "qtablewidget.h"
#include "Core/Core/Component.h"
#include "qheaderview.h"
#include "qgroupbox.h"
#include "qlineedit.h"
#include "qspinbox.h"
#include "qtoolbutton.h"
#include "Core/Core/Property.h"
#include "qcheckbox.h"
#include "qlayoutitem.h"
#include "qscrollarea.h"
#include "Core/Utils/Vector2D.h"
#include "qpushbutton.h"
#include "PropertyEditor/Vector2DLineEdit.h"
#include "PropertyEditor/ComponentGroupBox.h"
#include "qlabel.h"
#include "Core/Utils/Result.h"
#include "PropertyEditor/ColorPaletteWidget.h"

ComponentsDockWidget* ComponentsDockWidget::instance = nullptr;

void clearLayout(QLayout* layout)
{
	while (QLayoutItem* item = layout->takeAt(0))
	{
		if (QWidget* widget = item->widget())
			widget->deleteLater();

		if (QLayout* childLayout = item->layout())
			clearLayout(childLayout);

		if (QSpacerItem* spaerItem = item->spacerItem())
			layout->removeItem(spaerItem);

		delete item;
	}
}

ComponentsDockWidget::ComponentsDockWidget(QWidget* parent) :QDockWidget(parent)
{
	instance = this;
	selected_gameobject = nullptr;
	hierarchy = nullptr;
}

void ComponentsDockWidget::set_components_widget(QWidget* widget)
{
	components_widget = widget;
	auto verticalLayout = new QVBoxLayout(components_widget);
	verticalLayout->setSpacing(20);
	verticalLayout->setObjectName("verticalLayout");
	verticalLayout->setContentsMargins(5, 5, 5, 5);
}

void ComponentsDockWidget::clear()
{
	clearLayout(components_widget->layout());
	property_Object_map.clear();
	Object_Property_map.clear();
}

ComponentsDockWidget* ComponentsDockWidget::get_instance()
{
	return instance;
}

GameObject* ComponentsDockWidget::get_selected_gameobject()
{
	return selected_gameobject;
}

void ComponentsDockWidget::onPropertyChanged(Property* property)
{
	if (property_Object_map.find(property) != property_Object_map.end())
	{
		auto object = property_Object_map[property];
		if (property->type == Property::INT)
		{
			auto spinBox = (QSpinBox*)object;
			spinBox->setValue(property->get_data<int>());
		}
		else if (property->type == Property::FLOAT)
		{
			auto doubleSpinBox = (QDoubleSpinBox*)object;
			doubleSpinBox->setValue( property->get_data<float>());
		}
		else if (property->type == Property::STRING)
		{
			auto lineEdit = (QLineEdit*)object;
			lineEdit->setText(QString::fromStdString(property->get_data<std::string>()));
		}
		else if (property->type == Property::BOOL)
		{
			auto checkBox = (QCheckBox*)object;
			checkBox->setCheckState((Qt::CheckState)(property->get_data<bool>()));
		}
		else if (property->type == Property::VECTOR2D)
		{
			auto lineEdit = (QLineEdit*)object;
			lineEdit->setText(QString::fromStdString(property->get_data<Vector2D>().tostring()));
		}
	}
	else
		Debug::logError("Property not found: "+ property->get_name());
}

void ComponentsDockWidget::refresh()
{
	clear();
	if (selected_gameobject == nullptr)
		return;

	//setup gameobject properties
	auto groupBox = new QGroupBox("GameObject", components_widget);
	groupBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	
	auto layout = new QGridLayout(groupBox);
	layout->addWidget(new QLabel("name"), 0, 0);
	auto lineEdit = new QLineEdit(selected_gameobject->name.c_str(),groupBox);
	layout->addWidget(lineEdit,0,1);
	connect(lineEdit, &QLineEdit::editingFinished, [=]()
	{
		if (auto result = selected_gameobject->set_name(lineEdit->text().toStdString());!result.result)
		{
			lineEdit->setText(selected_gameobject->name.c_str());
			lineEdit->selectAll();
			Debug::warningBox(this, result.message);
		}			
		else
		{
			selected_gameobject->name = lineEdit->text().toStdString();
			hierarchy->refreshGameObject();
		}
	});
	layout->addWidget(new QLabel("is active"), 1, 0);
	auto checkBox = new QCheckBox(groupBox);
	checkBox->setCheckState((Qt::CheckState)((selected_gameobject->isActive)?2:0));
	layout->addWidget(checkBox, 1, 1);
	connect(checkBox, &QCheckBox::stateChanged, [=](int state)
	{
		selected_gameobject->isActive = (bool)state;
	});
	components_widget->layout()->addWidget(groupBox);

	//setup components properties
	for (auto component : selected_gameobject->components)
	{
		component->onPropertyChange.registerFunc(&ComponentsDockWidget::onPropertyChanged, this);
		auto groupBox = new ComponentGroupBox(this,component);
		components_widget->layout()->addWidget(groupBox);		
	}

	components_widget->layout()->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void ComponentsDockWidget::onPropertyInputed(QObject*  sender, void* value)
{
	if (Object_Property_map.find(sender) != Object_Property_map.end())
	{
		auto property = Object_Property_map[sender];
		property->get_component()->set_property(property, value);
	}
	else
		Debug::logError("Property not found: " + sender->objectName().toStdString());
}

void ComponentsDockWidget::onIntChanged(int value)
{
	auto sender = QObject::sender();
	onPropertyInputed(sender, &value);
}

void ComponentsDockWidget::onFloatChanged(double value)
{
	auto sender = QObject::sender();
	float f = (float)value;
	onPropertyInputed(sender, &f);
}

void ComponentsDockWidget::onStringChanged()
{
	auto sender = QObject::sender();
	auto value = (QLineEdit*)sender;
	auto text = value->text();
	auto str = text.toStdString();
	onPropertyInputed(sender, &str);
}

void ComponentsDockWidget::onBoolChanged(bool value)
{
	auto sender = QObject::sender();
	onPropertyInputed(sender, &value);
}

void ComponentsDockWidget::onVector2DChanged(Vector2D vec)
{
	auto sender = QObject::sender();
	onPropertyInputed(sender, &vec);
}

void ComponentsDockWidget::onColorChanged(Color32 value)
{
	auto sender = QObject::sender();
	onPropertyInputed(sender, &value);
}

void ComponentsDockWidget::onGameObjectSelected(GameObject* gameobj)
{
	if (selected_gameobject == gameobj)
		return;
	else if(selected_gameobject!=nullptr)
	{
		for (auto component : selected_gameobject->components)
		{
			component->onPropertyChange.unRegisterFunc(&ComponentsDockWidget::onPropertyChanged, this);
		}
	}
	selected_gameobject = gameobj;
	refresh();
	if (gameobj == nullptr)
		return;
	
}


