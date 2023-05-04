#include "AssetWidget.h"
#include <QHBoxLayout>

#include "Core/Core/Property.h"
#include "Core/ResourceManagement/ResourceMgr.h"
#include "AssetSelectorDialog.h"

using namespace std;

AssetWidget::AssetWidget(QWidget* parent, Property* property) :QWidget(parent), property(property)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	assetLineEdit = new QLineEdit(this);
	assetLineEdit->setText(property->get_data<string>().c_str());
	initWordList();
	QCompleter* completer = new QCompleter(word_list, this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setFilterMode(Qt::MatchContains);
	assetLineEdit->setCompleter(completer);
	layout->addWidget(assetLineEdit);
	connect(assetLineEdit, &QLineEdit::editingFinished, [=]()
	{
		string name = assetLineEdit->text().toStdString();

		// reset to None
		if (name == "None" || name == "") 
		{
			property->set_data<string>(name);
			return;
		}
		else if (ResourceMgr::get_instance().isExist(property->asset_type, name))
		{
			property->set_data<string>(name);
			return;
		}
		else
		{
			property->set_data<string>(name);
			QSignalBlocker blocker(assetLineEdit);
			assetLineEdit->setText(QString::fromStdString(name+" (Not Found) "));
		}
	});
	/*
	string assetName = property->get_data<string>();
	if (assetName == "None" || assetName =="")
		assetLabel = new QLabel("None");
	else 
	{
		if (ResourceMgr::get_instance().isExist(property->asset_type,assetName))
			assetLabel = new QLabel(QString::fromStdString(assetName));
		else
			assetLabel = new QLabel(QString::fromStdString(assetName).append("(not found)"));
	}
	layout->addWidget(assetLabel);
	*/
	assetButton = new QPushButton("...");
	assetButton->setFixedWidth(20);
	layout->addWidget(assetButton);
	connect(assetButton, &QPushButton::clicked, this, &AssetWidget::onButtonClicked);
}

AssetWidget::~AssetWidget()
{
	delete assetLabel;
	delete assetButton;
	delete assetLineEdit;
}

void AssetWidget::initWordList()
{
	word_list << "None";
	for (auto& pair : ResourceMgr::get_instance().assets[property->asset_type])
	{
		auto asset = pair.second;
		word_list << asset->get_name().c_str();
	}
}

void AssetWidget::onButtonClicked()
{
	AssetSelectorDialog dialog(property->asset_type,this);
	if (dialog.exec() == QDialog::Accepted)
	{
		auto assetName = dialog.getSelectedAssetName();
		property->set_data<std::string>(assetName);
		QSignalBlocker blocker(assetLineEdit);
		assetLineEdit->setText(QString::fromStdString(assetName));
	}
}
