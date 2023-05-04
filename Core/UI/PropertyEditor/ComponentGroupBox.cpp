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
#include "TextureSelectorWidget.h"
#include "Core/ResourceManagement/SceneMgr.h"
#include "AnimationComboBox.h"
#include "SpineAnimationSelectorWidget.h"
#include "SkinComboBox.h"
#include "ScriptNameLineEdit.h"
#include "AssetWidget.h"

ComponentGroupBox::ComponentGroupBox(QWidget* parent, Component* component) :QGroupBox(parent), component(component)
{
	setTitle(Component::getTypeName(component->componentType).c_str());
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	auto layout = new QGridLayout(this);
	int row = 0;
	auto widget =(ComponentsDockWidget*)parent;

	// create property widgets
	for (auto it = component->properties.vbegin();it < component->properties.vend();++it)
	{
		auto property = *it;
		if (!property->is_visible)
			continue;
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
			spinbox->setReadOnly(!property->is_editable);
			connect(spinbox, &QSpinBox::valueChanged, widget, [=](int value) {property->set_data<int>(value);});
			break;
		}
		case Property::FLOAT:
		{
			auto spinbox = new QDoubleSpinBox();
			spinbox->setKeyboardTracking(false);
			spinbox->setDecimals(7);
			spinbox->setRange(-FLT_MAX, FLT_MAX);
			spinbox->setValue(property->get_data<float>());
			spinbox->setReadOnly(!property->is_editable);
			layout->addWidget(spinbox, row, 1);
			widget->Object_Property_map[spinbox] = property;
			widget->property_Object_map[property] = spinbox;
			connect(spinbox, &QDoubleSpinBox::valueChanged, widget, [=](double value) {
				property->set_data<float>((float)value);
			});
			break;
		}
		case Property::STRING:
		{
			auto lineEdit = new QLineEdit();
			lineEdit->setText(QString::fromStdString(property->get_data<std::string>()));
			lineEdit->setDisabled( !property->is_editable);
			layout->addWidget(lineEdit, row, 1);
			widget->Object_Property_map[lineEdit] = property;
			widget->property_Object_map[property] = lineEdit;
			connect(lineEdit, &QLineEdit::editingFinished, [=]() {
				auto str = lineEdit->text().toStdString();
				property->set_data<std::string>(str);
			});
			break;
		}
		case Property::BOOL:
		{
			auto checkbox = new QCheckBox();
			checkbox-> setCheckState((Qt::CheckState)((property->get_data<bool>()) ? 2 : 0));
			checkbox->setDisabled(!property->is_editable);
			layout->addWidget(checkbox, row, 1);
			widget->Object_Property_map[checkbox] = property;
			widget->property_Object_map[property] = checkbox;
			connect(checkbox, &QCheckBox::stateChanged, [=](int value) {
				property->set_data<bool>(value == 2);
			});
			break;
		}
		case Property::VECTOR2D:
		{
			auto lineEdit = new Vector2DLineEdit();
			lineEdit->setText(QString::fromStdString(property->get_data<Vector2D>().tostring()));
			lineEdit->setDisabled(!property->is_editable);
			layout->addWidget(lineEdit, row, 1);
			widget->Object_Property_map[lineEdit] = property;
			widget->property_Object_map[property] = lineEdit;
			connect(lineEdit, &Vector2DLineEdit::Vector2DChanged, [=](Vector2D value) {
				property->set_data<Vector2D>(value);
			});
			break;
		}
		case Property::COLOR:
		{
			auto ColorPaletteWidegt = new ColorPaletteWidget();
			ColorPaletteWidegt->setColor(property->get_data<Color32>());
			layout->addWidget(ColorPaletteWidegt, row, 1);
			widget->Object_Property_map[ColorPaletteWidegt] = property;
			widget->property_Object_map[property] = ColorPaletteWidegt;
			connect(ColorPaletteWidegt, &ColorPaletteWidget::colorChanged, [=](Color32 color) {
				property->set_data<Color32>(color);
			});
			break;
		}
		case Property::COMBO_BOX:
		{
			auto comboBox = new LayerComboBox();
			int order = property->get_data<int>();
			int index = 0;
			for (auto& pair : SceneMgr::get_instance().get_render_setting()->get_render_layers())
			{
				if (pair.first == order)
				{
					comboBox->setCurrentIndex(index);
				}
				index++;
			}
			layout->addWidget(comboBox, row, 1);
			widget->Object_Property_map[comboBox] = property;
			widget->property_Object_map[property] = comboBox;
			comboBox->setDisabled( !property->is_editable);
			connect(comboBox, &QComboBox::currentIndexChanged, [=](int index) {
				property->set_data<int>(index);
			});
			break;
		}
		case Property::TEXTURE2D:
		{
			auto texture2D = new TextureSelectorWidget(this,property);
			layout->addWidget(texture2D, row, 1);
			widget->Object_Property_map[texture2D] = property;
			widget->property_Object_map[property] = texture2D;
			break;
		}
		case Property::ANIMATION_COMBOBOX:
		{
			auto comboBox = new AnimationComboBox(dynamic_cast<SpineAnimator*>(component), this);
			comboBox->setCurrentIndex(property->get_data<int>());
			comboBox->setDisabled(!property->is_editable);
			layout->addWidget(comboBox, row, 1);
			widget->Object_Property_map[comboBox] = property;
			widget->property_Object_map[property] = comboBox;
			connect(comboBox, &QComboBox::currentIndexChanged, [=](int index) {
				property->set_data<int>(index);
			});
			break;
		}
		case Property::ANIMATION:
		{
			auto spine = new SpineAnimationSelectorWidget(this, property);
			layout->addWidget(spine, row, 1);
			widget->Object_Property_map[spine] = property;
			widget->property_Object_map[property] = spine;
			break;
		}
		case Property::SKIN_COMBOBOX:
		{
			auto comboBox = new SkinComboBox(dynamic_cast<SpineAnimator*>(component), this);
			comboBox->setCurrentIndex(property->get_data<int>());
			comboBox->setDisabled(!property->is_editable);
			layout->addWidget(comboBox, row, 1);
			widget->Object_Property_map[comboBox] = property;
			widget->property_Object_map[property] = comboBox;
			connect(comboBox, &QComboBox::currentIndexChanged, [=](int index) {
				property->set_data<int>(index);
			});
			break;
		}
		case Property::SCRIPT_LINEEDIT:
		{
			auto lineEdit = new ScriptNameLineEdit(this,property);
			layout->addWidget(lineEdit, row, 1);
			widget->Object_Property_map[lineEdit] = property;
			widget->property_Object_map[property] = lineEdit;
			break;
		}
		case Property::ASSET:
		{
			auto assetWidget = new AssetWidget(this, property);
			layout->addWidget(assetWidget, row, 1);
			widget->Object_Property_map[assetWidget] = property;
			widget->property_Object_map[property] = assetWidget;
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
