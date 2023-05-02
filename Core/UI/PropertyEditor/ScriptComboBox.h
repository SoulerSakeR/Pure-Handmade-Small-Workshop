#pragma once
#include "AnimationComboBox.h"

class Property;

class ScriptComboBox : public AnimationComboBox
{
public:
	ScriptComboBox(QWidget* parent, Property* property);
	virtual void updateItems() override;

private:
	Property* property;
	std::string text;
};

