#pragma once
#include <qdialog.h>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

class TextureSelectorDialog : public QDialog
{
	Q_OBJECT
public: 
	TextureSelectorDialog(QWidget* parent = nullptr);
	~TextureSelectorDialog();
	std::string getSelectedTextureName() const;
private:
	void initSearchBar();
	void initListWidget();
	void initButtons();
	std::string selectedTextureName;
	QLineEdit* searchBar;
	QListWidget* listWidget;
	QPushButton* okButton;
};

