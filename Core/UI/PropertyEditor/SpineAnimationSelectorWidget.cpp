#include "SpineAnimationSelectorWidget.h"
#include <qboxlayout.h>
#include <QLabel>
#include <QPushButton>
#include "Core/ResourceManagement/ResourceMgr.h"
#include "Core/Core/Property.h"
#include "Core/UI/AnimationSelectorDialog.h"

using namespace std;

SpineAnimationSelectorWidget::SpineAnimationSelectorWidget(QWidget* parent, Property* property):QWidget(parent), property(property)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	string spineName = property->get_data<string>();
	if (spineName == "None")
		spineAnimationLabel = new QLabel("None");
	else if (ResourceMgr::get_instance().isExist<SpineAnimationData>(spineName))
		spineAnimationLabel = new QLabel(QString::fromStdString(spineName));
	else
		spineAnimationLabel = new QLabel(QString::fromStdString(spineName).append("(not found)"));
	layout->addWidget(spineAnimationLabel);
	spineButton = new QPushButton("...");
	spineButton->setFixedWidth(20);
	layout->addWidget(spineButton);
	connect(spineButton, &QPushButton::clicked, this, &SpineAnimationSelectorWidget::onButtonClicked);
}

SpineAnimationSelectorWidget::~SpineAnimationSelectorWidget()
{
	delete spineAnimationLabel;
	delete spineButton;
}

void SpineAnimationSelectorWidget::onButtonClicked()
{
	AnimationSelectorDialog dialog(this);
	if (dialog.exec() == QDialog::Accepted)
	{
		auto spineName = dialog.getSelectedSpineName();
		property->set_data<std::string>(spineName);
		spineAnimationLabel->setText(QString::fromStdString(spineName));
	}
}