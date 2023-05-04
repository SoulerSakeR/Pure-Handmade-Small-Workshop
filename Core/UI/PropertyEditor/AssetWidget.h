#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>

class Property;

class AssetWidget : public QWidget
{
	Q_OBJECT
public:
	AssetWidget(QWidget* parent = nullptr, Property* property = nullptr);
	~AssetWidget();
public slots:
	void onButtonClicked();
private:
	QLabel* assetLabel;
	QPushButton* assetButton;
	Property* property;
};

