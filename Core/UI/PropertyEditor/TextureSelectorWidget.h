#pragma once
#include <qwidget.h>
#include <QLabel>
#include <QPushButton>
#include "Core/Core/Property.h"

class TextureSelectorWidget :	public QWidget
{
	Q_OBJECT
public:
	TextureSelectorWidget(QWidget* parent = nullptr,Property* property = nullptr);
	~TextureSelectorWidget();
public slots:
	void onButtonClicked();
private:
	QLabel* textureLabel;
	QPushButton* textureButton;
	Property* property;
};

