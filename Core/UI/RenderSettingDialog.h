#pragma once
#include <qdialog.h>
#include <qevent.h>

class RenderSettingDialog : public QDialog
{
	Q_OBJECT
public:
	RenderSettingDialog(QWidget* parent = nullptr);

	virtual void closeEvent(QCloseEvent* event) override;
};

