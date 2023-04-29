#pragma once
#include <qcombobox.h>

class SpineAnimator;

class AnimationComboBox : public QComboBox
{
	Q_OBJECT
public:
	AnimationComboBox(SpineAnimator* animator, QWidget* parent = nullptr);

	void updateItems();

public slots:
	void setCurrentIndexNoSignal(int index);

private:
	SpineAnimator* animator;
};

