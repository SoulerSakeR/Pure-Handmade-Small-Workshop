#include "AssetSelectorDialog.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>

#include "Core/ResourceManagement/ResourceMgr.h"


AssetSelectorDialog::AssetSelectorDialog(PHAsset::AssetType assetType, QWidget* parent):QDialog(parent),assetType(assetType)
{
	searchBar = nullptr;
	listWidget = nullptr;
	okButton = nullptr;
	selectedAssetName = "";
	setMinimumSize(300, 400);
	QVBoxLayout* layout = new QVBoxLayout(this);
	initSearchBar();
	initListWidget();
	initButtons();
}

AssetSelectorDialog::~AssetSelectorDialog()
{
	delete searchBar;
	delete listWidget;
	delete okButton;
}

std::string AssetSelectorDialog::getSelectedAssetName() const
{
	return selectedAssetName;
}

void AssetSelectorDialog::initSearchBar()
{
	searchBar = new QLineEdit(this);
	searchBar->setPlaceholderText("Search...");
	searchBar->setClearButtonEnabled(true);
	searchBar->setFixedHeight(30);
	searchBar->setStyleSheet("QLineEdit{border: 1px solid gray;border-radius: 5px;padding: 0 8px;}");
	layout()->addWidget(searchBar);
	connect(searchBar, &QLineEdit::editingFinished, [this]()
	{
		// hide all items
		for (int i = 0; i < listWidget->count(); ++i) {
			QListWidgetItem* item = listWidget->item(i);
			item->setHidden(true);
		}

		// show only items that match the text
		auto filter = searchBar->text();
		for (auto item : listWidget->findItems(filter, Qt::MatchFlag::MatchContains))
		{
			item->setHidden(false);
		}
	});
}

void AssetSelectorDialog::initListWidget()
{
	listWidget = new QListWidget(this);
	listWidget->setViewMode(QListView::IconMode);
	listWidget->setIconSize(QSize(100, 100));
	listWidget->setResizeMode(QListView::Adjust);
	listWidget->setGridSize(QSize(120, 120));
	listWidget->setSpacing(10);
	listWidget->setStyleSheet("QListWidget{background-color: rgb(255, 255, 255);border: 1px solid gray;border-radius: 5px;padding: 0 8px;}");
	listWidget->addItem(new QListWidgetItem("None"));
	for (auto& assets : ResourceMgr::get_instance().assets[assetType])
	{
		auto assetData = static_cast<PHAsset*>(assets.second);
		QFileIconProvider iconProvider;
		QFileInfo fileInfo(assetData->get_path().c_str());
		QString iconPath = iconProvider.icon(fileInfo).name();
		listWidget->addItem(new QListWidgetItem(QIcon(iconPath), QString::fromStdString(assetData->get_name())));
	}
	layout()->addWidget(listWidget);
}

void AssetSelectorDialog::initButtons()
{
	okButton = new QPushButton("OK", this);
	okButton->setAutoDefault(false);
	okButton->setFixedHeight(30);
	okButton->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border: 1px solid gray;border-radius: 5px;padding: 0 8px;}");
	layout()->addWidget(okButton);
	connect(okButton, &QPushButton::clicked, [this]() {
		if (auto item = listWidget->currentItem();item != nullptr)
		{
			selectedAssetName = item->text().toStdString();
			accept();
		}
		else
		{
			reject();
		}
	});
}
