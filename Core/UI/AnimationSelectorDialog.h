#pragma once
#include <qdialog.h>

class SpineAnimationData;
class QLineEdit;
class QListWidget;

class AnimationSelectorDialog : public QDialog
{
public:
	AnimationSelectorDialog(QWidget* parent = nullptr);
	~AnimationSelectorDialog();
	std::string getSelectedSpineName() const;
private:
	void initSearchBar();
	void initListWidget();
	void initButtons();
	std::string selectedSpineName;
	QLineEdit* searchBar;
	QListWidget* listWidget;
	QPushButton* okButton;
};

