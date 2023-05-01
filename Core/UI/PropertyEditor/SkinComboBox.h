#pragma once
#include "AnimationComboBox.h"
class SkinComboBox : public AnimationComboBox
{
public:
	SkinComboBox(SpineAnimator* animator, QWidget* parent = nullptr);
	virtual void updateItems() override;
};

