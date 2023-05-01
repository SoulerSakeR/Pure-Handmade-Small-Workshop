#include "AnimationSelectorDialog.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>

#include "Core/ResourceManagement/ResourceMgr.h"


AnimationSelectorDialog::AnimationSelectorDialog(QWidget* parent)
{
	searchBar = nullptr;
	listWidget = nullptr;
	okButton = nullptr;
	selectedSpineName = "";
	setMinimumSize(300, 400);
	QVBoxLayout* layout = new QVBoxLayout(this);
	initSearchBar();
	initListWidget();
	initButtons();
}

AnimationSelectorDialog::~AnimationSelectorDialog()
{
	delete searchBar;
	delete listWidget;
	delete okButton;
}

std::string AnimationSelectorDialog::getSelectedSpineName() const
{
	return selectedSpineName;
}

void AnimationSelectorDialog::initSearchBar()
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

void AnimationSelectorDialog::initListWidget()
{
	listWidget = new QListWidget(this);
	listWidget->setViewMode(QListView::IconMode);
	listWidget->setIconSize(QSize(100, 100));
	listWidget->setResizeMode(QListView::Adjust);
	listWidget->setGridSize(QSize(120, 120));
	listWidget->setSpacing(10);
	listWidget->setStyleSheet("QListWidget{background-color: rgb(255, 255, 255);border: 1px solid gray;border-radius: 5px;padding: 0 8px;}");
	listWidget->addItem(new QListWidgetItem("None"));
	for (auto& spine : ResourceMgr::get_instance().spine_assets)
	{
		listWidget->addItem(new QListWidgetItem(QIcon(QString::fromStdString(spine.second->getTexturePath())), QString::fromStdString(spine.second->get_name())));
	}
	layout()->addWidget(listWidget);
}

void AnimationSelectorDialog::initButtons()
{
	okButton = new QPushButton("OK", this);
	okButton->setAutoDefault(false);
	okButton->setFixedHeight(30);
	okButton->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border: 1px solid gray;border-radius: 5px;padding: 0 8px;}");
	layout()->addWidget(okButton);
	connect(okButton, &QPushButton::clicked, [this]() {
		if (auto item = listWidget->currentItem();item != nullptr)
		{
			selectedSpineName = item->text().toStdString();
			accept();
		}
		else
		{
			reject();
		}
	});
}
