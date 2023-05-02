#include "AnimationComboBox.h"
#include "Core/Core/SpineAnimator.h"

AnimationComboBox::AnimationComboBox(SpineAnimator* animator, QWidget* parent) :QComboBox(parent), animator(animator)
{
	if(animator != nullptr)
		updateItems();
}

void AnimationComboBox::updateItems()
{
	clear();
	for (const auto& animationName : animator->getAllAnimations())
	{
		addItem(QString::fromStdString(animationName));
	}
}

void AnimationComboBox::setCurrentIndexNoSignal(int index)
{
	blockSignals(true);
	updateItems();
	setCurrentIndex(index);
	blockSignals(false);
}
