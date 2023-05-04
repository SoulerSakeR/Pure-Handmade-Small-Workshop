#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>

class Property;
class QLineEdit;

class AssetWidget : public QWidget
{
	Q_OBJECT
public:
	AssetWidget(QWidget* parent = nullptr, Property* property = nullptr);
	~AssetWidget();

	void initWordList();
public slots:
	void onButtonClicked();
private:
	QLabel* assetLabel = nullptr;
	QLineEdit* assetLineEdit= nullptr;
	QPushButton* assetButton = nullptr;
	Property* property = nullptr;
	QStringList word_list;
};

