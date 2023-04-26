#include "TextureSelectorWidget.h"
#include <QHBoxLayout>
#include "Core/ResourceManagement/ResourceMgr.h"
#include "Core/UI/TextureSelectorDialog.h"

using namespace std;

TextureSelectorWidget::TextureSelectorWidget(QWidget* parent, Property* property) :QWidget(parent), property(property)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	string textureName = property->get_data<string>();
	if (textureName=="None")
		textureLabel = new QLabel("None");
	else if (ResourceMgr::get_instance().isTextureExist(textureName))
		textureLabel = new QLabel(QString::fromStdString(textureName));
	else
		textureLabel = new QLabel(QString::fromStdString(textureName).append("(not found)"));
	layout->addWidget(textureLabel);
	textureButton = new QPushButton("...");
	textureButton->setFixedWidth(20);
	layout->addWidget(textureButton);
	connect(textureButton, &QPushButton::clicked, this, &TextureSelectorWidget::onButtonClicked);
}

TextureSelectorWidget::~TextureSelectorWidget()
{
	delete textureLabel;
	delete textureButton;
}

void TextureSelectorWidget::onButtonClicked()
{
	TextureSelectorDialog dialog(this);
	if (dialog.exec() == QDialog::Accepted)
	{
		auto textureName = dialog.getSelectedTextureName();
		property->get_component()->set_property(property, &textureName);
		textureLabel->setText(QString::fromStdString(textureName));
	}
}