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
	assetButton = new QPushButton("...");
	assetButton->setFixedWidth(20);
	layout->addWidget(assetButton);
	connect(assetButton, &QPushButton::clicked, this, &AssetWidget::onButtonClicked);
}

AssetWidget::~AssetWidget()
{
	delete assetLabel;
	delete assetButton;
}

void AssetWidget::onButtonClicked()
{
	AssetSelectorDialog dialog(property->asset_type,this);
	if (dialog.exec() == QDialog::Accepted)
	{
		auto assetName = dialog.getSelectedAssetName();
		property->set_data<std::string>(assetName);
		assetLabel->setText(QString::fromStdString(assetName));
	}
}
