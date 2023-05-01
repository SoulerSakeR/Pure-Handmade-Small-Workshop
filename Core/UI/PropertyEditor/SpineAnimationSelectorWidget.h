#pragma once
#include <qwidget.h>

class Property;
class QLabel;
class QPushButton;

class SpineAnimationSelectorWidget : public QWidget
{
	Q_OBJECT
public:
	SpineAnimationSelectorWidget(QWidget* parent = nullptr, Property* property = nullptr);
	~SpineAnimationSelectorWidget();
public slots:
	void onButtonClicked();
private:
	QLabel* spineAnimationLabel;
	QPushButton* spineButton;
	Property* property;
};

