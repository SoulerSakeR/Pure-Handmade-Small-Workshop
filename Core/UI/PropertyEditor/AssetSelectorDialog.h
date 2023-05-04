#pragma once
#include <qdialog.h>
#include "Core/ResourceManagement/PHAsset.h"

class QLineEdit;
class QListWidget;
class QPushButton;

class AssetSelectorDialog : public QDialog
{
	Q_OBJECT
public:
	AssetSelectorDialog(PHAsset::AssetType assetType,QWidget* parent = nullptr);
	~AssetSelectorDialog();
	std::string getSelectedAssetName() const;
private:
	void initSearchBar();
	void initListWidget();
	void initButtons();
	std::string selectedAssetName;
	QLineEdit* searchBar;
	QListWidget* listWidget;
	QPushButton* okButton;
	PHAsset::AssetType assetType;
};

