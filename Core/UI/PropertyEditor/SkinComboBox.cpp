#include "SkinComboBox.h"
#include "Core/Core/SpineAnimator.h"

SkinComboBox::SkinComboBox(SpineAnimator* animator, QWidget* parent) :AnimationComboBox(animator, parent)
{
	updateItems();
}

void SkinComboBox::updateItems()
{
	clear();
	for (const auto& skinName : animator->getAllSkins())
	{
		addItem(QString::fromStdString(skinName));
	}
}
