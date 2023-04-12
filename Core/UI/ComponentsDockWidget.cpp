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
#include "DeleteComponentButton.h"

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
}

void ComponentsDockWidget::set_components_widget(QWidget* widget)
{
	components_widget = widget;
	auto verticalLayout = new QVBoxLayout(components_widget);
	verticalLayout->setSpacing(20);
	verticalLayout->setObjectName("verticalLayout");
	verticalLayout->setContentsMargins(0, 0, 0, 0);
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
		changed_property = property;
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
			checkBox->setChecked(property->get_data<bool>());
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
	for (auto component : selected_gameobject->components)
	{
		auto groupBox = new QGroupBox(QString::fromStdString(component->getName(component->componentType)), components_widget);
		groupBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
		auto verticalLayout = new QVBoxLayout(groupBox);
		verticalLayout->setSpacing(20);
		verticalLayout->setObjectName("verticalLayout");
		verticalLayout->setContentsMargins(0, 0, 0, 0);

		auto componentWidget = new QTableWidget(groupBox);
		groupBox->layout()->addWidget(componentWidget);
		componentWidget->verticalHeader()->setVisible(false);
		componentWidget->setColumnCount(2);
		componentWidget->horizontalHeader()->setStretchLastSection(true);
		componentWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
		componentWidget->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
		componentWidget->setHorizontalHeaderLabels(QStringList() << "Property" << "Value");
		for (auto& pair : component->properties)
		{
			auto property = pair.second;
			componentWidget->setRowCount(componentWidget->rowCount() + 1);
			componentWidget->setItem(componentWidget->rowCount() - 1, 0, new QTableWidgetItem(QString::fromStdString(property->get_name())));
			switch (property->type)
			{
			case Property::INT:
			{
				auto spinbox = new QSpinBox();
				spinbox->setKeyboardTracking(false);
				spinbox->setValue(property->get_data<int>());
				componentWidget->setCellWidget(componentWidget->rowCount() - 1, 1, spinbox);
				Object_Property_map[spinbox] = property;
				property_Object_map[property] = spinbox;
				connect(spinbox, &QSpinBox::valueChanged, this, &ComponentsDockWidget::onIntChanged);
				break;
			}
			case Property::FLOAT:
			{
				auto spinbox = new QDoubleSpinBox();
				spinbox->setKeyboardTracking(false);
				spinbox->setDecimals(7);
				spinbox->setRange(-FLT_MAX, FLT_MAX);
				spinbox->setValue(property->get_data<float>());
				componentWidget->setCellWidget(componentWidget->rowCount() - 1, 1, spinbox);
				Object_Property_map[spinbox] = property;
				property_Object_map[property] = spinbox;
				connect(spinbox, &QDoubleSpinBox::valueChanged, this, &ComponentsDockWidget::onFloatChanged);
				break;
			}
			case Property::STRING:
			{
				auto lineEdit = new QLineEdit();
				lineEdit->setText(QString::fromStdString(property->get_data<std::string>()));
				componentWidget->setCellWidget(componentWidget->rowCount() - 1, 1, lineEdit);
				Object_Property_map[lineEdit] = property;
				property_Object_map[property] = lineEdit;
				connect(lineEdit, &QLineEdit::editingFinished, this, &ComponentsDockWidget::onStringChanged);
				break;
			}
			case Property::BOOL:
			{
				auto checkbox = new QCheckBox();
				checkbox->setChecked(property->get_data<bool>());
				componentWidget->setCellWidget(componentWidget->rowCount() - 1, 1, checkbox);
				Object_Property_map[checkbox] = property;
				property_Object_map[property] = checkbox;
				connect(checkbox, &QCheckBox::stateChanged, this, &ComponentsDockWidget::onIntChanged);
				break;
			}
			case Property::VECTOR2D:
			{
				auto lineEdit = new QLineEdit();
				lineEdit->setText(QString::fromStdString(property->get_data<Vector2D>().tostring()));
				componentWidget->setCellWidget(componentWidget->rowCount() - 1, 1, lineEdit);
				Object_Property_map[lineEdit] = property;
				property_Object_map[property] = lineEdit;
				connect(lineEdit, &QLineEdit::editingFinished, this, &ComponentsDockWidget::onStringChanged);
				break;
			}
			}
		}
		auto deleteButton = new DeleteComponentButton(groupBox,component);
		deleteButton->setText(QString("delete "));
		groupBox->layout()->addWidget(deleteButton);
		components_widget->layout()->addWidget(groupBox);
	}
	components_widget->layout()->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void ComponentsDockWidget::onPropertyInputed(QObject*  sender, void* value)
{
	if (Object_Property_map.find(sender) != Object_Property_map.end())
	{
		auto property = Object_Property_map[sender];
		if (changed_property == property)
		{
			changed_property = nullptr;
			return;
		}
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

void ComponentsDockWidget::onVector2DChanged(QString value)
{
	auto sender = QObject::sender();
	onPropertyInputed(sender, &value);
}

void ComponentsDockWidget::onGameObjectSelected(GameObject* gameobj)
{
	selected_gameobject = gameobj;
	if (gameobj == nullptr)
		return;
	refresh();
}


