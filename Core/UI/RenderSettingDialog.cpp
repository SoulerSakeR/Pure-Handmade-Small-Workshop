#include "RenderSettingDialog.h"
#include <QGroupbox>
#include <QVBoxLayout>
#include <qlabel.h>
#include <QGridLayout>
#include "Core/ResourceManagement/SceneMgr.h"
#include <QPushButton>

RenderSettingDialog::RenderSettingDialog(QWidget* parent):QDialog(parent)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	QGroupBox* groupbox = new QGroupBox("Render Layer",this);
	layout->addWidget(groupbox);
	QGridLayout* gridLayout = new QGridLayout(groupbox);
	gridLayout->addWidget(new QLabel("Layer Name"), 0, 0);
	gridLayout->addWidget(new QLabel("Layer Order"), 0, 1);
	int row = 1;
	for (const auto& layer : SceneMgr::get_instance().get_render_setting()->get_render_layers())
	{
		if (layer.second == "Default")
		{
			gridLayout->addWidget(new QLabel(QString::fromStdString(layer.second)), row, 0);
			gridLayout->addWidget(new QLabel(QString::number(layer.first)), row, 1);
			auto addButton = new QPushButton("Add");
			gridLayout->addWidget(addButton,row,2);
			connect(addButton, &QPushButton::clicked, [=]()
			{
				int row = gridLayout->rowCount();
				auto lineEdit = new QLineEdit("New Layer");
				gridLayout->addWidget(lineEdit, row, 0);
				auto lineEdit2 = new QLineEdit("0");
				gridLayout->addWidget(lineEdit2, row, 1);
				QPushButton* saveButton = new QPushButton("Save");
				gridLayout->addWidget(saveButton, row, 2);
				connect(saveButton, &QPushButton::clicked, [=]()
				{
					if (!SceneMgr::get_instance().get_render_setting()->addRenderLayer(lineEdit2->text().toInt(), lineEdit->text().toStdString()))
					{
						Debug::warningBox(this, "Layer Order has been used!");
						return;
					}
					ComponentsDockWidget::get_instance()->refresh();
					gridLayout->removeWidget(saveButton);
					saveButton->deleteLater();
					QPushButton* deleteButton = new QPushButton("Delete");
					gridLayout->addWidget(deleteButton, row, 2);
					connect(deleteButton, &QPushButton::clicked, [=]()
					{
						SceneMgr::get_instance().get_render_setting()->removeRenderLayer(lineEdit2->text().toInt());
						gridLayout->removeWidget(deleteButton);
						gridLayout->removeWidget(lineEdit);
						gridLayout->removeWidget(lineEdit2);
						deleteButton->deleteLater();
						lineEdit->deleteLater();
						lineEdit2->deleteLater();
					});
				});
			});
		}		
		else
		{
			auto lineEdit = new QLineEdit(QString::fromStdString(layer.second));
			gridLayout->addWidget(lineEdit, row, 0);
			auto lineEdit2 = new QLineEdit(QString::number(layer.first));
			gridLayout->addWidget(lineEdit2, row, 1);
			QPushButton* deleteButton = new QPushButton("Delete");
			gridLayout->addWidget(deleteButton,row,2);
			connect(deleteButton, &QPushButton::clicked, [=]()
			{
				SceneMgr::get_instance().get_render_setting()->removeRenderLayer(lineEdit2->text().toInt());
				gridLayout->removeWidget(deleteButton);
				gridLayout->removeWidget(lineEdit);
				gridLayout->removeWidget(lineEdit2);
				deleteButton->deleteLater();
				lineEdit->deleteLater();
				lineEdit2->deleteLater();
			});
		}
		++row;
	}
}

void RenderSettingDialog::closeEvent(QCloseEvent* event)
{
	this->destroy();
	QDialog::closeEvent(event);
}