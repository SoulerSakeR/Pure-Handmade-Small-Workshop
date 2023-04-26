#include "TextureEditorDialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QComboBox>
#include "Core/Core/Debug.h"
#include "Core/SystemStatus/GameEngine.h"

TextureEditorDialog::TextureEditorDialog(Texture2D* texture, QWidget* parent): QDialog(parent), texture(texture)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("Texture Editor");
	QGridLayout* layout = new QGridLayout(this);
	QLabel* label = new QLabel("Texture Name:", this);
	layout->addWidget(label, 0, 0);
	auto textureName = new QLineEdit(QString::fromStdString(texture->get_name()), this);
	layout->addWidget(textureName, 0, 1);
	label = new QLabel("Image Path:", this);
	layout->addWidget(label, 1, 0);
	auto texturePath = new QLineEdit(QString::fromStdString(texture->get_img_path()), this);
	layout->addWidget(texturePath, 1, 1);
	auto fileButton = new QPushButton("...", this);
	fileButton->setFixedWidth(20);
	fileButton->setAutoDefault(false);
	layout->addWidget(fileButton, 1, 2);
	connect(fileButton, &QPushButton::clicked, this, [texturePath]()
	{
		QString path = QFileDialog::getOpenFileName(nullptr, "Open Image", GameEngine::get_instance().getGamePath().c_str(), "Image Files (*.png *.jpg *.bmp)");
		if (!path.isEmpty())
		{
			texturePath->setText(path);
		}
	});
	label = new QLabel("Auto Generate MipMap:", this);
	layout->addWidget(label, 2, 0);
	auto autoMipMap = new QCheckBox(this);
	layout->addWidget(autoMipMap, 2, 1);
	autoMipMap->setCheckState(texture->get_mipmap() ? Qt::Checked : Qt::Unchecked);
	label = new QLabel("Minification Filter:", this);
	layout->addWidget(label, 3, 0);
	auto minFilter = new QComboBox(this);
	minFilter->addItem("Nearest");
	minFilter->addItem("Linear");
	minFilter->addItem("Nearest MipMap Nearest");
	minFilter->addItem("Nearest MipMap Linear");
	minFilter->addItem("Linear MipMap Nearest");	
	minFilter->addItem("Linear MipMap Linear");
	minFilter->setCurrentIndex(texture->get_minification_filter());
	layout->addWidget(minFilter, 3, 1);
	label = new QLabel("Magnification Filter:", this);
	layout->addWidget(label, 4, 0);
	auto magFilter = new QComboBox(this);
	magFilter->addItem("Nearest");
	magFilter->addItem("Linear");
	magFilter->addItem("Nearest MipMap Nearest");
	magFilter->addItem("Nearest MipMap Linear");
	magFilter->addItem("Linear MipMap Nearest");
	magFilter->addItem("Linear MipMap Linear");
	magFilter->setCurrentIndex(texture->get_magnification_filter());
	layout->addWidget(magFilter, 4, 1);
	label = new QLabel("Wrap Mode:", this);
	layout->addWidget(label, 5, 0);
	auto wrapMode = new QComboBox(this);
	wrapMode->addItem("Repeat");
	wrapMode->addItem("Mirrored Repeat");
	wrapMode->addItem("Clamp To Edge");
	wrapMode->addItem("Clamp To Border");
	wrapMode->setCurrentIndex(texture->get_wrap_mode());
	layout->addWidget(wrapMode, 5, 1);
	auto saveButton = new QPushButton("Save", this);
	saveButton->setAutoDefault(false);
	layout->addWidget(saveButton, 6, 1);
	connect(saveButton, &QPushButton::clicked, this, [this, textureName, texturePath, autoMipMap, minFilter, magFilter, wrapMode]()
	{
		if(!this->texture->set_name(textureName->text().toStdString()))
		{
			Debug::warningBox(this, "Texture2D with name " + textureName->text().toStdString() + " already exist");
			return;
		}
		this->texture->set_img_path(texturePath->text().toStdString());
		this->texture->set_mipmap(autoMipMap->checkState() == Qt::Checked);
		this->texture->set_minification_filter(minFilter->currentIndex());
		this->texture->set_magnification_filter(magFilter->currentIndex());
		this->texture->set_wrap_mode(wrapMode->currentIndex());
		this->texture->save();
		this->accept();
	});
}
