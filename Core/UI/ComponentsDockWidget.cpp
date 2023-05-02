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
#include "Core/UI/PropertyEditor/AnimationComboBox.h"
#include "Core/UI/PropertyEditor/SkinComboBox.h"
#include "qtimer.h"
#include "Core/Core/Component.h"

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
	timer = new QTimer(this);
	timer->setInterval(50);
	timer->start();
	connect(timer, &QTimer::timeout, this, &ComponentsDockWidget::onTimer);
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
	if(!is_refreshing)
		changed_properties_buffer1.push_back(property);
	else
		changed_properties_buffer2.push_back(property);
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
	layout->addWidget(new QLabel("Name"), 0, 0);
	auto lineEdit = new QLineEdit(selected_gameobject->get_name().c_str(), groupBox);
	layout->addWidget(lineEdit,0,1);
	connect(lineEdit, &QLineEdit::editingFinished, [=]()
	{
		if (auto result = selected_gameobject->set_name(lineEdit->text().toStdString());!result.result)
		{
			lineEdit->setText(selected_gameobject->get_name().c_str());
			lineEdit->selectAll();
			Debug::warningBox(this, result.message);
		}			
		else
		{
			selected_gameobject->get_name() = lineEdit->text().toStdString();
			hierarchy->refreshGameObject();
		}
	});
	layout->addWidget(new QLabel("Is Active"), 1, 0);
	auto checkBox = new QCheckBox(groupBox);
	checkBox->setCheckState((Qt::CheckState)((selected_gameobject->is_active())?2:0));
	layout->addWidget(checkBox, 1, 1);
	connect(checkBox, &QCheckBox::stateChanged, [=](int state)
	{
		selected_gameobject->set_active(state==2);
	});
	layout->addWidget(new QLabel("Tag"), 2, 0);
	auto tagLineEdit = new QLineEdit(selected_gameobject->get_tag().c_str(), groupBox);
	layout->addWidget(tagLineEdit, 2, 1);
	connect(tagLineEdit, &QLineEdit::editingFinished, [=]()
	{
		auto string = tagLineEdit->text().toStdString();
		selected_gameobject->set_tag(string);
	});
	components_widget->layout()->addWidget(groupBox);

	//setup components properties
	for (auto component : selected_gameobject->components)
	{
		component->onPropertyChanged.registerFunc(&ComponentsDockWidget::onPropertyChanged, this);
		auto groupBox = new ComponentGroupBox(this,component);
		components_widget->layout()->addWidget(groupBox);		
	}

	components_widget->layout()->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

void ComponentsDockWidget::updatePropertyData(Property* property)
{
	if (property_Object_map.find(property) != property_Object_map.end())
	{
		auto object = property_Object_map[property];
		if (property->type == Property::INT)
		{
			auto spinBox = (QSpinBox*)object;
			QSignalBlocker block(spinBox);
			spinBox->setValue(property->get_data<int>());
		}
		else if (property->type == Property::FLOAT)
		{
			auto doubleSpinBox = (QDoubleSpinBox*)object;
			QSignalBlocker block(doubleSpinBox);
			doubleSpinBox->setValue(property->get_data<float>());
		}
		else if (property->type == Property::STRING)
		{
			auto lineEdit = (QLineEdit*)object;
			QSignalBlocker block(lineEdit);
			lineEdit->setText(QString::fromStdString(property->get_data<std::string>()));
		}
		else if (property->type == Property::BOOL)
		{
			auto checkBox = (QCheckBox*)object;
			QSignalBlocker block(checkBox);
			checkBox->setCheckState((Qt::CheckState)(property->get_data<bool>() ? 2 : 0));
		}
		else if (property->type == Property::VECTOR2D)
		{
			auto lineEdit = (QLineEdit*)object;
			auto str = property->get_data<Vector2D>().tostring();
			QSignalBlocker block(lineEdit);
			lineEdit->setText(str.c_str());
		}
		else if (property->type == Property::ANIMATION_COMBOBOX)
		{
			auto comboBox = (AnimationComboBox*)object;
			comboBox->setCurrentIndexNoSignal(property->get_data<int>());
		}
		else if (property->type == Property::SKIN_COMBOBOX)
		{
			auto comboBox = (SkinComboBox*)object;
			comboBox->setCurrentIndexNoSignal(property->get_data<int>());
		}

	}
	else
	{
		Component* component = dynamic_cast<Component*>(property->get_object());
		Debug::logError() << "Property not found: " << property->get_name() << " in Component : " << component->getTypeName(component->componentType)
			<< " of GameObject " << component->gameObject->get_name() << "\n";
	}
}

void ComponentsDockWidget::onPropertyInputed(QObject*  sender, void* value)
{
	if (Object_Property_map.find(sender) != Object_Property_map.end())
	{
		auto property = Object_Property_map[sender];
		//property->get_component()->set_property(property, value);
	}
	else
		Debug::logError("Property not found: " + sender->objectName().toStdString());
}

void ComponentsDockWidget::onTimer()
{
	is_refreshing = true;
	for (auto property : changed_properties_buffer1)
	{
		if(property->is_visible)
			updatePropertyData(property);
	}
	changed_properties_buffer1.clear();
	is_refreshing = false;
	for (auto property : changed_properties_buffer2)
	{
		if (property->is_visible)
			updatePropertyData(property);
	}
	changed_properties_buffer2.clear();
}


void ComponentsDockWidget::onGameObjectSelected(GameObject* gameobj)
{
	if (selected_gameobject == gameobj)
		return;
	else if(selected_gameobject!=nullptr)
	{
		for (auto component : selected_gameobject->components)
		{
			component->onPropertyChanged.unRegisterFunc(&ComponentsDockWidget::onPropertyChanged, this);
		}
	}
	selected_gameobject = gameobj;
	refresh();
	if (gameobj == nullptr)
		return;	
}


