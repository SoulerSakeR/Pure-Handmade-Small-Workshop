#include "ComponentGroupBox.h"
#include <qboxlayout.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include "Core/UI/ComponentsDockWidget.h"
#include "Vector2DLineEdit.h"
#include "DeleteComponentButton.h"
#include "ColorPaletteWidget.h"
#include "LayerComboBox.h"

ComponentGroupBox::ComponentGroupBox(QWidget* parent, Component* component) :QGroupBox(parent), component(component)
{
	setTitle(Component::getName(component->componentType).c_str());
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	auto layout = new QGridLayout(this);
	int row = 0;
	auto widget =(ComponentsDockWidget*)parent;
	for (auto it = component->properties.vbegin();it < component->properties.vend();++it)
	{
		auto property = *it;
		layout->addWidget(new QLabel(QString(property->get_name().c_str())),row, 0);
		switch (property->type)
		{
		case Property::INT:
		{
			auto spinbox = new QSpinBox();
			spinbox->setKeyboardTracking(false);
			spinbox->setValue(property->get_data<int>());
			layout->addWidget(spinbox, row, 1);
			widget->Object_Property_map[spinbox] = property;
			widget->property_Object_map[property] = spinbox;
			connect(spinbox, &QSpinBox::valueChanged, widget, &ComponentsDockWidget::onIntChanged);
			break;
		}
		case Property::FLOAT:
		{
			auto spinbox = new QDoubleSpinBox();
			spinbox->setKeyboardTracking(false);
			spinbox->setDecimals(7);
			spinbox->setRange(-FLT_MAX, FLT_MAX);
			spinbox->setValue(property->get_data<float>());
			layout->addWidget(spinbox, row, 1);
			widget->Object_Property_map[spinbox] = property;
			widget->property_Object_map[property] = spinbox;
			connect(spinbox, &QDoubleSpinBox::valueChanged, widget, &ComponentsDockWidget::onFloatChanged);
			break;
		}
		case Property::STRING:
		{
			auto lineEdit = new QLineEdit();
			lineEdit->setText(QString::fromStdString(property->get_data<std::string>()));
			layout->addWidget(lineEdit, row, 1);
			widget->Object_Property_map[lineEdit] = property;
			widget->property_Object_map[property] = lineEdit;
			connect(lineEdit, &QLineEdit::editingFinished, widget, &ComponentsDockWidget::onStringChanged);
			break;
		}
		case Property::BOOL:
		{
			auto checkbox = new QCheckBox();
			checkbox-> setCheckState((Qt::CheckState)((property->get_data<bool>()) ? 2 : 0));
			layout->addWidget(checkbox, row, 1);
			widget->Object_Property_map[checkbox] = property;
			widget->property_Object_map[property] = checkbox;
			connect(checkbox, &QCheckBox::stateChanged, widget, &ComponentsDockWidget::onIntChanged);
			break;
		}
		case Property::VECTOR2D:
		{
			auto lineEdit = new Vector2DLineEdit();
			lineEdit->setText(QString::fromStdString(property->get_data<Vector2D>().tostring()));
			layout->addWidget(lineEdit, row, 1);
			widget->Object_Property_map[lineEdit] = property;
			widget->property_Object_map[property] = lineEdit;
			connect(lineEdit, &Vector2DLineEdit::Vector2DChanged, widget, &ComponentsDockWidget::onVector2DChanged);
			break;
		}
		case Property::COLOR:
		{
			auto ColorPaletteWidegt = new ColorPaletteWidget();
			ColorPaletteWidegt->setColor(property->get_data<Color32>());
			layout->addWidget(ColorPaletteWidegt, row, 1);
			widget->Object_Property_map[ColorPaletteWidegt] = property;
			widget->property_Object_map[property] = ColorPaletteWidegt;
			connect(ColorPaletteWidegt, &ColorPaletteWidget::colorChanged, widget, &ComponentsDockWidget::onColorChanged);
			break;
		}
		case Property::COMBO_BOX:
		{
			auto comboBox = new LayerComboBox();
			comboBox->setCurrentIndex(property->get_data<int>());
			layout->addWidget(comboBox, row, 1);
			widget->Object_Property_map[comboBox] = property;
			widget->property_Object_map[property] = comboBox;
			connect(comboBox, &QComboBox::currentIndexChanged, widget, &ComponentsDockWidget::onIntChanged);
			break;
		}
		}
		row++;
	}
	auto deleteButton = new DeleteComponentButton(this, component);
	deleteButton->setText(QString("delete"));
	layout->addWidget(deleteButton, row, 0);	
}

ComponentGroupBox::~ComponentGroupBox()
{
	
}
